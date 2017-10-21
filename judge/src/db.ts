import JsonDB = require("node-json-db");
import { config } from "./config";

export const teamDB = new JsonDB(config.dbPath + "/teams", true, true);
export const gameDB = new JsonDB(config.dbPath + "/games", true, true);