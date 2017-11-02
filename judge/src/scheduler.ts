import fs = require("fs");
import { spawn } from "child_process";
import { config } from "./config";
import { mapList, mapName } from "./maplist";
import { AsyncArray } from "ts-modern-async/lib";
import { Game, registerFinishedGame, nextGameId, game, findOpponents } from "./games";
import { FunnelPriorityArray, randomInteger, move } from "./utils";
import { getTeamsSortByElo, Team } from "./teams";

interface GameInProgress {
  game: () => Promise<Game>;
  id: number;
}

export const HighPriorityQueue: AsyncArray<GameInProgress> = new AsyncArray();
export const LowPriorityQueue: AsyncArray<GameInProgress> = new AsyncArray();

async function consumeGamesLoop(queues: AsyncArray<GameInProgress>[]) {
  const gameQueue = new FunnelPriorityArray(queues);
  let broken = false;
  function breakLoop() {
    broken = true;
  }
  setTimeout(async () => {
    while (!broken) {
      const g = await gameQueue.consume();
      console.log("Calculating game " + g.id + ".");
      const gameResult = await g.game();
      console.log("Finished game " + gameResult.id + ".");
      // Mover las repeticiones 
      await move(config.gameInProgressPath + "/" + g.id + ".replay", config.gameFinishedPath + "/" + g.id + ".replay");
      await move(config.gameInProgressPath + "/replay." + g.id + ".html", config.htmlPath + "/" + g.id + ".html");
      fs.writeFileSync(config.htmlPath + "/" + g.id + ".html", fs.readFileSync(config.htmlPath + "/" + g.id + ".html").toString().replace(/\.\.\/\.\.\/ants\/linux/g, ""));

      // Memorizar el orden actual de los equipos:
      const teams = getTeamsSortByElo();

      // Registrar el juego acabado:
      registerFinishedGame(gameResult);

      // Comprobar si los equipos han cambiado de orden después de la partida
      const teams2 = getTeamsSortByElo();

      /**
       * Lista de equipos que han cambiado su orden.
       */
      const highPriorityTeams = new Set<string>();

      for (let i = 0; i < teams.length; i++) {
        if (teams[i].login !== teams2[i].login) {
          highPriorityTeams.add(teams[i].login);
          highPriorityTeams.add(teams2[i].login);
        }
      }

      // Generar partidas para los equipos que han cambiado de puesto.
      while (highPriorityTeams.size) {
        const team = highPriorityTeams.keys().next().value;
        const size = randomInteger(2, Math.min(5, teams2.length));

        const opponents = findOpponents(team, size, teams2);
        
        for (const opponent of opponents) {
          highPriorityTeams.delete(opponent);
        }

        const id = nextGameId();
        HighPriorityQueue.produce({
          id,
          game: () => game(opponents, id),
        });
      }
    }
  }, 0);
  return breakLoop;
}

export const stopLoop = consumeGamesLoop([HighPriorityQueue, LowPriorityQueue]);
