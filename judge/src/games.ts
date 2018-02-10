import { mapList, mapName } from "./maplist";
import { config } from "./config";
import { spawn } from "child_process";
import { gameDB, teamDB } from "./db";
import { updateElo, Team, getTeamsSortByElo } from "./teams";
import { readFileSync, writeFileSync } from "fs";
import { randomInteger } from "./utils";

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

export async function game(players: string[], id: number, options = defaultOptions) {
  return new Promise<Game>((res, rej) => {
    const maps = (Object.keys(mapList) as mapName[]).filter((key: mapName) => mapList[key] === players.length);
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
      args.push(`"sh ${config.botsPath}/${p}/bot.sh exec ${p}-${id}"`);
    })

    // Linea para ejecutar los finishes:
    let finishes = "";
    players.forEach((p) => {
      finishes += `sh ${config.botsPath}/${p}/bot.sh finish ${p}-${id} ; `;
    });

    const process = spawn("/bin/sh", [], { shell: true });
    const result = [] as Array<string | Buffer>;

    process.stdout.on("data", (data) => {
      result.push(data);
    });

    console.log("python " + args.reduce((a, b) => a + " " + b) + " ; " + finishes + " exit\n");
    process.stdin.write("python " + args.reduce((a, b) => a + " " + b) + " ; " + finishes + " exit\n")

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

/**
 * Generates an array of teams which are fair to play against t. `t` is always included in this array. 
 * @param t Team which is going to participate.
 * @param size (Integer) Size of game. Must be at least 2 and a valid value (not bigger than 10, not bigger than the amount of teams).
 * @param ts (Optional) Array of teams.
 */
export function findOpponents(t: Team|string, size: number, ts?: Team[]) {
  ts = ts || getTeamsSortByElo().filter((team) => !team.disabled);
  if (typeof t === "object") {
    t = t.login;
  }

  const index = ts.findIndex((e) => e.login === t);
  const first = randomInteger(Math.max(0,index-size+1), Math.min(ts.length-size, index));

  // console.log("team: " + t);
  // console.log("team index: " + index);
  // console.log("first: " + first);
  // console.log("size: " + size);

  const gameTeams = [] as string[];
  for (let i = first; i < first+size; i++) {
    gameTeams.push(ts[i].login);
  }

  return gameTeams;
}

export function registerFinishedGame(g: Game) {
  gameDB.push(`/${g.id}`, g);
  updateElo(g);
}
