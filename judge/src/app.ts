import fs = require("fs");
import { config } from "./config";
import express = require("express");
import { game } from "./scheduler";
import { AsyncArray } from "ts-modern-async";


// Id of the last game.
let id = 0;
try { 
  const importedId = require("../game_logs/lastid");
} catch (e) {

}

let lastFinishedId = id;

const app = express();
require("express-ws")(app);

interface Game {
  id: number;
  game: Promise<Array<string | Buffer>>;
}

const gameQueue = new AsyncArray<Game>();

app.get("/:id", (req, res) => {
  const first = gameQueue[0];
  const id = parseInt(req.params.id, 10);
  if (!first || first.id > id) {
    if (fs.existsSync("./game_logs/" + id + ".stream")) {
      res.setHeader("content-type", "text/plain");
      fs.createReadStream("./game_logs/" + id + ".stream").pipe(res);
    } else {
      res.statusCode = 404;
      res.send("Not found");
    }
  } else {
    console.log(gameQueue);
    res.statusCode = 404;
    res.send("Game in progress.");
  }
});

app.get("/", (req, res) => {
  id = id + 1;
  fs.writeFileSync("game_logs/lastid.json", "{\"id\":" + id + "}");

  const index = gameQueue.produce({
    id,
    game: game(["\"../ants/linux/sample_bots/python/HunterBot.py\"",
      "\"../ants/linux/sample_bots/python/GreedyBot.py\""], id)
  });
  res.send("Generating game with id " + id + ".");
});

app.ws("/ws", (ws, req) => {

})

async function consumeEveryGame() {
  while (true) {
    const g = await gameQueue.consume();
    console.log("Calculating game "+g.id+".");
    await g.game;
    console.log("Finished game "+g.id+".");
    lastFinishedId = g.id;
  }
}

consumeEveryGame();

app.listen(config.port, () => {
  console.log("Listening to port " + config.port);
});