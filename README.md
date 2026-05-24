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

###  Bibliotecas Necessárias

Para compilar este projeto, você precisará instalar as seguintes bibliotecas diretamente pelo **Gerenciador de Bibliotecas** da Arduino IDE (`Ferramentas` > `Gerenciar Bibliotecas...` ou `Ctrl + Shift + I`):

1. **PubSubClient.h** (por *Nick O'Leary*)
   * **Para que serve:** Gerencia a conexão e o envio de mensagens via protocolo MQTT.
   * **Como buscar:** Digite `PubSubClient` na barra de pesquisa.

2. **MAX30100.h** (por *Oxullo Intersecans*)
   * **Para que serve:** Controla o sensor MAX30100 para leitura de batimentos cardíacos e oxigenação (SpO2).
   * **Como buscar:** Digite `MAX30100 PulseOximeter` e instale a versão do autor Oxullo.

3. **VEGA_MLX90614.h** (por *Adafruit*)
   * **Para que serve:** Realiza a leitura do sensor de temperatura infravermelho MLX90614.
   * **Como buscar:** Digite `MLX90614` na barra de pesquisa.

####  Bibliotecas Nativas (Já inclusas na IDE)
As bibliotecas abaixo não precisam ser instaladas manualmente, pois já vêm integradas ao selecionar a sua placa:
* **`Wire.h`** (Para comunicação I2C dos sensores).
* **`WiFi.h`** (Se você estiver utilizando ESP32) ou **`ESP8266WiFi.h`** (Se estiver utilizando NodeMCU/ESP8266).

