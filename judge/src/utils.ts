import * as mv from "mv";
import { AsyncArray } from "ts-modern-async/lib";
import * as core from "express-serve-static-core";

export class FunnelPriorityArray<T> extends AsyncArray<T> {
  private indexes: number[];

  constructor(arrays: AsyncArray<T>[]) {
    super();

    
    if (!arrays) {
      this.indexes = [];
      return;
    }
    
    this.indexes = arrays.map((e) => 0);

    const registerFunnelConsumer = (e: AsyncArray<T>, i: number) => {
      e.consume().then((t) => {
        if (this.length === 0) {
          this.produce(t);
        } else {
          // console.log(this);
          Array.prototype.splice.call(this, this.indexes[i], 0, t);
          // console.log(this);
        }
        for (let j = i; j < this.indexes.length; j++) {
          this.indexes[j]++;
        }
        registerFunnelConsumer(e, i);
      }).catch((err) => {
        console.error(err);
        registerFunnelConsumer(e, i);
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

export function expressAsync(a: (req: core.Request, res: core.Response) => Promise<any>) {
  return (r: core.Request, rs: core.Response, next: core.NextFunction) => {
    a(r, rs).then((n) => next()).catch(next);
  }
}

export function randomInteger(min: number, max: number) {
  return Math.round(min + Math.random() * (max - min));
}

export async function move(src: string, dest: string, options?: { mkdirp?: boolean; clobber?: boolean; }) {
  return new Promise((res, rej) => {
    if (options) {
      mv(src, dest, options, (err) => {
        if (err) {
          rej(err);
        } else {
          res();
        }
      })
    } else {
      mv(src, dest, (err) => {
        if (err) {
          rej(err);
        } else {
          res();
        }
      })
    }
  });
}