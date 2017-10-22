export const config = {
  port: parseInt(process.env.PORT || "3000", 10),
  antsPath: "../ants/linux",
  dbPath: "./db",
  gameInProgressPath: "./game_logs",
  gameFinishedPath: "./game_finished",
  botsPath: "./bots",
  initialElo: 50,
  eloBet: 5,
  htmlPath: "html",
}