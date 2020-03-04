#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define AP_SSID "esp_8266-id" //这里改成你的AP名字
#define AP_PSW "12345678"     //这里改成你的AP密码 8位以上

//以下三个定义为调试定义
#define DebugBegin(baud_rate) Serial.begin(baud_rate)
#define DebugPrintln(message) Serial.println(message)
#define DebugPrint(message) Serial.print(message)

const char *username = "admin";
const char *password = "esp8266";

IPAddress local_IP(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

ESP8266WebServer server(80);//创建webserver

//ap初始化
void initAP()
{
  DebugPrintln("start soft-ap config");

  //配置AP信息
  WiFi.mode(WIFI_AP);

  DebugPrintln(WiFi.softAPConfig(local_IP, gateway, subnet) ? "successful" : "Failed!");

  //启动AP模式，并设置账号和密码
  DebugPrint("Setting soft-AP SS_ID and Password ");
  bool result = WiFi.softAP(AP_SSID, AP_PSW);

  if (result)
  {
    DebugPrintln("start successful");
    //输出 soft-ap ip地址
    DebugPrintln(String("Soft-AP IP address = ") + WiFi.softAPIP().toString());
    //输出 soft-ap mac地址
    DebugPrintln(String("MAC address = ") + WiFi.softAPmacAddress().c_str());
  }
  else
  {
    DebugPrintln("start Failed!");
  }
}

//初始化服务器
void initWebServer(){
  //以下配置uri对应的handler
  server.on("/", []() {
    //校验帐号和密码
    if (!server.authenticate(username, password)) {
      return server.requestAuthentication();
    }
    server.send(200, "text/plain", "Login OK");
  });

  server.begin();
}

void setup()
{
  DebugBegin(9600);
  initAP();
  initWebServer();
}

void loop()
{
  server.handleClient();
}