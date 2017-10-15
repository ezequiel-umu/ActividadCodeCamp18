import { spawn } from "child_process";
import { config } from "./config";
import { mapList, mapName } from "./maplist";


export async function game(players: string[], id: number, turns: number = 1000) {

  return new Promise<Array<string | Buffer>>((res, rej) => {
    const maps = Object.keys(mapList).filter((key: mapName) => mapList[key] === players.length);
    const randomMap = maps[(Math.random() * maps.length) | 0]

    const args = [config.antsPath + "/playgame.py",
      "-g", ""+id,
      "-So",
      "-m", config.antsPath + "/" + randomMap,
      "--player_seed", "" + ((Math.random() * 65535) | 0),
      "--end_wait", "0.5",
      "--verbose",
      "--log_dir", "game_logs",
      "--turns", "" + turns];
    players.forEach((p) => {
      args.push(p)
    })

    // console.log("python " + args.reduce((a, b) => a + " " + b));

    const process = spawn("python", args);
    const result = [] as Array<string | Buffer>;

    process.stdout.on("data", (data) => {
      result.push(data);
    });

    process.on("exit", (code) => {

      if (code === 0) {
        res(result);
      } else {
        rej(code);
      }
    });
  });
}
