#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

// Konfigurasi jaringan Wi-Fi
const char* ssid = "LAPTOP-VTAGR6NF 3567";
const char* password = "45nY38}9";

// URL server lokal
const char* serverName = "http://192.168.1.100:5000/sensor";

// Pin sensor DHT dan tipe sensor
#define DHTPIN 4      // Ubah sesuai dengan pin yang digunakan
#define DHTTYPE DHT11  // Pilih jenis sensor DHT

// Inisialisasi sensor DHT
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  
  // Menghubungkan ke Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Menghubungkan ke WiFi...");
  }
  Serial.println("Terhubung ke WiFi");

  // Memulai sensor DHT
  dht.begin();
}

void loop() {
  // Membaca nilai kelembapan dari sensor DHT
  float humidity = dht.readHumidity();
  Serial.print("Nilai Kelembapan: ");
  Serial.println(humidity);

  if (isnan(humidity)) {
    Serial.println("Gagal membaca data dari sensor DHT!");
    return;
  }

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Mengatur URL server
    http.begin(serverName);

    // Mengatur header untuk tipe konten
    http.addHeader("Content-Type", "application/json");

    // Membuat payload JSON
    String httpRequestData = "{\"humidity\":" + String(humidity) + "}";
    
    // Mengirimkan data ke server dengan metode POST
    int httpResponseCode = http.POST(httpRequestData);
    
    // Mengecek respons dari server
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error: ");
      Serial.println(httpResponseCode);
    }

    // Menutup koneksi
    http.end();
  } else {
    Serial.println("Tidak terhubung ke WiFi");
  }

  // Menunggu sebelum pengambilan data berikutnya
  delay(60000);  // Mengirim data setiap 60 detik
} 






