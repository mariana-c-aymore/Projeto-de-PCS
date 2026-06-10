#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_MLX90614.h>
#include <ArduinoJson.h>

// Configurações do Wi-Fi
const char* SSID = " ";
const char* SENHA = " ";

// Configurações do MQTT
const char* MQTT_BROKER = "broker.hivemq.com"; 
const int PORTA_MQTT = 1883;
const char* MQTT_TOPIC = "enfermaria/leito_X/sinais_vitais"; 

// Instâncias
Adafruit_MLX90614 termometro;  // Usando Adafruit (compatível com Wokwi)
WiFiClient CLIENTE;
PubSubClient CONEXAO(CLIENTE); 

// Variáveis para simular MAX30100 (já que não tem suporte nativo)
float simulatedHeartRate = 75.0;
float simulatedSpO2 = 98.0;
unsigned long lastSensorRead = 0;

// Variáveis de Controle de Tempo
unsigned long tempoAnterior = 0;
const long intervaloEnvio = 2000; 

// Função para simular leituras do MAX30100
void lerMAX30100Simulado(float &batimentos, float &oxigenacao) {
  unsigned long agora = millis();
  if (agora - lastSensorRead > 500) {
    // Variação natural dos sinais vitais
    simulatedHeartRate += (random(-30, 30) / 100.0);
    simulatedSpO2 += (random(-15, 15) / 100.0);
    
    // Mantém valores em faixas realistas
    if (simulatedHeartRate < 60) simulatedHeartRate = 60;
    if (simulatedHeartRate > 100) simulatedHeartRate = 100;
    if (simulatedSpO2 < 90) simulatedSpO2 = 90;
    if (simulatedSpO2 > 100) simulatedSpO2 = 100;
    
    lastSensorRead = agora;
  }
  
  batimentos = simulatedHeartRate;
  oxigenacao = simulatedSpO2;
}

// Função para Conectar ao Wi-Fi
void CONECTE_AO_WIFI() {
  delay(10);
  Serial.println();
  Serial.print("Conectando-se à rede: ");
  Serial.println(SSID);

  WiFi.begin(SSID, SENHA);

  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED && tentativas < 20) {
    delay(500);
    Serial.print(".");
    tentativas++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWi-Fi conectado!");
    Serial.print("Endereço IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFalha ao conectar ao Wi-Fi!");
  }
}

// Função para Conectar ao MQTT
void CONECTE_AO_MQTT() {
  int tentativas = 0;
  while (!CONEXAO.connected() && tentativas < 5) { 
    if (WiFi.status() != WL_CONNECTED) {
      CONECTE_AO_WIFI();
    }

    Serial.print("Tentando conectar ao Broker MQTT...");
    String CLIENTE_ID = "LEITO-01-" + String(random(0, 0xffff), HEX); 
    
    if (CONEXAO.connect(CLIENTE_ID.c_str())) {
      Serial.println(" Sucesso! Conectado ao Broker.");
    } else {
      Serial.print(" Falhou. Código de erro: ");
      Serial.print(CONEXAO.state()); 
      Serial.println(" Tentando novamente em 5 segundos...");
      tentativas++;
      delay(5000);
    }
  }
}

void setup() {
  Wire.begin(21, 22); // SDA=21, SCL=22
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("=================================");
  Serial.println("Sistema de Monitoramento - Enfermaria");
  Serial.println("=================================");
  
  CONECTE_AO_WIFI();
  CONEXAO.setServer(MQTT_BROKER, PORTA_MQTT);
  
  Serial.println("\nInicializando os sensores...");
  
  // Inicializa MLX90614
  if (!termometro.begin()) {
    Serial.println("AVISO: Sensor MLX90614 não encontrado! Usando simulação.");
  } else {
    Serial.println("Sensor MLX90614 OK");
  }
  
  Serial.println("Sensor MAX30100: Modo simulação ativado");
  Serial.println("\nSistema pronto!\n");
}

void loop() {
  if (!CONEXAO.connected()) {
    CONECTE_AO_MQTT();
  }
  CONEXAO.loop(); 

  unsigned long tempoAtual = millis();
  if (tempoAtual - tempoAnterior >= intervaloEnvio) {
    tempoAnterior = tempoAtual;

    // Leitura do MLX90614 (real)
    float temperatura = 25.0;
    if (termometro.begin()) {
      temperatura = termometro.readObjectTempC();
    } else {
      // Fallback para simulação
      temperatura = 36.5 + random(-10, 20) / 10.0;
    }
    
    // Leitura do MAX30100 (simulada)
    float batimentos = 0;
    float oxigenacao = 0;
    lerMAX30100Simulado(batimentos, oxigenacao);
    
    // Cria JSON
    StaticJsonDocument<128> doc; 
    
    doc["bpm"] = round(batimentos);
    doc["spo2"] = round(oxigenacao);
    doc["temp"] = String(temperatura, 1).toFloat();

    String payload;
    serializeJson(doc, payload); 

    // Debug
    Serial.println("---------------------------------");
    Serial.print("Temperatura: ");
    Serial.print(temperatura, 1);
    Serial.println(" °C");
    Serial.print("Batimentos: ");
    Serial.print(round(batimentos));
    Serial.println(" BPM");
    Serial.print("SpO2: ");
    Serial.print(round(oxigenacao));
    Serial.println(" %");
    Serial.print("JSON: ");
    Serial.println(payload);

    if (CONEXAO.publish(MQTT_TOPIC, payload.c_str())) {
      Serial.println("✅ Dados publicados com sucesso!");
    } else {
      Serial.println("❌ Erro ao publicar no MQTT.");
    }
    Serial.println("---------------------------------\n");
  }
}