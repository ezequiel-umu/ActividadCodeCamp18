let getSessionInfo;
let setSessionInfo;

(() => {
"use strict";

if (typeof(Storage) !== "undefined") {
  main();
} else {
  swal("Loco", "Tu explorador no soporta local-storage", "error");
}


function main() {
  getSessionInfo = () => {
    return JSON.parse(localStorage.cc18sess || "{}");
  }

  setSessionInfo = (sessInfo) => {
    localStorage.cc18sess = JSON.stringify(sessInfo);
  }
}

})();