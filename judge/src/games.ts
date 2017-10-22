import { mapList, mapName } from "./maplist";
import { config } from "./config";
import { spawn } from "child_process";
import { gameDB } from "./db";
import { updateElo } from "./teams";

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

function grep(haystack: Array<string | Buffer>, needle: RegExp) {

  function searchInLine(line: string) {
    return needle.test(line);
  }

  let acum = "";
  for (const k of haystack) {
    acum += k.toString();
    let m;
    while (m = /^(.*)\n/.exec(acum)) {
      const line = m[1];
      if (searchInLine(line)) {
        return line;
      } else {
        acum = acum.substring(line.length + 1);
      }
    }
  }
}

export async function game(players: string[], id: number, options = defaultOptions) {
  return new Promise<Game>((res, rej) => {
    const maps = Object.keys(mapList).filter((key: mapName) => mapList[key] === players.length);
    const randomMap = maps[(Math.random() * maps.length) | 0]

    const args = [config.antsPath + "/playgame.py",
      "-g", "" + id,
      "-S",
      "-m", config.antsPath + "/" + randomMap,
      "--player_seed", "" + ((Math.random() * 65535) | 0),
      "--end_wait", "" + options.turnTime,
      "--verbose",
      "--log_dir", config.gameInProgressPath,
      "--turns", "" + options.turns];
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
        const score = grep(result, /^score/);
        if (score) {
          const m = /^score ([0-9 ]+)$/.exec(score);
          if (m) {
            const scores = m[1].split(" ").map((e) => parseInt(e, 10));
            res({
              id,
              teams: players,
              scores,
            });
          } else {
            rej(new Error("Score incorrectly parsed in game " + id));
          }
        } else {
          console.log(result.reduce((a,b) => a + b.toString(), ""));
          rej(new Error("Score not found in game " + id));
        }
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