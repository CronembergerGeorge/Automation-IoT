# 🌡️ Sistema de Monitoramento de Temperatura e Controle Ambiental

Sistema de monitoramento em tempo real de temperatura, umidade e luminosidade usando Arduino, com interface gráfica em Python e controle automatizado de ventilação.

## 📋 Descrição do Projeto

Este projeto implementa um sistema completo de monitoramento ambiental que:
- Coleta dados de temperatura, umidade e luminosidade através de sensores
- Calcula média móvel de temperatura para maior precisão
- Envia dados via comunicação serial para processamento em Python
- Exibe gráficos em tempo real da temperatura
- Registra histórico de dados em arquivo CSV
- Controla automaticamente LEDs e servo motor baseado em limites de temperatura

## 🔧 Componentes de Hardware

- **Arduino** (qualquer modelo compatível)
- **Sensor DHT11** - Temperatura e Umidade
- **Sensor LDR** (ou fotoresistor) - Luminosidade
- **Servo Motor SG90** - Controle de ventilação/exaustão
- **2 LEDs** (Verde e Vermelho) - Indicadores visuais
- **Resistores** apropriados
- **Protoboard e jumpers**

## 📌 Esquema de Conexões

| Componente | Pino Arduino |
|------------|--------------|
| DHT11 (Dados) | Digital 2 |
| LED Verde | Digital 8 |
| LED Vermelho | Digital 9 |
| Servo Motor | Digital 31 |
| Sensor LDR | Analógico A0 |

## 💻 Requisitos de Software

### Python
```bash
pip install pyserial matplotlib
```

### Arduino IDE
Bibliotecas necessárias:
- DHT sensor library (by Adafruit)
- Servo library (incluída no Arduino IDE)

## 📊 Funcionalidades

### Monitoramento
- **Média Móvel**: Calcula média das últimas 10 leituras de temperatura para maior estabilidade
- **Leitura contínua**: Coleta dados a cada 500ms
- **Protocolo CSV**: Dados enviados no formato `temperatura,umidade,luz`

### Controle Automatizado
| Condição | Ação |
|----------|------|
| Temperatura > 20°C | LED vermelho LIGADO, Servo em 180°, Status "Alta" |
| Temperatura ≤ 20°C | LED verde LIGADO, Servo em 0°, Status "Normal" |

### Registro de Dados
- Arquivo gerado: `historico_clima.csv`
- Formato: `Horário, Temperatura, Umidade, Luz, Status`
- Atualização em tempo real

### Visualização
- Gráfico animado mostrando temperatura em tempo real
- Exibe últimos 50 pontos de medição
- Atualização automática a cada segundo

## 📁 Estrutura do Projeto

```
projeto-monitoramento/
│
├── docs/                        # Documentação
│   ├── imagens/                 # Imagens do projeto (circuito, esquemas, etc.)
│   └── short/                   # Resumos e versões curtas da documentação
│
├── firmware/                    # Código do Arduino
│   └── monitoramento_arduino.ino
│
├── software/                    # Software Python
│   └── monitoramento_python.py
│
├── historico_clima.csv          # Arquivo de dados (gerado automaticamente)
└── README.md                    # Este arquivo
```

## 🔍 Protocolo de Comunicação

### Arduino → Python
```
temperatura,umidade,luz
Exemplo: 25.5,60.2,512
```

### Python → Arduino
- `Q` - Ativa modo "temperatura alta" (LED vermelho, servo 180°)
- `F` - Ativa modo "temperatura normal" (LED verde, servo 0°)

## 📝 Notas Técnicas

- A média móvel reduz ruído nas leituras do sensor DHT11
- O delay de 500ms entre leituras respeita as especificações do DHT11
- O buffer circular no array de temperaturas otimiza o uso de memória
- O gráfico mantém apenas os últimos 50 pontos para melhor visualização
