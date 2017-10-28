import { mapList, mapName } from "./maplist";
import { config } from "./config";
import { spawn } from "child_process";
import { gameDB } from "./db";
import { updateElo } from "./teams";
import { readFileSync, writeFileSync } from "fs";

// Id of the last game.
let lastId = 0;
try {
  const importedId = require("../game_logs/lastid");
  lastId = importedId.id;
} catch (e) {

}

export function nextGameId() {
  const id = ++lastId;
  writeFileSync("game_logs/lastid.json", "{\"id\":" + id + "}");
  return id;
}

export interface Game {
  scores: number[];
  teams: string[];
  id: number;
}

interface GameOptions {
  turns: number;
  turnTime: number;
}

const defaultOptions = {
  turns: 1000,
  turnTime: 0.5,
}

// function grep(haystack: Array<string | Buffer>, needle: RegExp) {

//   function searchInLine(line: string) {
//     return needle.test(line);
//   }

//   let acum = "";
//   for (const k of haystack) {
//     acum += k.toString();
//     let m;
//     while (m = /^(.*)\n/.exec(acum)) {
//       const line = m[1];
//       if (searchInLine(line)) {
//         return line;
//       } else {
//         acum = acum.substring(line.length + 1);
//       }
//     }
//   }
// }

export async function game(players: string[], id: number, options = defaultOptions) {
  return new Promise<Game>((res, rej) => {
    const maps = Object.keys(mapList).filter((key: mapName) => mapList[key] === players.length);
    const randomMap = maps[(Math.random() * maps.length) | 0]

    const args = [config.antsPath + "/playgame.py",
      "-g", "" + id,
      "--nolaunch",
      "-m", config.antsPath + "/" + randomMap,
      "--player_seed", "" + ((Math.random() * 65535) | 0),
      "--end_wait", "" + options.turnTime,
      "--verbose",
      "--log_dir", config.gameInProgressPath,
      "--html=replay." + id + ".html",
      "--turns", "" + options.turns,
    ];
    players.forEach((p) => {
      args.push(`"sh ${config.botsPath}/${p}/bot.sh exec"`);
    })

    const process = spawn("/bin/sh", [], { shell: true });
    const result = [] as Array<string | Buffer>;

    process.stdout.on("data", (data) => {
      result.push(data);
    });

    console.log("python " + args.reduce((a, b) => a + " " + b) + " ; exit\n");
    process.stdin.write("python " + args.reduce((a, b) => a + " " + b) + " ; exit\n")

    process.on("exit", (code) => {
      if (code === 0) {
        const replay = JSON.parse(readFileSync(config.gameInProgressPath + "/" + id + ".replay").toString());
        res({
          id,
          teams: players,
          scores: replay.score,
        });
      } else {
        rej(code);
      }
    });
  });
}

export function registerFinishedGame(g: Game) {
  gameDB.push(`/${g.id}`, g);
  updateElo(g);
}