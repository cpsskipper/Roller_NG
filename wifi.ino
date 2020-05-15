
void init_wifi(){
  WiFi.disconnect();
  WiFi.hostname(client_id);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
