const suhu = document.getElementById("suhu"),
     suhu2 = document.getElementById("suhu2"),
     lembap = document.getElementById("lembap"),
     gasTxt = document.getElementById("gas"),
     alertBx = document.querySelector(".alert"),
     ip = "esp32_ip_address",
     endpoint = `http://${ip}`;

function getSuhu() {
     fetch(endpoint + "/suhu")
          .then((response) => response.json())
          .then((data) => {
               suhu.innerHTML = Math.round(data.suhu);
               suhu2.innerHTML = Math.round(data.suhu);
               lembap.innerHTML = Math.round(data.lembap);
               // return data;
          })
          .catch((error) => {
               console.error("Error fetching data:", error);
          });
}
function getGas() {
     fetch(endpoint + "/gas")
          .then((response) => response.json())
          .then((data) => {
               if (data.gas > 0) {
                    gasTxt.innerHTML = "ADA";
                    document.body.classList.add("warning");
               } else {
                    gasTxt.innerHTML = "TIDAK";
                    document.body.classList.remove("warning");
               }
          })
          .catch((error) => {
               console.error("Error fetching data:", error);
          });
}

setInterval(() => {
     getSuhu();
     getGas();
}, 500);
