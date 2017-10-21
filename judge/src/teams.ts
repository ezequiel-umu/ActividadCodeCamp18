import { teamDB } from "./db";

interface Team {
  login: string;
  password: string;
}

/**
 * Finds team. Complexity: O(1) 
 * @param login Team's login
 */
export function findTeamByLogin(login: string): Team | null {
  try {
    return teamDB.getData(`/${login}`);
  } catch (e) {
    return null;
  }
}

/**
 * Finds team, then checks password. Complexity: O(1) 
 * @param login Team's login
 */
export function findTeamByLoginAndPassword(login: string, password: string): Team | null {
  const team = findTeamByLogin(login);
  return (team && team.login === login && team.password === password) ? team : null;
}

