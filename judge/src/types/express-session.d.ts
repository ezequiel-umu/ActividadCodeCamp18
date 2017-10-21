import * as core from "express-serve-static-core";

declare global {
  namespace Express {
    export interface Session {
      login: string;
    }
  }
}