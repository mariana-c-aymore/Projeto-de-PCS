const char* MQTT = "broker.hivemq.com"; //const é o define da linguagem c
const int PORTA_MQTT = 1883;
const char* mqtt_topic = "enfermaria/leitoX/sinais_vitais"


WiFiClient CLIENTE_IP;           // conecta-se ao IP. WifiClient é um tipo de variável, assim como int é
PubSubClient CLIENTE(CLIENTE); // Passa o CLIENTE de rede para a biblioteca MQTT


void CONECTE_AO_MQTT() {
 while (!CLIENTE.connected()) { //enquanto o cliente NÃO estiver conectado...
   Serial.print("Tentando conectar ao Broker MQTT...");


   String CLIENTE_ID = "LEITO-01" + String(random(0, 0xffff), HEX); //Cria ID único e aleatório para o LEITO 01, converte para a base hexadecimal (HEX) e depois para texto (STRING)
  
   if (CLIENTE.connect(CLIENTE_ID.c_str())) {
     Serial.println(" Sucesso! Conectado ao Broker.");
   } else {
     // Se falhar, mostra o código do erro e espera 5 segundos antes de tentar de novo
     Serial.print(" Falhou. Código de erro: ");
     Serial.print(CLIENTE.state());
     Serial.println(" Tentando novamente em 5 segundos...");
     delay(5000);
   }
 }
}


void setup() 
  Wire.begin();
  Serial.begin(115200);
   CLIENTE.setServer(MQTT, PORTA_MQTT);
  
  Serial.println("Inicializando o sensor...");
  Serial.println("Inicializando o sensor de temperatura MLX90614...");
  
  // Inicializa o sensor físico
  termometro.begin();
  
  // Inicializa o sensor já configurado para calcular batimentos e oxigênio
  if (!oximetro.begin()) {
    Serial.println("Erro: Sensor MAX30100 não foi encontrado!");
    while(1); // Trava aqui se houver erro de hardware
  }
}


void loop() {
  // Atualiza as leituras internas do sensor (essencial rodar sempre)
  oximetro.update();


  // Puxa os valores já calculados pelas funções prontas!
  float batimentos = oximetro.getHeartRate();
  float oxigenacao = oximetro.getSpO2();


  // Mostra os valores finais e limpos na tela
  Serial.print("BPM: ");
  Serial.print(batimentos);
  Serial.print(" | SpO2: ");
  Serial.print(oxigenacao);
  Serial.println("%");
}
