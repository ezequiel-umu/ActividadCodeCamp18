import fs = require("fs");
import { config } from "./config";
import express = require("express");
import { game, HighPriorityQueue } from "./scheduler";
import { AsyncArray } from "ts-modern-async";

// Create game directories.
try {
  fs.mkdirSync(config.gameFinishedPath);
} catch (e) {

}
try {
  fs.mkdirSync(config.gameInProgressPath);
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

app.get("/:id", (req, res) => {
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

app.get("/", (req, res) => {
  lastId = lastId + 1;
  const id = lastId;
  
  fs.writeFileSync("game_logs/lastid.json", "{\"id\":" + id + "}");
  
  HighPriorityQueue.produce({
    id,
    game: () => game([["python", "../ants/linux/sample_bots/python/HunterBot.py"],
      ["python", "../ants/linux/sample_bots/python/LeftyBot.py"]], id)
  });
  res.send("Generating game with id " + id + ".");
});

app.ws("/ws", (ws, req) => {

})

app.listen(config.port, () => {
  console.log("Listening to port " + config.port);
});