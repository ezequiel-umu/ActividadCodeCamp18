import fs = require("mz/fs");
import tar = require("tar");
import multer = require("multer");
import { config } from "./config";
import express = require("express");
import session = require("express-session");
import { HighPriorityQueue, LowPriorityQueue } from "./scheduler";
import { AsyncArray } from "ts-modern-async";
import { findTeamByLogin, restartElo, Team, BotRuntimeList, isBotRuntime, getTeams, compile } from "./teams";
import { acl } from "./acl";
import { game, Game, nextGameId, findOpponents } from "./games";
import { teamDB, gameDB } from "./db";
import { expressAsync, randomInteger } from "./utils";
import https = require('https');
import express_enforces_ssl = require('express-enforces-ssl');

// HTTPS options
const httpsoptions = {
  cert: fs.readFileSync(config.certPath),
  key:  fs.readFileSync(config.keyPath)
};

export function silentMkdir(dir: string) {
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
silentMkdir(config.debugPath);

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

app.use(express_enforces_ssl());

app.use(bodyParser.json());
app.use(session({
  secret: "m8overurbdazn8ir4e98ym",
  resave: true,
  saveUninitialized: false,
}));

app.use(express.static("static", {
  
}));

const api = express.Router();

api.get("/classification", (req, res) => {
  const teams = teamDB.getData("/");
  const arrTeams = [];
  for (const k in teams) {
    arrTeams.push({
      team: k,
      elo: (teams[k].elo as number)|0
    });
  }
  res.send(arrTeams.sort((a, b) => a.elo - b.elo));
});

api.get("/game/:id", acl, (req, res) => {
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

api.get("/game/debug/:id", acl, (req, res) => {
  const teamName = req.session && req.session.login as string;
  const id = parseInt(req.params.id, 10);
  if (fs.existsSync(config.debugPath + "/" + id + "-" + teamName + ".log")) {
    res.setHeader("content-type", "text/plain");
    fs.createReadStream(config.debugPath + "/" + id + "-" + teamName + ".log").pipe(res);
  } else {
    res.statusCode = 404;
    res.send("Not found");
  }
});

api.get("/game", acl, (req, res) => {
  const games = gameDB.getData("/") as Game[];
  res.send(games);
});

function GenerateRoundGame() {
  const arrTeams = getTeams().filter((t) => !t.disabled);
  const teams = new Set<string>(arrTeams.map((t) => t.login));
  while (teams.size) {
    const id = nextGameId(); 
    const team = teams.keys().next().value;
    const size = randomInteger(2, Math.min(5, arrTeams.length));

    const opponents = findOpponents(team, size, arrTeams);
    
    for (const opponent of opponents) {
      teams.delete(opponent);
    }

    LowPriorityQueue.produce({
      id,
      game: () => game(opponents, id),
    });
  }
}

api.get("/", acl, (req, res) => {
  GenerateRoundGame();
  res.send("Generating a whole round of games.");
});

api.post("/bot", acl, uploader.single("bot.tar.gz"), expressAsync(async (req, res) => {
  const teamName = (req.session && req.session.login) as string;
  const file = req.file;
  const kind = req.query.kind;
  if (teamName && file && isBotRuntime(kind)) {
    LowPriorityQueue.length = 0;
    HighPriorityQueue.length = 0;
    await compile(teamName, file, kind);
    GenerateRoundGame(); 
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

api.post("/logout", (req, res) => {
  if (req.session) {
    delete req.session.login;
  }
  res.send("OK");
});

app.use("/api", api);

app.ws("/ws", (ws, req, res) => {

});

restartElo();

app.listen(config.port, () => {
  console.log("Listening to port " + config.port);
});

console.log("Listening to httpsPort "+ config.httpsPort);
https.createServer(httpsoptions, app).listen(config.httpsPort);
