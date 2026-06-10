# 📟 Firmware e Integração dos Sensores

Esta branch contém o código-fonte responsável por inicializar, calibrar e ler os dados brutos dos sensores de sinais vitais (**MAX30100** e **MLX90614**), além de estruturar esses dados antes do envio.

---

## 🗺️ Pinagem e Conexões (I2C Bus)

Ambos os sensores utilizados neste projeto comunicam-se via protocolo **I2C**. Como o ESP32/ESP8266 possui pinos de barramento I2C padrão, eles foram conectados em paralelo nas mesmas linhas de dados (`SDA`) e clock (`SCL`).

### Tabela de Conexões (Exemplo para ESP32)

| Componente | Pino no Sensor | Pino no ESP32 | Descrição |
| :--- | :--- | :--- | :--- |
| **MAX30100** | VIN | 3.3V | Alimentação |
| | GND | GND | Aterramento |
| | SDA | GPIO 21 | Linha de Dados I2C |
| | SCL | GPIO 22 | Linha de Clock I2C |
| **MLX90614** | VIN | 3.3V / 5V | Alimentação |
| | GND | GND | Aterramento |
| | SDA | GPIO 21 | Linha de Dados I2C (Compartilhado) |
| | SCL | GPIO 22 | Linha de Clock I2C (Compartilhado) |

> ⚠️ **Nota Importante:** Verifique se o seu módulo MAX30100 necessita de resistores de *pull-up* nas linhas SDA/SCL para operar corretamente em 3.3V (alguns módulos comerciais possuem resistores de 4.7kΩ vinculados incorretamente a 5V).

---

## 🛠️ Funcionamento do Código nesta Branch

O firmware presente nesta branch executa as seguintes etapas:

1. **Inicialização (`setup`):** * Inicializa a comunicação Serial para depuração.
   * Inicia o barramento I2C (`Wire.begin()`).
   * Configura o MAX30100 com a corrente de LED apropriada e ativa o filtro de batimentos.
   * Inicializa o MLX90614.
2. **Loop de Leitura (`loop`):**
   * Atualiza o objeto do oxímetro (`pox.update()`).
   * Realiza leituras assíncronas de Temperatura Corporal (Objeto) e Temperatura Ambiente utilizando intervalos baseados em `millis()` para não bloquear o código.
   * Exibe os resultados no Monitor Serial para validação local.

---

## 🚀 Como Testar Localmente

Para garantir que a leitura dos sensores está funcionando perfeitamente antes de subir os dados via MQTT:

1. Faça o checkout para esta branch:
   ```bash
   git checkout nome-desta-branch
