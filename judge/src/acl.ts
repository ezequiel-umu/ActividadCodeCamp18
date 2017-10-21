import * as core from "express-serve-static-core";

export function acl(req: core.Request, res: core.Response, next: core.NextFunction) {
  if (req.session && req.session.login) {
    next();
  } else {
    res.statusCode = 401;
    res.send("Not authorized.")
  }
}