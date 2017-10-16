interface Game {
  id: number;
  game: () => Promise<Array<string | Buffer>>;
}