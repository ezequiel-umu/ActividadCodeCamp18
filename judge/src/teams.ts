import { teamDB, gameDB } from "./db";
import { config } from "./config";
import { Game } from "./games";

export const BotRuntimeList = {
  "C++": true,
  "Java": true,
  "Pascal": true,
  "Python": true,
} 

type BotRuntime = keyof typeof BotRuntimeList;

export interface Team {
  login: string;
  password: string;
  elo: number;
  botRuntime: BotRuntime;
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

declare global {
  interface Object {
    values<T>(o: T): T[];
    values(o: any): any[];
  }
}

/**
 * List teams. Complexity: O(n*log(n)).
 */
export function getTeamsSortByElo(): Team[] {
  return Object.values<Team>(teamDB.getData(`/`)).sort((a,b) => a.elo - b.elo);
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

  for (let i = 0; i < n; i++) {
    const expectedScore = (positions[i].elo / totalElo) * totalScore;
    const pWin = ((positions[i].score - expectedScore)/totalScore) * bet;
    teams[positions[i].team].elo = positions[i].elo + pWin;
  }
  teamDB.push("/", teams);
}