async function doGet(url, data) {
  return new Promise((res, rej) => {
    const xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
      if (this.readyState === XMLHttpRequest.DONE && this.status < 400) {
        res(xhttp.responseText);
      } else if (this.readyState === XMLHttpRequest.DONE) {
        rej({
          message: xhttp.responseText,
          statusCode: this.status
        });
      }
    };
    xhttp.onerror = rej;
    if (data) {
      xhttp.open("GET", url + "?" + encodeObject(data), true);
    } else {
      xhttp.open("GET", url, true);      
    }
    xhttp.send();
  });
}

async function doPost(url, data) {
  return new Promise((res, rej) => {
    const xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
      if (this.readyState === XMLHttpRequest.DONE && this.status < 400) {
        res(xhttp.responseText);
      } else if (this.readyState === XMLHttpRequest.DONE) {
        rej({
          message: xhttp.responseText,
          statusCode: this.status
        });
      }
    };
    xhttp.onerror = rej;
    xhttp.open("POST", url, true);
    console.log(data);
    if (data instanceof File) {
      const fd = new FormData();
      fd.append("bot.tar.gz", data);
      console.log("seding file");
      xhttp.send(fd);
    } else if (typeof data === "object") {
      xhttp.setRequestHeader("Content-Type", "application/json");
      xhttp.send(JSON.stringify(data));            
    } else {
      xhttp.send(data);      
    }
  });
}