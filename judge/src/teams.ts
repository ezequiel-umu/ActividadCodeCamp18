import { teamDB, gameDB } from "./db";
import { config } from "./config";
import { Game } from "./games";
import tar = require("tar");
import fs = require("mz/fs");
import {copyFileSync} from "fs"
import { silentMkdir } from "./app";
import { spawn } from "mz/child_process";
import util = require("util");
import rr = require("rimraf");

const rimraf = util.promisify(rr);


export const BotRuntimeList = {
  "C++": true,
} 

type BotRuntime = keyof typeof BotRuntimeList;

export interface Team {
  login: string;
  password: string;
  elo: number;
  botRuntime: BotRuntime;
  disabled: boolean;
}

export function isBotRuntime(runtime: string): runtime is BotRuntime {
  return runtime in BotRuntimeList;
}

/**
 * Finds team. Complexity: O(1)
 * @param login Team's login
 */
export function findTeamByLogin(login: string): Team | null {
  try {
    return teamDB.getData(`/${login}`);
  } catch (e) {
    return null;
  }
}

/**
 * List teams. Complexity: O(n*log(n)).
 */
export function getTeamsSortByElo(): Team[] {
  return Object.values<Team>(teamDB.getData(`/`)).sort((a,b) => a.elo - b.elo);
}

/**
 * List teams. Complexity: O(1).
 */
export function getTeams(): Team[] {
  return Object.values<Team>(teamDB.getData(`/`));
}

/**
 * Finds team, then checks password. Complexity: O(1)
 * @param login Team's login
 */
export function findTeamByLoginAndPassword(login: string, password: string): Team | null {
  const team = findTeamByLogin(login);
  return (team && team.login === login && team.password === password) ? team : null;
}

export function restartElo() {
  const teams = teamDB.getData("/");
  for (const k in teams) {
    const team = teams[k] as Team;
    team.elo = config.initialElo;
  }

  teamDB.push("/", teams);

  const games = gameDB.getData("/");
  for (const k in games) {
    const game = games[k] as Game;
    updateElo(game);
  }
}

export function updateElo(game: Game) {
  const teams = teamDB.getData("/");
  const n = game.teams.length;
  const mean = game.scores.reduce((a, b) => a + b) / game.scores.length;
  const bet = n * config.eloBet;
  const positions = game.scores.map((e, i) => ({ score: e, team: game.teams[i], elo: teams[game.teams[i]].elo }));
  const totalScore = positions.reduce((a, b) => a + b.score, 0);
  let totalElo = positions.reduce((a, b) => a + b.elo, 0);

  if (totalScore == 0) {
    return;
  }

  for (let i = 0; i < n; i++) {
    const expectedScore = (positions[i].elo / totalElo) * totalScore;
    const pWin = ((positions[i].score - expectedScore)/totalScore) * bet;
    teams[positions[i].team].elo = positions[i].elo + pWin;
  }
  teamDB.push("/", teams);
}

export async function compile(teamName: string, file: Express.Multer.File, kind: BotRuntime) {
  const team = findTeamByLogin(teamName) as Team;
  team.disabled = true;
  team.elo = config.initialElo;
  team.botRuntime = kind;
  teamDB.push("/" + teamName, team);
  await rimraf(config.botsPath + "/" + teamName);
  silentMkdir(config.botsPath + "/" + teamName);
  await tar.x({
    cwd: config.botsPath + "/" + teamName,
    file: file.path
  });
  await fs.unlink(file.path);
  await fs.unlink(config.botsPath + "/" + teamName + "/bot.sh")
  copyFileSync(config.runtimes + "/" + kind, config.botsPath + "/" + teamName + "/bot.sh")
  await execCompile(teamName);
  team.disabled = false;
  teamDB.push("/" + teamName, team);
}

export async function execCompile(teamName: string) {
  return new Promise((res, rej) => {
    
    const process = spawn("/bin/sh", [], { shell: true });
    const result = [] as Array<string | Buffer>;

    process.stdout.on("data", (data) => {
      // Do nothing
    });

    console.log(`sh ./bots/${teamName}/bot.sh build ; exit\n`);
    process.stdin.write(`sh ./bots/${teamName}/bot.sh build ; exit\n`)

    process.on("exit", (code) => {
      if (code === 0) {
        res();
      } else {
        rej(new Error("Exited with status code: " + code));
      }
    });
  });
}
