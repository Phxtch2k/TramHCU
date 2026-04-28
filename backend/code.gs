function doPost(e) {
  try {

    const raw = JSON.parse(e.postData.contents);

    const data = {
      station: raw.station,
      rssi: raw.rssi,
      time: new Date().toISOString()
    };

    const baseUrl = "https://trackingrealtime-9a5a1-default-rtdb.firebaseio.com/tram";

    // 1️⃣ status (ล่าสุด)
    UrlFetchApp.fetch(baseUrl + "/status.json", {
      method: "put",
      contentType: "application/json",
      payload: JSON.stringify(data)
    });

    // 2️⃣ logs (history)
    UrlFetchApp.fetch(baseUrl + "/logs.json", {
      method: "post",
      contentType: "application/json",
      payload: JSON.stringify(data)
    });

    return ContentService.createTextOutput("OK");

  } catch (err) {
    return ContentService.createTextOutput("ERROR: " + err);
  }
}
