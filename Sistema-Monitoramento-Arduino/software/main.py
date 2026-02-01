import serial # Importa a biblioteca serial para comunicação com o Arduino
import time # Importa time para manipulação de tempo
import csv # Importa csv para manipulação de arquivos CSV
from datetime import datetime # Importa datetime para manipulação de datas e horas
import matplotlib.pyplot as plt # Importa matplotlib para plotagem de gráficos
from matplotlib.animation import FuncAnimation # Importa FuncAnimation para animação do gráfico


arduino = serial.Serial(port='COM3', baudrate=9600) # Configura a porta serial e a taxa de transmissão
time.sleep(2) 


x_data = []
y_temp = []

plt.style.use('ggplot') # Define o estilo do gráfico
fig, ax = plt.subplots() # Cria a figura e os eixos do gráfico
line, = ax.plot([],[],label='Temperatura (°C)', color= 'red') # Inicializa a linha do gráfico
ax.set_xlim(10, 50) # Define os limites do eixo x
ax.set_ylim(0, 50) # Define os limites do eixo y
ax.legend() # Adiciona a legenda ao gráfico
plt.title("Monitoramento de Temperatura em Tempo Real") # Define o título do gráfico

def update_grafico(frame): # Função para atualizar o gráfico em tempo real
    if arduino.in_waiting > 0: # Verifica se há dados disponíveis na porta serial
        dados = arduino.readline().decode('utf-8').strip() # Lê os dados da porta serial
        try:
            temp, hum, luz = dados.split(',') # Divide os dados em temperatura, umidade e luz
            temp_float = float(temp) # Converte a temperatura para float
            agora = datetime.now().strftime('%H:%M:%S') # Obtém o horário atual
            
            if temp_float > 20: # Verifica se a temperatura está acima de 20°C
                status = "Alta" # Define o status como alta
                arduino.write(b'Q')  # Envia 'Q' para o Arduino para mostrar temperatura alta 
            else:
                status = "Normal" # Define o status como normal
                arduino.write(b'F')  # Envia 'F' para o Arduino para mostrar temperatura normal
            
            print(f"[{agora}] Status da temperatura: {status} | Temperatura: {temp}°C | Humidade: {hum}% | Luz: {luz}   ")
            
            with open("historico_clima.csv", "a", newline ='') as arquivo_csv: # Abre o arquivo CSV para escrita
                escritor_csv = csv.writer(arquivo_csv) # Cria o escritor CSV
                escritor_csv.writerow([agora, temp, hum, luz, status]) # Escreve os dados no arquivo CSV
            
            y_temp.append(temp_float) # Adiciona a temperatura à lista y_temp
            x_data.append(len(x_data)) # Adiciona o índice à lista x_data
            
             # Atualiza os dados do gráfico
            
            line.set_data(x_data[-50:], y_temp[-50:]) # Atualiza a linha do gráfico com os últimos 50 pontos
            ax.set_xlim(max(0, len(y_temp)-50), len(y_temp)+5) # Ajusta o limite do eixo x para mostrar os últimos 50 pontos
        except ValueError: 
            pass
    return line,
        
ani = FuncAnimation(fig, update_grafico, interval=1000, cache_frame_data=False) # Cria a animação do gráfico
plt.show() # Exibe o gráfico

arduino.close() # Fecha a conexão serial ao finalizar o programa