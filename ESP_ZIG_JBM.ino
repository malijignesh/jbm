// updated 30/08/19 for jbm
/* update log:
   1. time stamp added for topic "topic/CU" i.e current values only
   2. retention of data if no mqtt connection
   3. cwar size changed from 8 to 11 characters
   4. cwar is also store in esp
   5. the host web page thet appears while using interupt is also changed , an input for current threshold ia added
    caution :- enter the threshold value always in 3 digits ex:- 067 056 129 etc.
 * */
//#include <FS.h>
#include"Arduino.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <DNSServer.h>
#include <WiFiManager.h>
#include<EEPROM.h>
#include "rsa.h"
#include <string.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
//#include <IRremoteESP8266.h>
//#include<IRsend.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
String sss = "";
int coun = 0, ky = 0, ky1 = 0, ky2 = 0, ky3 = 0, ky4 = 0;
//#include "IPaddpass.h"
int khz = 38;
int ccnt = 0;
//IRsend irsend(15);
//IPAddress setip(192, 168, 1, 18); //set static ip
//IPAddress gateip(192, 168, 1, 1); //set getteway
//IPAddress subip(255, 255, 255, 0);//set subnet
//IPAddress dnsip(8, 8, 8, 8);//set subnet



int sizes = 0, sizep = 0, sizest = 0, sizegate = 0, sizesub = 0, sizedns = 0, sizecal = 0, sizecur = 0;;
int sizetopic = 0, sizemqtt = 0, sizebit = 0;
//String mq1, topic_main;
//char* mqt11,maintopic;
long RSA_Own_Private_Key[3] = {323, 5, 173};
const int MOST_NO_OF_DIGITS = 10;
String mqtt = "";
unsigned long trig = 0;
String kill = "";
int flag1 = 0;
String ac = "";
boolean recon_flag = true;



long lastReconnectAttempt = 0;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800); //utc ofset in seconds 5.5*60*60(gmt+5:30)


long add[5];


SoftwareSerial swSer(14, 12);//, false, 256); //Rx,Tx
int flag = 0;
String sssid;
String  stip;
String  gateway;
String subnet ;
String  dnsserver;
//const char*  stip2;
//const char*  gateway2;
//const char* subnet2 ;
//const char*  dnsserver2;
String pass, typee;
String topic = "";
char typeee;
String Argument_Name, Clients_Response1, Clients_Response2;
const char* ssid = "GW032";
const char* password = "LLRIT123";
ESP8266WebServer server(80);
IPAddress myIP;

WiFiClient espClient;
PubSubClient client(espClient);
//=========================================================================SETUP
//=========================================================================SETUP
void setup(void) {
  Serial.begin(115200);
  Serial.println("setup");

  lastReconnectAttempt = 0;
  //  irsend.begin();
  pinMode(13, OUTPUT);
  pinMode(2, OUTPUT);
  digitalWrite(13, 0);
  attachInterrupt(digitalPinToInterrupt(13), isr_flag , RISING);
  EEPROM.begin(512);

  swSer.begin(9600);
  Serial.println("");

  Wire.begin();
  station();

  typeee = EEPROM.read(0);
  sizetopic = EEPROM.read(4);
  sizemqtt = EEPROM.read(3);
  mqtt = "";
  topic = "";

  for (int i = 0; i < sizemqtt; i++)
  {
    mqtt += char( EEPROM.read(6 + sizes + sizep + i));
  }
  for (int i = 0; i < sizetopic - 1; i++)
  {
    topic += char( EEPROM.read(6 + i + sizes + sizep + sizemqtt));
  }


  Serial.println("LIMELIGHT READER HARDWARE V 5.0 FIRMWARE V4.2");
  char mqadd[sizemqtt + 1];
  mqtt.toCharArray(mqadd, sizemqtt + 1);
  client.setServer(mqadd, 1883);
  client.setCallback(callback);
  timeClient.begin();

}


void callback(char* topic, byte* payload, unsigned int length) {
  Wire.beginTransmission(9);
  String acbuf;

  for (int i = 0; i < length; i++) {

    Serial.print((char)payload[i]);
    if (payload[0] == 'l' || payload[0] == 'q') {
      Wire.write(char(payload[i]));

    }
    else
    {
      acbuf += (char)payload[i];
    }

  }


  if (payload[0] != 'l' && payload[0] != 'q') {
    decryptir(acbuf);
  }
  else


    Wire.endTransmission();
  Serial.println();

}


boolean reconnect() {

  // Loop until we're reconnected
  char topic_main[sizetopic];
  topic.toCharArray(topic_main, sizetopic);
  String tltf = topic, tlcf = topic;
  tltf += "/BT";
  //tacf += "/BT2";
  tlcf += "/LC";
  char tl[tltf.length()], tlc[tlcf.length()];
  tltf.toCharArray(tl, tltf.length() + 1);
  //tacf.toCharArray(tac, tacf.length() + 1);
  tlcf.toCharArray(tlc, tlcf.length() + 1);
  Serial.println(String(topic_main));
  //  while (!client.connected()) {
  //    Serial.print("Attempting MQTT connection...");
  if (client.connect(topic_main)) {

    Serial.println("connected");
    client.subscribe(tl);//"ADMININ/LIGHT");
    client.subscribe(tlc);//"ADMININ/AC");
    device_helth();
    recon_flag = true;
  }
  return client.connected();


  //    else {
  //      Wire.beginTransmission(9);
  //      Wire.write("l4");
  //      ccnt++;
  //      Serial.print("failed, rc=");
  //      Serial.print(client.state());
  //      Serial.println(" try again in 5 seconds");
  //      delay(1000);
  //      if (ccnt == 6)
  //      {
  //        ESP.reset();
  //      }
  //    }
  //}
}

void rsa_en() {
  Serial.println(micros());
  long* k;//]={85,98};

  k = rsa_encrypt("100000", RSA_Own_Private_Key);
  Serial.print(k[0]);
  Serial.print(",");
  Serial.print(k[1]);
  Serial.print(",");
  Serial.print(k[2]);
  Serial.print(",");
  Serial.print(k[3]);
  Serial.print(",");
  Serial.print(k[4]);
}


void isr_flag()
{
  flag = 1;
  Serial.println("INTERRUPT ENTERED");
}

void HandleClient() {
  String webpage;
  //String IPaddress = WiFi.localIP().toString();
  //String IPaddress = "192.168.4.1";
  String IPaddress = myIP.toString();
  webpage = "<!DOCTYPE html><html><head><title>Configuration</title><style>body {background-color: #2E2E2E;font-family: Courier, Helvetica, Sans-Serif;Color: white;}.header {font-family: Consolas;color: lightgoldenrodyellow;}.main {border: 1px solid white;border-radius: 10px;padding: 30px;width: 60%;height: auto;}label {text-align: right;padding: 10px;}.textbox, .check {border-radius: 3px;border: 1px solid grey;margin: 10px;transition: all 0.5s;height: 20px;outline: none;padding: 5px;}.textbox:hover, .check:hover {box-shadow: 0px 0px 10px white;}.button {border-radius: 5px;background: #000;border: 1px solid white;padding: 5px;margin: 10px;display: block;color: white;font-size: 16px;transition: all 0.5s;padding: 5px 10px 5px 10px;align-self: center;}.button:hover {background: #fff;color: black;}input[type=checkbox] {width: 10px;height: 10px;border: 1px solid black;background: white;}</style></head><body><center><div class=\"header\"><h1>Lime Light Infotech</h1></div><div class=\"main\"><h3>Configuration</h3><hr><form action=\"http://" + IPaddress + "\" method=\"POST\"><table class=\"table\"><tr><td><label for=\"ssid\">SSID:</label></td><td><input type=\"text\" name=\"ssid\" class=\"textbox\" placeholder=\"Enter SSID\" id=\"ssid\"></td></tr><tr><td><label for=\"password\">Password: </label></td><td><input type=\"password\" name=\"password\" class=\"textbox\" placeholder=\"Enter Password\" id=\"password\"></td></tr><tr><td><label for=\"stip\">STIP:</label></td><td><input type=\"text\" name=\"stip\" class=\"textbox\" placeholder=\"Enter STIP\" id=\"gateway\"></td></tr><tr><td><label for=\"gateway\">GATEWAY:</label></td><td><input type=\"text\" name=\"gateway\" class=\"textbox\" placeholder=\"Enter GATEWAY\" id=\"gateway\"></td></tr><tr><td><label for=\"subnet\">SUBNET:</label></td><td><input type=\"text\" name=\"subnet\" class=\"textbox\" placeholder=\"Enter SUBNET\" id=\"subnet\"></td></tr><tr><td><label for=\"subnet\">DNSSERVER:</label></td><td><input type=\"text\" name=\"dnsserver\" class=\"textbox\" placeholder=\"Enter DNSSERVER\" id=\"dnsserver\"></td></tr><tr><td><label for=\"mqtt\">MQTT Address: </label></td><td><input type=\"text\" name=\"mqtt\" class=\"textbox\" placeholder=\"Enter MQTT address\" id=\"mqtt\"></td></tr><tr><td><label for=\"mqtttopic\">MQTT Topic: </label></td><td><input type=\"text\" name=\"mqtttopic\" class=\"textbox\" placeholder=\"Enter MQTT Topic\" id=\"mqtttopic\"></td></tr><tr><td><label for=\"type\">MQTT Type: </label></td><td><select name=\"environment\" id=\"env\"><option value=\"1\" selected>Select</option><option value=\"1\">IS</option><option value=\"2\">OS</option><option value=\"3\">PC</option></select></td></tr><tr><td><label for=\"env\">Environment: </label></td><td><select name=\"environment\" id=\"env\"><option value=\"1\">Yes</option><option value=\"0\" selected>No</option></select></td></tr><tr><td><label for=\"power\">Power(Current): </label></td><td><select name=\"power\" id=\"power\"><option value=\"1\">Yes</option><option value=\"0\" selected>No</option></select></td></tr><tr><td><label for=\"rfid433\">433 RFID: </label></td><td><select name=\"rfid433\" id=\"rfid433\"><option value=\"1\">Yes</option><option value=\"0\" selected>No</option></select></td></tr><tr><td><label for=\"rfid24\">2.4 RFID: </label></td><td><select name=\"rfid24\" id=\"rfid24\"><option value=\"1\">Yes</option><option value=\"0\" selected>No</option></select></td></tr><tr><td><label for=\"auto\">Automation: </label></td><td><select name=\"automation\" id=\"auto\"><option value=\"1\">Yes</option><option value=\"0\" selected>No</option></select></td></tr><tr><td><label for=\"access\">Access: </label></td><td><select name=\"access\" id=\"access\"><option value=\"1\">Yes</option><option value=\"0\" selected>No</option></select></td></tr><tr><td><label for=\"ir\">IR: </label></td><td><select name=\"ir\" id=\"ir\"><option value=\"1\">Yes</option><option value=\"0\" selected>No</option></select></td></tr><tr><td><label for=\"ir\">Power: </label></td><td><select name=\"ir\" id=\"ir\"><option value=\"1\">Low</option><option value=\"2\">Min</option><option value=\"3\">High</option><option value=\"4\">Max</option></select></td></tr></table><tr><td><label for=\"ir\">Threshold: </label></td><td><input type=\"text\" name=\"threshold\" class=\"textbox\" placeholder=\"Enter threshold\" id=\"threshold\"></td></tr><label for=\"ir\">Current: </label></td><td><input type=\"text\" name=\"current\" class=\"textbox\" placeholder=\"Enter current\" id=\"current\"></td></tr><label for=\"ir\">CALIBRATION: </label></td><td><input type=\"text\" name=\"calibration\" class=\"textbox\" placeholder=\"Enter CALIBRATION\" id=\"calibration\"></td></tr><button type=\"submit\" value=\"Submit\" class=\"button\">Submit</button></form></div></center></body></html>";
  server.send(200, "text/html", webpage); // Send a response to the client asking for input
  if (server.args() > 0 ) { // Arguments were received
    sssid = server.arg(0);
    pass = server.arg(1);
    //        String c ="c";

    String stip = server.arg(2);
    stip += ".";
    String gateway = server.arg(3);
    gateway += ".";
    String subnet = server.arg(4);
    subnet += ".";
    String dnsserver = server.arg(5);
    dnsserver += ".";

    mqtt = server.arg(6);
    topic = server.arg(7);
    typee = server.arg(8);
    String environment = server.arg(9);
    String power = server.arg(10);
    String rfid433 = server.arg(11);
    String rfid24 = server.arg(12);
    String automation = server.arg(13);
    String access = server.arg(14);
    String ir = server.arg(15);
    String pr = server.arg(16);
    String thr = server.arg(17);
    String cur = server.arg(18);
    String cal = server.arg(19);

    Serial.print("SSID: ");
    Serial.println(sssid);
    Serial.print("Password: ");
    Serial.println(pass);
    Serial.print("stip: ");
    Serial.println(stip);
    Serial.print("gateway: ");
    Serial.println(gateway);
    Serial.print("subnet: ");
    Serial.println(subnet);
    Serial.print("dnsserver: ");
    Serial.println(dnsserver);
    Serial.print("MQTT Address: ");
    Serial.println(mqtt);
    Serial.print("Topic: ");
    Serial.println(topic);
    Serial.print("type: ");
    Serial.println(typee);
    Serial.print("Environment: ");
    Serial.println(environment);
    Serial.print("Power: ");
    Serial.println(power);
    Serial.print("433 RFID: ");
    Serial.println(rfid433);
    Serial.print("2.4 RFID: ");
    Serial.println(rfid24);
    Serial.print("Automation: ");
    Serial.println(automation);
    Serial.print("Access: ");
    Serial.println(access);
    Serial.print("IR: ");
    Serial.println(ir);
    Serial.print("power: ");
    Serial.println(pr);
    Serial.print("Threshold: ");
    Serial.println(thr);
    Serial.print("Current: ");
    Serial.println(cur);
    Serial.print("caliration: ");
    Serial.println(cal);


    String bits = 'c' + environment + power + rfid433 + rfid24 + automation + access + ir + pr + thr + cal + cur ;
    Serial.println(bits);

    //=======================================================================================================================
    //=======================================================================================================================

    char ssid1[sssid.length() + 1];
    sssid.toCharArray(ssid1, sssid.length() + 1);

    char pass1[pass.length() + 1];
    pass.toCharArray(pass1, pass.length() + 1);

    char mqtt1[mqtt.length() + 1];
    mqtt.toCharArray(mqtt1, mqtt.length() + 1);

    char topic1[topic.length() + 1];
    topic.toCharArray(topic1, topic.length() + 1);

    char bits1[bits.length() + 1];
    bits.toCharArray(bits1, bits.length() + 1);

    char stip1[stip.length() + 1];
    stip.toCharArray(stip1, stip.length() + 1);

    char gateway1[gateway.length() + 1];
    gateway.toCharArray(gateway1, gateway.length() + 1);

    char subnet1[subnet.length() + 1];
    subnet.toCharArray(subnet1, subnet.length() + 1);

    char dnsserver1[dnsserver.length() + 1];
    dnsserver.toCharArray(dnsserver1, dnsserver.length() + 1);

    char cal1[cal.length() + 1];
    cal.toCharArray(cal1, cal.length() + 1);

    char cur1[cur.length() + 1];
    cur.toCharArray(cur1, cur.length() + 1);

    //=======================================================================================================================
    //=======================================================================================================================

    Wire.beginTransmission(9);//this block of code sends datastreams of bit to atmega got by handleclient ==>> line no: 302
    Wire.write(bits1);
    Wire.endTransmission();
    Serial.println(mqtt);

    //===================================================================

    EEPROM.begin(512);
    // swSer.print(bits1);
    Serial.println(sizeof(ssid1));
    Serial.println(sizeof(pass1));

    //=======================================================================================================================
    //=======================================================================================================================
    EEPROM.write(0, typee.charAt(0));//============================
    EEPROM.commit();
    EEPROM.write(1, sizeof(ssid1));//============================
    EEPROM.commit();
    EEPROM.write(2, sizeof(pass1));//============================
    EEPROM.commit();
    EEPROM.write(3, sizeof(mqtt1));//============================
    EEPROM.commit();
    EEPROM.write(4, sizeof(topic1));//============================
    EEPROM.commit();
    EEPROM.write(5, sizeof(bits1));//============================
    EEPROM.commit();
    EEPROM.write(6, sizeof(stip1));//============================
    EEPROM.commit();
    EEPROM.write(7, sizeof(gateway1));//============================
    EEPROM.commit();
    EEPROM.write(8, sizeof(subnet1));//============================
    EEPROM.commit();
    EEPROM.write(9, sizeof(dnsserver1));//============================
    EEPROM.commit();
    EEPROM.write(10, sizeof(cal1));//============================
    EEPROM.commit();
    EEPROM.write(11, sizeof(cur1));//============================
    EEPROM.commit();

    //=======================================================================================================================
    //=======================================================================================================================
    Serial.println("EEPROM WR");
    //============================//1
    //    Serial.println("");
    //    Serial.print("ssid :");
    for (int i = 0; i < sizeof(ssid1); i++)
    { EEPROM.write(11 + i, ssid1[i]);
      //      Serial.print(ssid1[i]);
      EEPROM.commit();
    }

    //============================//2
    //    Serial.println("");
    //    Serial.print("pass :");
    for (int i = 0; i < sizeof(pass1); i++)
    {
      EEPROM.write(11 + sizeof(ssid1) + i, pass1[i]);
      //    Serial.print(pass1[i]);
      EEPROM.commit();
    }
    //============================//3
    //   Serial.println("");
    //   Serial.print("mqtt :");
    for (int i = 0; i < sizeof(mqtt1); i++)

    {
      EEPROM.write(11 + sizeof(pass1) + sizeof(ssid1) + i, mqtt1[i]);
      //  Serial.print(mqtt1[i]);
      EEPROM.commit();
    }

    //============================//4
    //    Serial.println("");
    //    Serial.print("topic :");
    for (int i = 0; i < sizeof(topic1); i++)
    {
      EEPROM.write(11 + sizeof(pass1) + sizeof(ssid1) + sizeof(mqtt1) + i, topic1[i]);
      //  Serial.print(topic[i]);
      EEPROM.commit();
    }

    //============================//5
    //    Serial.println("");
    //    Serial.print("bits :");
    for (int i = 0; i < sizeof(bits1); i++)
    {
      EEPROM.write(11 + sizeof(pass1) + sizeof(ssid1) + sizeof(mqtt1) + sizeof(topic1) + i, bits1[i]);
      //      Serial.println(bits[i]);
      EEPROM.commit();
    }

    //============================//6
    Serial.println("");
    Serial.print("stip :");
    for (int i = 0; i < sizeof(stip1); i++)
    {
      EEPROM.write(11 +  sizeof(pass1) + sizeof(ssid1) + sizeof(mqtt1) + sizeof(topic1) + sizeof( bits1) + i, stip1[i]);
      Serial.print(stip1[i]);
      //      delay(2000);

      EEPROM.commit();
    }

    //============================//7
    Serial.println("");
    Serial.print("sgateway :");
    for (int i = 0; i < sizeof(gateway1); i++)

    {
      EEPROM.write(11 +  sizeof(pass1) + sizeof(ssid1) + sizeof(mqtt1) + sizeof(topic1) + sizeof( bits1)  + sizeof( stip1) + i, gateway1[i]);
      Serial.print(gateway1[i]);
      EEPROM.commit();
    }

    //============================//8
    Serial.println("");
    Serial.print("subnet :");

    for (int i = 0; i < sizeof(subnet1); i++)
    {
      EEPROM.write(11 +  sizeof(pass1) + sizeof(ssid1) + sizeof(mqtt1) + sizeof(topic1) + sizeof( bits1) + sizeof( stip1) + sizeof(gateway1) + i, subnet1[i]);
      Serial.print(subnet1[i]);
      //      delay(2000);
      EEPROM.commit();
    }

    //============================//9
    Serial.println("");
    Serial.print("dnsserverr :");
    for (int i = 0; i < sizeof(dnsserver1); i++)
    {
      EEPROM.write(11 +  sizeof(pass1) + sizeof(ssid1) + sizeof(mqtt1) + sizeof(topic1) + sizeof( bits1) + sizeof( stip1) + sizeof(gateway1) + sizeof(subnet1) + i, dnsserver1[i]);
      Serial.print(dnsserver1[i]);
      EEPROM.commit();
    }

    Serial.println("");
    Serial.print("Calibration1 :");
    for (int i = 0; i < sizeof(cal1); i++)
    {
      EEPROM.write(11 +  sizeof(pass1) + sizeof(ssid1) + sizeof(mqtt1) + sizeof(topic1) + sizeof( bits1) + sizeof( stip1) + sizeof(gateway1) + sizeof(subnet1) + sizeof (dnsserver1) + i , cal1[i]);
      Serial.print(cal1[i]);
      EEPROM.commit();
    }

    Serial.println("");
    Serial.print("Current1 :");
    for (int i = 0; i < sizeof(cur1); i++)
    {
      EEPROM.write(11 +  sizeof(pass1) + sizeof(ssid1) + sizeof(mqtt1) + sizeof(topic1) + sizeof( bits1) + sizeof( stip1) + sizeof(gateway1) + sizeof(subnet1) + sizeof (dnsserver1) + sizeof (cal1)  + i , cur1[i]);
      Serial.print(cur1[i]);
      EEPROM.commit();
    }

    //=======================================================================================================================
    //=======================================================================================================================
    //    delay(2000);
    ESP.restart();
  }
}


String gettime()
{
  timeClient.update();

  //  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  //  Serial.print(", ");
  //  Serial.print(timeClient.getHours());
  //  Serial.print(":");
  //  Serial.print(timeClient.getMinutes());
  //  Serial.print(":");
  //  Serial.println(timeClient.getSeconds());
  //Serial.println(timeClient.getFormattedTime());
  return timeClient.getFormattedTime();
}


void loop(void) {
  // Serial.println("loop");
  if (flag == 1) {
    ap();
    flag = 0;
  }
  if (!client.connected()) {
    long now = millis();
    if (now - lastReconnectAttempt > 5000) {
      lastReconnectAttempt = now;
      // Attempt to reconnect

      recon_flag = false;

      Wire.beginTransmission(9);
      Wire.write("l4");
      ccnt++;
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(1000);
      if (ccnt == 6)
      {
        ESP.restart();
      }



      if (reconnect()) {
        lastReconnectAttempt = 0;
        recon_flag = true;
      }
    }

  } else {
    client.loop();
    recon_flag = true;
  }

  if (recon_flag || kill.length() > 200)  /// if disconneted the kill value is preserved, if connected the kill is initialised additionally there is a length limit for safe operation
    kill = "";

  String ttef = topic, thuf = topic, tidf = topic, tcuf = topic;
  ttef += "/TE";
  thuf += "/HU";
  tcuf += "/CU";
  if (typeee == '1')
    tidf += "/IS";
  else if (typeee == '2')
    tidf += "/OS";
  else if (typeee == '3')
    tidf += "/PC";
  //Serial.println(tidf);
  char tte[ttef.length() + 1], thu[thuf.length() + 1], tid[tidf.length() + 1], tcu[tcuf.length() + 1];
  ttef.toCharArray(tte, ttef.length() + 1);
  thuf.toCharArray(thu, thuf.length() + 1);
  tidf.toCharArray(tid, tidf.length() + 1);
  tcuf.toCharArray(tcu, tcuf.length() + 1);

  if (swSer.available() > 0) {
    kill = swSer.readString();
    //Serial.println(kill);
    int a = kill.indexOf('d');
    int ida = kill.indexOf('A');
    // int st = kill.indexOf('#');
    int ict = kill.indexOf('i');
    int coma = 0;
    String tmp, hum;
    if (a != -1)
    {
      int sl = kill.indexOf(':');
      int st = kill.indexOf('#', a + 1);
      hum = kill.substring(a + 1, sl);//.toInt();
      tmp = kill.substring(sl + 1, st);//.toInt();
      Serial.println(String(tmp) + "  ____" + String(hum));
      tmp = " " + tmp;
      char mssg[tmp.length() + 1];
      tmp.toCharArray(mssg, tmp.length() + 1);
      client.publish(tte, mssg);

      Serial.println(String(tte) + "  ____" + String(mssg));

      hum.toCharArray(mssg, 3);
      client.publish(thu, mssg);
      Serial.println(String(thu) + "  ____" + String(mssg));

    }
rept1:// loop untill all current values are read
    if (ict != -1)
    {
      int st = kill.indexOf('#', ict + 1);
      String cur = kill.substring(ict + 1, st);//.toInt();
      cur = gettime() + ":" + cur;
      char mssg[cur.length() + 1];
      cur.toCharArray(mssg, cur.length() + 1);
      client.publish(tcu, mssg);
      Serial.println(mssg);
      kill = kill.substring(st);// eliminate the reading once published
      ict = kill.indexOf('i');
      goto rept1;



    }
rept:
    if (ida != -1)
    {
      // Serial.println(kill);
      char mssg1[6] = "";
      mssg1[5] = '\0';
      coma = ida;
      int  st = kill.indexOf('#', ida + 1);

      for (byte co = 0; co < 5;)
      {
        coma = kill.indexOf(',', coma + 1);
        if (co == 4) {
          add[co] = long(kill.substring(ida + 1, st).toInt());
        }
        else
          add[co] = long(kill.substring(ida + 1, coma).toInt());
        ida = coma;


        co++;
      }

      long* kit = (rsa_decrypt(add, RSA_Own_Private_Key, 6));

      mssg1[0] = char(48 + add[0]);
      // Serial.println(mssg1[0]);
      for (byte c1 = 1; c1 < 5;) {
        //  Serial.println(kit[c1]);
        mssg1[c1] = char(kit[c1]);
        c1++;
      }
      //Serial.print("id=");
      //snprintf (mssg2, 5, "%c%c%c%c%c", mssg1[0],mssg1[1],mssg1[2],mssg1[3],mssg1[4]);
      Serial.println(mssg1);
      client.publish(tid, mssg1);
      kill = kill.substring(st);
      ida = kill.indexOf('A');
      goto rept;
    }

  }

  //delay(500);
  //Serial.println("here");
  if (millis() - trig > 30000)
  {
    device_helth();
    trig = millis();
  }
}


void station() {

  WiFi.softAPdisconnect();
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  delay(100);

  //========================================================================================================================================
  //  fetching legnth of string  stored in eeprom in 1st 10bit  (0+9)

  sizes = EEPROM.read(1);
  sizep = EEPROM.read(2);
  sizemqtt = EEPROM.read(3);
  sizetopic = EEPROM.read(4);
  sizebit = EEPROM.read(5);
  sizest = EEPROM.read(6);
  sizegate = EEPROM.read(7);
  sizesub = EEPROM.read(8);
  sizedns = EEPROM.read(9);
  sizecal = EEPROM.read(10);
  sizecur = EEPROM.read(11);

  //========================================================================================================================================

  char ssid1[sizes + 1];
  char pass1[sizep + 1];
  char stip1[sizest + 1];
  char gateway1[sizegate + 1];
  char subnet1[sizesub + 1];
  char dnsserver1[sizedns + 1];
  char cal1[sizecal + 1];
  char cur1[sizecur + 1];

  //========================================================================================================================================
  //========================================================================================================================================
  //========================================================================================================================================

  Serial.println("===============================EEPROM READ IN SETUP==========================  ");
  Serial.println("");
  Serial.print("SSID1   ");

  for (int i = 0; i < sizes + 1; i++)//ADD1
  {
    ssid1[i] = EEPROM.read(11 + i);
  }
  Serial.println(ssid1);

  //========================================================================================================================================
  Serial.print("PASS1   ");

  for (int i = 0; i < sizep; i++)//ADD2
  {
    pass1[i] = EEPROM.read(11 + i + sizes);
  }
  Serial.println(pass1);

  //========================================================================================================================================
  Serial.print("STIP1   ");

  for (int i = 0; i < sizest; i++)//ADD6
  {
    stip1[i] = EEPROM.read(11 + i + sizes + sizep + sizemqtt + sizetopic + sizebit);
    //    Serial.print(stip1[i]);
    //    delay(1000);
  }
  Serial.println(stip1);
  //char* set=stip1;
  //========================================================================================================================================
  Serial.print("GATEWAY1   ");
  for (int i = 0; i < sizegate; i++)//ADD7
  {
    gateway1[i] = EEPROM.read(11 + i + sizes + sizep +  sizemqtt + sizetopic + sizebit + sizest );
    //    delay(1000);
    //    Serial.print(gateway1[i]);
  }

  Serial.println(gateway1);

  //========================================================================================================================================
  Serial.print("subnet1   ");
  for (int i = 0; i < sizesub; i++)//ADD8
  {
    subnet1[i] = EEPROM.read(11 + i + sizes + sizep + sizemqtt + sizetopic + sizebit + sizest + sizegate );
    //    delay(1000);
    //    Serial.print(subnet1[i]);


  }

  Serial.println(subnet1);

  //========================================================================================================================================
  Serial.print("DNSSERVER1   ");
  for (int i = 0; i < sizedns; i++)//ADD9
  {
    dnsserver1[i] = EEPROM.read(11 + i + sizes  + sizep + sizemqtt + sizetopic + sizebit + sizest + sizegate + sizesub);
    //    delay(1000);
    //    Serial.print(dnsserver1[i]);

  }

  Serial.println(dnsserver1);

  //========================================================================================================================================
  Serial.print("CALIBRATION1   ");
  for (int i = 0; i < sizecal; i++)//ADD9
  {
    cal1[i] = EEPROM.read(11 + i + sizes  + sizep + sizemqtt + sizetopic + sizebit + sizest + sizegate + sizesub + sizedns);
    //    delay(1000);
    //    Serial.print(dnsserver1[i]);

  }

  Serial.println(cal1);

  //========================================================================================================================================
  Serial.print("Current1   ");
  for (int i = 0; i < sizecur; i++)//ADD9
  {
    cur1[i] = EEPROM.read(11 + i + sizes  + sizep + sizemqtt + sizetopic + sizebit + sizest + sizegate + sizesub + sizedns + sizecal);
    //    delay(1000);
    //    Serial.print(dnsserver1[i]);

  }

  Serial.println(cur1);


  //========================================================================================================================================
  //========================================================================================================================================



  //================================================================== IPCONFIG SETIP
  sss = "";
  coun = 0; ky1 = 0; ky2 = 0; ky3 = 0; ky4 = 0;

  for ( int i = 0; i <= sizeof(stip1); i++)//stip1
  {
    if ('.' == stip1[i])
    {
      coun += 1;
      i = i + 1;
      if (coun == 1)
      {
        ky1 = sss.toInt();
      }
      else if (coun == 2)
      {
        ky2 = sss.toInt();
      }
      else if (coun == 3)
      {
        ky3 = sss.toInt();
      }
      else if (coun == 4)
      {
        ky4 = sss.toInt();
      }
      sss = "";
    }
    sss += stip1[i];
  }
  IPAddress setip(ky1, ky2, ky3, ky4);
  Serial.print("IPaddress of SET IP  : ");
  Serial.println(setip);


  //================================================================== IPCONFIG GATEIP
  sss = "";
  coun = 0;  ky1 = 0; ky2 = 0; ky3 = 0; ky4 = 0;
  for ( int i = 0; i <= sizeof(gateway1); i++)
  { if ('.' == gateway1[i])
    { coun += 1;
      i = i + 1;
      if (coun == 1) {
        ky1 = sss.toInt();
      }
      else if (coun == 2) {
        ky2 = sss.toInt();
      }
      else if (coun == 3) {
        ky3 = sss.toInt();
      }
      else if (coun == 4) {
        ky4 = sss.toInt();
      } sss = "";
    } sss += gateway1[i];
  }
  IPAddress gateip(ky1, ky2, ky3, ky4);
  Serial.print("IPaddress of GATE IP  : ");
  Serial.println(gateip);


  //================================================================== IPCONFIG SUBTIP
  sss = "";
  coun = 0;  ky1 = 0; ky2 = 0; ky3 = 0; ky4 = 0;
  for ( int i = 0; i <= sizeof(subnet1); i++)
  { if ('.' == subnet1[i])
    { coun += 1;
      i = i + 1;
      if (coun == 1) {
        ky1 = sss.toInt();
      }
      else if (coun == 2) {
        ky2 = sss.toInt();
      }
      else if (coun == 3) {
        ky3 = sss.toInt();
      }
      else if (coun == 4) {
        ky4 = sss.toInt();
      } sss = "";
    } sss += subnet1[i];
  }
  IPAddress subip(ky1, ky2, ky3, ky4);
  Serial.print("IPaddress of SUBNET IP  : ");
  Serial.println(subip);

  delay(1000);

  //================================================================== IPCONFIG DNS SERVERIP
  sss = "";
  coun = 0; ky1 = 0; ky2 = 0; ky3 = 0; ky4 = 0;
  for ( int i = 0; i <= sizeof(dnsserver1); i++)
  { if ('.' == dnsserver1[i])
    { coun += 1;
      i = i + 1;
      if (coun == 1) {
        ky1 = sss.toInt();
      }
      else if (coun == 2) {
        ky2 = sss.toInt();
      }
      else if (coun == 3) {
        ky3 = sss.toInt();
      }
      else if (coun == 4) {
        ky4 = sss.toInt();
      } sss = "";
    } sss += dnsserver1[i];
  }
  IPAddress dnsip(ky1, ky2, ky3, ky4);
  Serial.print("IPaddress of SETIP  : ");
  Serial.println(dnsip);







  Serial.println("===============================IP CONFIG  IN SETUP  ==========================  ");
  Serial.print("IPaddresses   : ");
  Serial.print(setip);
  Serial.print("\t");
  Serial.print(gateip);
  Serial.print("\t");
  Serial.print(subip);
  Serial.print("\t");
  Serial.print(dnsip);
  Serial.println("\t");

  //========================================================================================================================================
  //========================================================================================================================================




  Serial.print("Connecting to ");
  Serial.println(ssid1);
  WiFi.config(setip, gateip, subip, dnsip);
  WiFi.begin(ssid1, pass1);

  //========================================================

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    digitalWrite(2, 1);
    delay(500);
    digitalWrite(2, 0);
    delay(500);
    if (flag == 1)
    {
      flag = 0;
      ap();
    }
  }

  //=========================================================

  Serial.println("");
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
  Serial.println(F("[CONNECTED]"));
  Serial.print("[IP ");
  Serial.print(WiFi.localIP());
  Serial.println("]");
}

//void receiveEvent(int howMany) {
//while (Wire.available()) { // loop through all but the last
//char c = Wire.read(); // receive byte as a character
//Serial.print(c);         // print the character
//}
//Serial.println();
//}






//=======================================================================================================
//=======================================================================================================
void ap() {
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  myIP = WiFi.softAPIP();
  Serial.print("HotSpt IP:");
  Serial.println(myIP);

  server.begin();
  server.on("/", HandleClient);
  while (1)
    server.handleClient();

  Serial.println("HTTP server started");

  //station();
}

//=======================================================================================================
//=======================================================================================================

void decryptir(String irraw)
{
  //="178,174,77,145,240,97,200,0,0,0,0,16,88";
  int en[13];
  unsigned int ird[211];
  ird[0] = 3350;
  ird[1] = 3350;
  String k1 = "";
  int st = 0, ss = irraw.indexOf(',');
  for (int u = 0; u < 13; u++)
  {
    en[u] = irraw.substring(st, ss).toInt();
    st = ss + 1;
    ss = irraw.indexOf(',', st);
  }
  for (int i = 12; i >= 0; i--)
  {


    for (int j = 0; j < 8; j++)
    {
      if (en[i] > 0)
      {
        int r = en[i] % 2;
        k1 = r + k1;
        en[i] = en[i] / 2;
      }
      else
        k1 = '0' + k1;
    }

  }
  //  Serial.println(k1);
  //  Serial.println(k1.length());
  int y = 2;
  for (int i1 = 0; i1 < 104; i1++)
  {
    if (k1.charAt(i1) == '1')
    {
      ird[y++] = 400;
      ird[y++] = 1300;
    }
    else
    {
      ird[y++] = 400;
      ird[y++] = 450;
    }
  }
  ird[y] = 450;
  for (int op = 0; op <= 210; op++)
  {
    Serial.print(ird[op]);
    Serial.print(",");
  }
  Serial.println();
  //irsend.sendRaw(ird, sizeof(ird) / sizeof(ird[0]), 38);

}

void device_helth()
{
  String cwar = "";
  float rssi = WiFi.RSSI();
  //Serial.println(rssi);
  String  hlt = gettime() + ":" + "";
  hlt += String(int(rssi));
  hlt += ",";




  String top = "";
  sizes = EEPROM.read(1);
  sizep = EEPROM.read(2);
  sizemqtt = EEPROM.read(3);
  sizetopic = EEPROM.read(4);
  sizebit = EEPROM.read(5);
  sizest = EEPROM.read(6);
  sizegate = EEPROM.read(7);
  sizesub = EEPROM.read(8);
  sizedns = EEPROM.read(9);



  for (int i = 0; i < sizetopic - 1; i++)
  {
    top += char( EEPROM.read(10 + i + sizes + sizep +  sizemqtt));
  }
  top += "/DH";
  Serial.print("publish topic = ");
  Serial.println(top);

  for (int i = 0; i < sizebit - 1; i++)
  {
    cwar += char( EEPROM.read(10 + i + sizes + sizep + sizemqtt + sizetopic));
  }
  //Serial.print("CWAR =");
  //Serial.println(cwar);
  hlt += cwar;
  hlt += ";";
  char htopic[topic.length() + 1];
  char mssg3[hlt.length() + 1];
  hlt.toCharArray(mssg3, hlt.length() + 1);
  top.toCharArray(htopic, top.length() + 1);
  Serial.println(mssg3);
  client.publish(htopic, mssg3);
  //  return();
  //goto loop1;
}
