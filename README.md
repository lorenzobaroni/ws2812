# Tarefa: Exibição de Números e Controle de LED RGB com Raspberry Pi Pico W

Este projeto tem como objetivo controlar uma matriz de LEDs WS2812 e um LED RGB utilizando o microcontrolador Raspberry Pi Pico W. O programa implementa a exibição de números na matriz de LEDs e o controle de um LED RGB que pisca continuamente. Além disso, dois botões permitem a navegação pelos números exibidos.

---

## Link do Vídeo Ensaio

[Demonstração de Funcionamento do Projeto](https://www.youtube.com/watch?v=fFORNoJMAN4&ab_channel=LorenzoGiuseppeOliveiraBaroni)

---

## Funcionalidades do Projeto

1. **Exibição de Números na Matriz de LEDs**:
   - Números de 0 a 9 são exibidos na matriz de LEDs WS2812.
   - Os números são representados por padrões definidos na memória.

2. **Controle de Botões**:
   - **Botão A**: Incrementa o número exibido na matriz de LEDs.
   - **Botão B**: Decrementa o número exibido na matriz de LEDs.

3. **LED RGB**:
   - Um LED RGB pisca continuamente no pino definido, alternando entre ligado e desligado.

---

## Componentes Necessários

- 1x Raspberry Pi Pico
- 1x Matriz de LEDs WS2812
- 1x LED RGB (pinos R, G e B conectados aos pinos definidos no código)
- 2x Botões (para controle de incremento e decremento)
- Resistores, jumpers e outros itens de conexão

---

## Esquema de Conexão

### Pinos Configurados:
- **Pino 7**: Dados para a matriz de LEDs WS2812.
- **Pino 11**: Componente verde do LED RGB.
- **Pino 12**: Componente azul do LED RGB.
- **Pino 13**: Componente vermelho do LED RGB.
- **Pino 5**: Botão A.
- **Pino 6**: Botão B.

---

## Como Executar o Código

1. Clone este repositório para sua máquina:
   ```bash
   git clone https://github.com/lorenzobaroni/ws2812
   ```
2. **Abra o projeto no VS Code.**

3. **Compile o código**

4. **Clique no arquivo diagram.json e teste o projeto no simulador do WOKWI Integrado no VS Code**

5. **Carregue o código no Raspberry Pi Pico.**

6. **Conecte os componentes (botões, matriz de LEDs e led rgb) conforme descrito no código.**

7. **Pressione os botões para alternar os numeros na matriz de led**

---

## Estrutura do Projeto 

```
.
├── CMakeLists.txt
├── ws2812.pio.h          # Biblioteca para controle de LEDs WS2812
├── ws2812.pio            # Código PIO utilizado no projeto
├── main.c                # Código principal do projeto
├── README.md             # Arquivo de documentação
```

---

## Licença

Este programa foi desenvolvido como um exemplo educacional e pode ser usado livremente para fins de estudo e aprendizado.

---

## Autor

- LORENZO GIUSEPPE OLIVEIRA BARONI

