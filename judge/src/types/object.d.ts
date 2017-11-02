import "";

declare global {
  interface Object {
    values<T>(o: T): T[];
    values(o: any): any[];
  }
}
