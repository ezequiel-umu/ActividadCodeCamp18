export const config = {
  // Server:
  port: parseInt(process.env.PORT || "3000", 10),
  
  // Paths:
  antsPath: "../ants/linux",
  dbPath: "./db",
  gameInProgressPath: "./game_logs",
  gameFinishedPath: "./game_finished",
  botsPath: "./bots",
  htmlPath: "html",
  uploadPath: "uploads",
  
  // Elo config
  initialElo: 1200,                                     // Elo inicial al subir un nuevo bot.
  eloBet: 10,                                           // Cuanto ELO tiene que apostar cada equipo para jugar.

}