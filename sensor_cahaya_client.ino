#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define MSG_BUFFER_SIZE  (50)
#define ldr A0                    
#define adc_max 1023
#define adc_ref 5.0
#define resistansi 560
#define skalaLux 125235178.3654270
#define exponen -1.604568157

const char* ssid = "Warnet Putri";
const char* password = "poiuytrewq";
const char* mqtt_server = "192.168.100.117";
int durasi,jarak;  

WiFiClient espClient;
PubSubClient client(espClient);

char msg[MSG_BUFFER_SIZE];

void setup_wifi() {
  delay(10);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length){
  for (int i = 0; i < length; i++) {
  }
}

void reconnect() {
  while (!client.connected()) {
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
    } else {
      delay(5000);
    }
  }
}

void setup() {
  setup_wifi();
  Serial.begin(115200);
  
  pinMode(ldr,INPUT);
 
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  
  client.loop();

  short intensitas;
  double tResistor, tLdr;
  double rLdr;
  double luxLdr;

  intensitas=analogRead(ldr);

  tResistor=(float)intensitas/adc_max*adc_ref;
  tLdr=adc_ref-tResistor;
  rLdr=tLdr/tResistor*resistansi;
  luxLdr=skalaLux*pow(rLdr,exponen);

  double vIn=luxLdr;
  char vOut=(char)vIn;

  Serial.print("Intensitas Cahaya : ");
  Serial.print(intensitas);
  Serial.println(" lux");
  
  snprintf (msg, MSG_BUFFER_SIZE, "Intensitas Cahaya : %ld lux",vOut);
  
  client.publish("sensor_ldr", msg);
  delay(1000);
}
