import fs = require("fs");
import { config } from "./config";
import express = require("express");
import session = require("express-session");
import { HighPriorityQueue } from "./scheduler";
import { AsyncArray } from "ts-modern-async";
import { findTeamByLogin, restartElo } from "./teams";
import { acl } from "./acl";
import { game } from "./games";

// Create game directories.
try {
  fs.mkdirSync(config.gameFinishedPath);
} catch (e) {

}

try {
  fs.mkdirSync(config.gameInProgressPath);
} catch (e) {

}

try {
  fs.mkdirSync(config.botsPath);
} catch (e) {

}

try {
  fs.mkdirSync(config.htmlPath);
} catch (e) {

}

// Id of the last game.
let lastId = 0;
try {
  const importedId = require("../game_logs/lastid");
  lastId = importedId.id;
} catch (e) {

}

const app = express();
require("express-ws")(app);
const bodyParser = require("body-parser");

app.use(bodyParser.json());
app.use(session({
  secret: "m8overurbdazn8ir4e98ym",
  resave: true,
  saveUninitialized: false,
}));

const api = express.Router();

api.get("/:id", acl, (req, res) => {
  const id = parseInt(req.params.id, 10);
  if (fs.existsSync(config.gameInProgressPath + "/" + id + ".stream")) {
    res.statusCode = 404;
    res.send("Game in progress.");
  } else if (fs.existsSync(config.gameFinishedPath + "/" + id + ".stream")) {
    res.setHeader("content-type", "text/plain");
    fs.createReadStream(config.gameFinishedPath + "/" + id + ".stream").pipe(res);
  } else {
    res.statusCode = 404;
    res.send("Not found");
  }
});

api.get("/", acl, (req, res) => {
  lastId = lastId + 1;
  const id = lastId;

  fs.writeFileSync("game_logs/lastid.json", "{\"id\":" + id + "}");

  HighPriorityQueue.produce({
    id,
    game: () => game(["hunter",
      "lefty",
      "influence"], id),
  });
  res.send("Generating game with id " + id + ".");
});

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

restartElo();

app.listen(config.port, () => {
  console.log("Listening to port " + config.port);
});

