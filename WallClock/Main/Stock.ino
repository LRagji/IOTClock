// #define SERVER "finnhub.io"

// void read_price(int x_i, int y_i, String stock_name) {
//   WiFiSSLClient client;
//   if (client.connect(SERVER, 443)) {
//     const String path = "/api/v1/quote?symbol=" + stock_name + "&token=" + FINNHUB_TOKEN;
//     Serial.println("Connecting to server : " + path);
//     // Make a HTTP request:
//     client.println("GET " + path + " HTTP/1.1");
//     client.println("Host: " SERVER);
//     client.println("Connection: close");
//     client.println();

//     // Check HTTP status
//     char status[32] = { 0 };
//     client.readBytesUntil('\r', status, sizeof(status));
//     if (strcmp(status, "HTTP/1.1 200 OK") == 0) {
//       // Skip HTTP Headers
//       char doubleNewLine[] = "\r\n\r\n";
//       if (client.find(doubleNewLine, 4) == true) {
//         JsonDocument doc;
//         DeserializationError error = deserializeJson(doc, client);
//         if (!error) {
//           String v = doc["c"];
//           String c = doc["dp"];
//           float current = v.toDouble();
//           float percentchange = c.toDouble();
//           Serial.println("Done:" + v + c);
//         } else {
//           Serial.println("JSON parsing error");
//         }
//       } else {
//         Serial.println("Malformed Response missing headers.");
//       }
//     } else {
//       Serial.println("Invalid Response Code: " + String(status));
//     }
//   } else {
//     Serial.println("Connection Failed");
//   }
// }
