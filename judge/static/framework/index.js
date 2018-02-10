let mainPromise;

(() => {
  "use strict";

const router = {
  "inicio": "/",
  "clasificacion": "/clasificacion.html",
  "visualizador": "/visualizador.html",
  "login": "/login.html",
  "perfil": "/perfil.html",
}

const routerInverso = {};

for (const k in router) {
  routerInverso[router[k]] = k;
}

const navbar = `
<nav class="navbar navbar-expand-lg navbar-light bg-light">
 <a class="navbar-brand" href="./"><img src="images/codecamp.png"/ width="22">{CodeCamp}</a>
  <button class="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarSupportedContent" aria-controls="navbarSupportedContent" aria-expanded="false" aria-label="Toggle navigation">
    <span class="navbar-toggler-icon"></span>
  </button>

  <div class="collapse navbar-collapse" id="navbarSupportedContent">
    <ul class="navbar-nav mr-auto">
      <li id="inicio" class="nav-item">
        <a class="nav-link" href="./">Inicio</a>
      </li>
      <li id="clasificacion" class="nav-item">
        <a class="nav-link" href="./clasificacion.html">Clasificación</a>
      </li>
      <li id="visualizador" class="nav-item">
        <a class="nav-link" href="./visualizador.html">Ver partidas</a>
      </li>
    </ul>
    <ul class="navbar-nav navbar-right">
      <li id="login" class="nav-item" hidden>
        <a class="nav-link" href="./login.html">Login</a>
      </li>
    </ul>
  </div>
</nav>
`

async function loadScript(source) {
  return new Promise((res, rej) => {
    const script = document.createElement('script');
    script.onload = function () {
      res();
    };
    script.onerror = function(err) {
      rej(err);
    }
    script.crossOrigin = "anonymous";
    script.src = source;
    document.head.appendChild(script); //or something of the likes
  })
}

async function loadCss(source) {
  return new Promise((res, rej) => {
    const script = document.createElement('link');
    script.onload = function () {
      res();
    };
    script.onerror = function(err) {
      rej(err);
    }
    script.rel = "stylesheet";
    script.href = source;
    document.head.appendChild(script); //or something of the likes
  })
}

async function scriptDeps() {
  await loadScript("https://code.jquery.com/jquery-3.2.1.slim.min.js");
  await loadScript("https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.12.3/umd/popper.min.js");
  await loadScript("https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0-beta.2/js/bootstrap.min.js");
  await loadScript("https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0-beta.2/js/bootstrap.min.js");
  await loadScript("https://unpkg.com/sweetalert/dist/sweetalert.min.js");
  await Promise.all([
    loadScript("/js/store.js"),
    loadScript("/js/API.js"),
  ]);
}

async function cssDeps() {
  await loadCss("https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0-beta.2/css/bootstrap.min.css")    
}

async function deps() {
  await Promise.all([
    cssDeps(), scriptDeps()
  ])
}

function resaltarMenu() {
  // Resaltar la página actual
  const path = window.location.pathname;
  const id = routerInverso[path];
  $("#"+id).addClass("active");
}

function mostrarLogin() {
  const session = getSessionInfo();
  if (session.login) {
    $("#login > a").text(session.login);
    $("#login > a").attr("href", "/perfil.html");
    $("#login").attr("id", "perfil");
    $("#perfil").attr("hidden", false);
  } else {
    $("#login").attr("hidden", false);
  }
}

function desactivarBotones() {
  const session = getSessionInfo();
  if (!session.login) {
    $("#visualizador").hide();
  }
}

async function main() {
  await deps();
  // Adjuntar la barra de navegación.
  $("body").prepend(navbar);
  
  desactivarBotones();
  mostrarLogin();
  resaltarMenu();
}

mainPromise = main();

})()