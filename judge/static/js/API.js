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
    xhttp.open("GET", url + "?" + encodeObject(data), true);
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
    if (typeof data === "object") {
      console.log(data);
      xhttp.setRequestHeader("Content-Type", "application/json");
      data = JSON.stringify(data);
    }
    xhttp.send(data);
  });
}