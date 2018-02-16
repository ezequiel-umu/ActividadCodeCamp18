export const config = {
  // Server:
  port: parseInt(process.env.PORT || "8080", 10),
  httpsPort: parseInt(process.env.HTTPS || "4443", 10),

  // Certs:
  certPath: "/home/codecampia/codecampia.cert",
  keyPath: "/home/codecampia/codecampia.key",
  
  // Paths:
  antsPath: "../ants/linux",
  dbPath: "./db",
  gameInProgressPath: "./game_logs",
  gameFinishedPath: "./game_finished",
  botsPath: "./bots",
  htmlPath: "html",
  uploadPath: "uploads",
  debugPath: "game_debugs",
  runtimes: "runtimes",
  
  // Elo config
  initialElo: 1200,                                     // Elo inicial al subir un nuevo bot.
  eloBet: 10,                                           // Cuanto ELO tiene que apostar cada equipo para jugar.

}