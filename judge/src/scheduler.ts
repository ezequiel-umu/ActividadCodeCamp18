import fs = require("fs");
import { spawn } from "child_process";
import { config } from "./config";
import { mapList, mapName } from "./maplist";
import { AsyncArray } from "ts-modern-async/lib";
import { Game, registerFinishedGame } from "./games";

interface GameInProgress {
  game: () => Promise<Game>;
  id: number;
}

export const HighPriorityQueue: AsyncArray<GameInProgress> = new AsyncArray();
export const LowPriorityQueue: AsyncArray<GameInProgress> = new AsyncArray();

class FunnelPriorityArray<T> extends AsyncArray<T> {
  private indexes: number[];

  constructor(arrays: AsyncArray<T>[]) {
    super();

    if (!arrays)
      return;

    this.indexes = arrays.map((e) => 0);

    const registerFunnelConsumer = (e: AsyncArray<T>, i: number) => {
      e.consume().then((t) => {
        if (this.length === 0) {
          this.produce(t);
        } else {
          console.log(this);
          Array.prototype.splice.call(this, this.indexes[i], 0, t);
          console.log(this);
        }
        for (let j = i; j < this.indexes.length; j++) {
          this.indexes[j]++;
        }
        registerFunnelConsumer(e,i);
      }).catch((err) => {
        console.error(err);
        registerFunnelConsumer(e,i);
      });
    }

    arrays.forEach(registerFunnelConsumer);
  }

  public async consume() {
    const result = await super.consume();
    for (let i = 0; i < this.indexes.length; i++) {
      this.indexes[i]--;
    }
    return result;
  }
}

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
      fs.renameSync(config.gameInProgressPath + "/" + g.id + ".stream", config.gameFinishedPath + "/" + g.id + ".stream");
      registerFinishedGame(gameResult);
    }
  }, 0);
  return breakLoop;
}

export const stopLoop = consumeGamesLoop([HighPriorityQueue, LowPriorityQueue]);
