// welove node ini akan dimasukkan kedalam esp01 (untuk kirin dan terima data antara firebase)
// (dan krim terima data antara esp01 dan mega)
// This example shows how to increase the value at document field. This operation required Email/password, custom or OAUth2.0 authentication.
// include lib yang menurut saya tidak penting
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
// definisi wifi pada esp01
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>
#define WIFI_SSID "Smart Biogas"
#define WIFI_PASSWORD "Smartbiogas01"
/* 2. Define the API Key */
#define API_KEY "AIzaSyD-zBPeBJjG4wHr7YVYbEqHcfFwYlIMf6U"
/* 3. Define the project ID */
#define FIREBASE_PROJECT_ID "welove-project"
/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "alpha@gmail.com"
#define USER_PASSWORD "yusuf1112"
// Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
// untuk membaca json data (data utama)
FirebaseJsonData sensorReady;
FirebaseJsonData isReady;
FirebaseJsonData emailUser;
// handle warna rgb
FirebaseJsonData warnaR;
FirebaseJsonData warnaG;
FirebaseJsonData warnaB;
// Declare FirebaseJson object (global or local)
FirebaseJson json;
FirebaseJson updateSampahTelahMasuk;
// data untuk mengonto dan mengirim ke firebase firestore
unsigned long dataMillis = 0;
// int count = 0;
// buat variabel bool untuk parsing data.
bool parsing = false;
String sData, data[10];
// inisialisasi nilai masuk sebagai int, tapi kirim ke firebase dalam bentuk bool
int isMasuk = 0;

void setup()
{
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
#if defined(ESP8266)
  fbdo.setBSSLBufferSize(1024 /* Rx buffer size in bytes from 512 - 16384 */, 4096 /* Tx buffer size in bytes from 512 - 16384 */);
#endif
  fbdo.setResponseSize(2048);
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop()
{
  // program untuk mendapatkan nilai sampah telah masuk dari arduino uno
  // dan mengirimkan datanya ke firestore
  while (Serial.available())
  {
    // buat variabel nilaiinput, dan masukkan nilai serial.readString kesana
    // String nilaiInput = Serial.readString();
    // print/tampilkan nilai input tadi di serial monitor
    char inChar = Serial.read();
    sData += inChar;
    if (inChar == '$')
      parsing = true;
    if (parsing)
    {
      int q = 0;
      for (int i = 0; i < sData.length(); i++)
      {
        if (sData[i] == '#')
        {
          q++;
          data[q] = "";
        }
        else
          data[q] += sData[i];
      }
      // setelah semua data didapatkan, kita akan print datanya ke serial satu persatu
      isMasuk = data[1].toInt();
      parsing = false;
      sData = "";
    }
  }

  // Firebase.ready() should be called repeatedly to handle authentication tasks.
  // program dibawah untuk mendadpatkan data dari firebase dan mengirimkannya ke arduino uno
  if (Firebase.ready() && (millis() - dataMillis > 1000 || dataMillis == 0))
  {
    dataMillis = millis();
    String documentPath = "tempat_sampah/4a34a5b3e6f797a3a930e7e5a98ef0cb";
    // String documentPath2 = "aa/bb";

    if (Firebase.Firestore.getDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), ""))
    {
      String dataSaya = fbdo.payload().c_str();
      json.setJsonData(dataSaya);
      json.get(isReady, "fields/isReady/booleanValue");
      json.get(sensorReady, "fields/sensorReady/booleanValue");
      json.get(emailUser, "fields/emailUser/stringValue");
      json.get(warnaR, "fields/warnaR/integerValue");
      json.get(warnaG, "fields/warnaG/integerValue");
      json.get(warnaB, "fields/warnaB/integerValue");
      Serial.print("#");
      Serial.print(isReady.to<bool>());
      Serial.print("#");
      Serial.print(sensorReady.to<bool>());
      Serial.print("#");
      Serial.print(emailUser.to<String>());
      Serial.print("#");
      Serial.print(warnaR.to<int>());
      Serial.print("#");
      Serial.print(warnaG.to<int>());
      Serial.print("#");
      Serial.print(warnaB.to<int>());
      Serial.println("#$");
    }
    else
      Serial.println("#######$");

    // program untuk mengirim data yang dibaca dari arduino uno ke firebase lewat esp01 semoga saja program ini berhasil.
    updateSampahTelahMasuk.clear();
    if (isMasuk == 1)
      updateSampahTelahMasuk.set("fields/isMasuk/booleanValue", true);
    else
      updateSampahTelahMasuk.set("fields/isMasuk/booleanValue", false);

    if (Firebase.Firestore.patchDocument(&fbdo, FIREBASE_PROJECT_ID, "" /* databaseId can be (default) or empty */, documentPath.c_str(), updateSampahTelahMasuk.raw(), "isMasuk" /* updateMask */))
      Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
    else
      Serial.println(fbdo.errorReason());
  }
}
