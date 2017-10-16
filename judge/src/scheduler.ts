import fs = require("fs");
import { spawn } from "child_process";
import { config } from "./config";
import { mapList, mapName } from "./maplist";
import { AsyncArray } from "ts-modern-async/lib";

export const HighPriorityQueue: AsyncArray<Game> = new AsyncArray();
export const LowPriorityQueue: AsyncArray<Game> = new AsyncArray();

export async function game(players: string[][], id: number, turns: number = 1000) {
  return new Promise<Array<string | Buffer>>((res, rej) => {
    const maps = Object.keys(mapList).filter((key: mapName) => mapList[key] === players.length);
    const randomMap = maps[(Math.random() * maps.length) | 0]

    const args = [config.antsPath + "/playgame.py",
      "-g", "" + id,
      "-S",
      "-m", config.antsPath + "/" + randomMap,
      "--player_seed", "" + ((Math.random() * 65535) | 0),
      "--end_wait", "0.5",
      "--verbose",
      "--log_dir", config.gameInProgressPath,
      "--turns", "" + turns];
    players.forEach((p) => {
      args.push(`"${p.reduce((a, b) => a + " " + b)}"`);
    })

    const process = spawn("/bin/sh", [], {shell: true});
    const result = [] as Array<string | Buffer>;

    process.stdout.on("data", (data) => {
      // console.log(data.toString());
      result.push(data);
    });

    // console.log("python " + args.reduce((a, b) => a + " " + b) + " ; exit\n");
    process.stdin.write("python " + args.reduce((a, b) => a + " " + b) + " ; exit\n")

    process.on("exit", (code) => {

      if (code === 0) {
        res(result);
      } else {
        rej(code);
      }
    });
  });
}

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

async function consumeGamesLoop(queues: AsyncArray<Game>[]) {
  const gameQueue = new FunnelPriorityArray(queues);
  let broken = false;
  function breakLoop() {
    broken = true;
  }
  setTimeout(async () => {
    while (!broken) {
      const g = await gameQueue.consume();
      console.log("Calculating game " + g.id + ".");
      await g.game();
      console.log("Finished game " + g.id + ".");
      fs.renameSync(config.gameInProgressPath + "/" + g.id + ".stream", config.gameFinishedPath + "/" + g.id + ".stream");
    }
  }, 0)
  return breakLoop;
}

export const stopLoop = consumeGamesLoop([HighPriorityQueue, LowPriorityQueue]);