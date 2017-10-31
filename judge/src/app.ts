import fs = require("mz/fs");
import tar = require("tar");
import multer = require("multer");
import { config } from "./config";
import express = require("express");
import session = require("express-session");
import { HighPriorityQueue } from "./scheduler";
import { AsyncArray } from "ts-modern-async";
import { findTeamByLogin, restartElo, Team, BotRuntimeList, isBotRuntime } from "./teams";
import { acl } from "./acl";
import { game, Game, nextGameId } from "./games";
import { teamDB, gameDB } from "./db";
import { expressAsync } from "./utils";

function silentMkdir(dir: string) {
  try {
    fs.mkdirSync(dir);
    return null;
  } catch (e) {
    return e as Error;
  }
}

// Create game directories.
silentMkdir(config.gameFinishedPath);
silentMkdir(config.gameInProgressPath);
silentMkdir(config.botsPath);
silentMkdir(config.htmlPath);
silentMkdir(config.uploadPath);

const app = express();
require("express-ws")(app);
const bodyParser = require("body-parser");
const uploader = multer({
  dest: config.uploadPath,
  limits: {
    fileSize: 524288, // 512KiB
  },
  fileFilter: (req, file, cb) => {
    if (file.mimetype === "application/gzip")
      cb(null, true);
    else
      cb(null, false);
  }
});

app.use(bodyParser.json());
app.use(session({
  secret: "m8overurbdazn8ir4e98ym",
  resave: true,
  saveUninitialized: false,
}));

app.use(express.static("static", {

}));

const api = express.Router();

api.get("/classification", acl, (req, res) => {
  const teams = teamDB.getData("/");
  const arrTeams = [];
  for (const k in teams) {
    arrTeams.push({
      team: k,
      elo: teams[k].elo as number
    });
  }
  res.send(arrTeams.sort((a, b) => a.elo - b.elo));
});

api.get("/game/:id", (req, res) => {
  const id = parseInt(req.params.id, 10);
  if (fs.existsSync(config.gameInProgressPath + "/" + id + ".replay")) {
    res.statusCode = 404;
    res.send("Game in progress.");
  } else if (fs.existsSync(config.htmlPath + "/" + id + ".html")) {
    res.setHeader("content-type", "text/html");
    fs.createReadStream(config.htmlPath + "/" + id + ".html").pipe(res);
  } else {
    res.statusCode = 404;
    res.send("Not found");
  }
});

api.get("/game", (req, res) => {
  const games = gameDB.getData("/") as Game[];
  res.send(games);
});

api.get("/", acl, (req, res) => {
  const id = nextGameId();

  HighPriorityQueue.produce({
    id,
    game: () => game(["hunter",
      "lefty",
      "influence"], id),
  });
  res.send("Generating game with id " + id + ".");
});

api.post("/bot", acl, uploader.single("bot.tar.gz"), expressAsync(async (req, res) => {
  const teamName = req.session && req.session.login as string;
  const team = teamDB.getData("/" + teamName) as Team;
  const file = req.file;
  const kind = req.query.kind;
  if (team && file && isBotRuntime(kind)) {
    team.elo = config.initialElo;
    team.botRuntime = kind;
    teamDB.push("/" + teamName, team);
    silentMkdir(config.botsPath + "/" + teamName);
    await tar.x({
      cwd: config.botsPath + "/" + teamName,
      file: file.path
    });
    await fs.unlink(file.path);
    res.send("OK");
  } else {
    res.statusCode = 400;
    res.send("Something wrong");
  }
}));

api.post("/login", (req, res) => {
  const { login, password } = req.body;
  if (typeof login === "string" && typeof password === "string") {
    const team = findTeamByLogin(login);
    if (team && team.password === password) {
      if (req.session) {
        req.session.login = login;
      }
      res.statusCode = 200;
      res.send("OK");
    } else {
      res.statusCode = 401;
      res.send("Forbidden.");
    }
  } else {
    res.statusCode = 400;
    res.send("Bad request.");
  }
});

app.use("/api", api);

app.ws("/ws", (ws, req, res) => {

});

// restartElo();

app.listen(config.port, () => {
  console.log("Listening to port " + config.port);
});

