#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <MAX30100.h>
#include <VEGA_MLX90614.h>
#include <ArduinoJson.h>

// Configurações do Wi-Fi
const char* SSID = "NOME_DA_SUA_REDE_WIFI";
const char* SENHA = "SENHA_DO_SEU_WIFI";

// Configurações do MQTT
const char* MQTT_BROKER = "broker.hivemq.com"; 
const int PORTA_MQTT = 1883;
const char* MQTT_TOPIC = "enfermaria/leito_X/sinais_vitais"; 

// Instâncias
MAX30100 oximetro;
VEGA_MLX90614 termometro;
WiFiClient CLIENTE;           
PubSubClient CONEXAO(CLIENTE); 

// Variáveis de Controle de Tempo
unsigned long tempoAnterior = 0;
const long intervaloEnvio = 2000; 

// Função para Conectar ao Wi-Fi
void CONECTE_AO_WIFI() {
  delay(10);
  Serial.println();
  Serial.print("Conectando-se à rede: ");
  Serial.println(SSID);

  WiFi.begin(SSID, SENHA);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi conectado!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

// Função para Conectar ao MQTT
void CONECTE_AO_MQTT() {
  while (!CONEXAO.connected()) { 
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
      delay(5000);
    }
  }
}

void setup() {
  Wire.begin(21, 22); //SDA=21, SCL=22)
  Serial.begin(115200);
  
  CONECTE_AO_WIFI();
  CONEXAO.setServer(MQTT_BROKER, PORTA_MQTT);
  
  Serial.println("Inicializando os sensores...");
  termometro.begin();
  
  if (!oximetro.begin()) {
    Serial.println("Erro: Sensor MAX30100 não foi encontrado!");
    while(1); 
  }
  oximetro.setMode(MAX30100_MODE_SPO2_HR);
  oximetro.setLedsCurrent(MAX30100_LED_CURR_24MA, MAX30100_LED_CURR_24MA);
}

void loop() {
  if (!CONEXAO.connected()) {
    CONECTE_AO_MQTT();
  }
  CONEXAO.loop(); 
  oximetro.update();

  unsigned long tempoAtual = millis();
  if (tempoAtual - tempoAnterior >= intervaloEnvio) {
    tempoAnterior = tempoAtual;

    float batimentos = oximetro.getHeartRate();
    float oxigenacao = oximetro.getSpO2();
    float temperatura = termometro.readObjectTempC(); 
    
    StaticJsonDocument<128> doc; 
    
    doc["bpm"] = round(batimentos);
    doc["spo2"] = oxigenacao;
    doc["temp"] = String(temperatura, 1).toFloat();

    String payload;
    serializeJson(doc, payload); 

    Serial.print("Payload JSON Gerado: "); 
    Serial.println(payload);

    if (CONEXAO.publish(MQTT_TOPIC, payload.c_str())) {
      Serial.println("Dados publicados com sucesso!");
    } else {
      Serial.println("Erro ao publicar no MQTT.");
    }
  }
}
