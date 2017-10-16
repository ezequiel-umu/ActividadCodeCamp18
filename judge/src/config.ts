export const config = {
  port: parseInt(process.env.PORT || "3000", 10),
  antsPath: "../ants/linux",
  gameInProgressPath: "./game_logs",
  gameFinishedPath: "./game_finished",
}