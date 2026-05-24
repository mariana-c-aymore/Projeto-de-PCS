# Projeto-de-PCS

# Monitor de Sinais Vitais - MQTT (Leito Hospitalar)

Este projeto consiste em um sistema de monitoramento de sinais vitais para leitos hospitalares utilizando a tecnologia IoT. Ele realiza a leitura de batimentos cardíacos, oxigenação do sangue (SpO2) e temperatura corporal, enviando esses dados em tempo real para um Broker MQTT.

---

##  Hardware Necessário

Para reproduzir este projeto, você precisará dos seguintes componentes:
* **Microcontrolador:** ESP32 ou ESP8266 (com suporte a Wi-Fi).
* **Sensor de Oxigênio e Batimentos:** MAX30100.
* **Sensor de Temperatura Infravermelho:** MLX90614.
* Cabos de conexão (Jumpers) e Protoboard.

---

##  Bibliotecas Necessárias

Antes de fazer o upload do código, você precisa instalar as seguintes bibliotecas diretamente pelo Gerenciador de Bibliotecas da Arduino IDE (`Ferramentas` > `Gerenciar Bibliotecas...`):

1. **PubSubClient** (por Nick O'Leary) - Para a comunicação MQTT.
2. **MAX30100_PulseOximeter** (ou a biblioteca específica do MAX30100 que você configurou no escopo).
3. **Adafruit MLX90614 Library** - Para a leitura do sensor de temperatura.
4. **Wire** (Já inclusa no core do Arduino) - Para comunicação I2C com os sensores.


