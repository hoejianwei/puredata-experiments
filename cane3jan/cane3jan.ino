#include <MPU9250_WE.h>
#include <Wire.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h> 

// --- WIFI SETTINGS ---
const char* ssid = "jw";        
const char* pass = "03876794";    

IPAddress outIp(172, 20, 10, 11); 
const unsigned int outPort = 8000;          

// --- SENSOR SETTINGS ---
#define MPU9250_ADDR 0x68
MPU9250_WE myMPU = MPU9250_WE(MPU9250_ADDR);

WiFiUDP udp; 

void setup() {
  Serial.begin(115200);
  Wire.begin();

  // 1. Connect to WiFi
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("My IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("Target IP: ");
  Serial.println(outIp);

  // 2. Init Sensor
  if(!myMPU.init()){
    Serial.println("MPU Init Failed");
    while(1);
  }
  myMPU.autoOffsets();
  myMPU.setMagOpMode(AK8963_CONT_MODE_100HZ);
  
  Serial.println("Starting OSC transmission...");
}

void loop() {
  xyzFloat acc = myMPU.getGValues();
  xyzFloat gyr = myMPU.getGyrValues();
  // xyzFloat mag = myMPU.getMagValues(); 

  // --- SERIAL DEBUGGING (Print to Serial Monitor) ---
  Serial.print("Acc: ");
  Serial.print(acc.x); Serial.print(", ");
  Serial.print(acc.y); Serial.print(", ");
  Serial.print(acc.z);
  
  Serial.print(" | Gyr: ");
  Serial.print(gyr.x); Serial.print(", ");
  Serial.print(gyr.y); Serial.print(", ");
  Serial.println(gyr.z); // Newline at the end for readability

  // --- SEND ACCELEROMETER OVER WIFI ---
  OSCMessage msgAcc("/accel"); 
  msgAcc.add(acc.x);
  msgAcc.add(acc.y);
  msgAcc.add(acc.z);

  msgAcc.add(gyr.x);
  msgAcc.add(gyr.y);
  msgAcc.add(gyr.z);

  udp.beginPacket(outIp, outPort);
  msgAcc.send(udp);
  udp.endPacket();
  msgAcc.empty();

  delay(50); 
}