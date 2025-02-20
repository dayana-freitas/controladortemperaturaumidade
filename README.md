# Controlador de Temperatura e Umidade com Arduino UNO

<a href="https://youtu.be/TjvKqfbe9nc">Apresentação do projeto no YouTube</a>

Projeto final desenvolvido para a disciplina de Sistemas Digitais, ministrada pelo professor Delvanei Gomes, no ano de 2024, na UTFPR.

<h3>Equipe:</h3>

- Cauã Felipe Gomes de Azevedo
- Dayana Resende de Freitas
- Elyas Emanuel Potulski Soboleski

O projeto foi desenvolvido com base no controlador de temperatura e umidade N323-RHT, da marca Novus, que possui três saídas de controle e pode ser configurado manualmente. Utilizando esse controlador como referência, criamos o protótipo que será apresentado, o qual possui funções para definir a temperatura e umidade mínima e máxima de acordo com as necessidades do usuário. As saídas do sistema serão utilizadas para ativar a irrigação, o aquecimento e a refrigeração da estufa conforme os parâmetros cadastrados.

<h3>Materiais utilizados:</h3>

- 1 Arduino UNO
- 3 Displays de 7 segmentos cátodo comum
- 3 Registradores de deslocamento 74HC595
- 1 Demultiplexador 74LS139
- 1 Contador 74LS393
- 1 Porta Inversora74LS04
- 1 Porta AND 74LS08
- 3 LEDs
- 3 Botões
- 2 Protoboards
- Resistores
- Jumpers

<h3>Instruções:</h3>

No arquivo <b>projeto.c</b> está a versão final apresentada do projeto, dentro dele o código está comentado afim de facilitar o entendimento da conexão de portas.

Na pasta imagens, é possível encontrar imagens da montagem do projeto.

Perceba que o arquivo <b>simulacao_tinkercad.c</b> é um pouco diferente do projeto final pois não possui a função demultiplexador.

A simulação pode ser encontrada em: <a href="https://www.tinkercad.com/things/lmZnEpiFMLK-controlador-de-temperatura-e-umidade?sharecode=qH96mGcl8HffHrVjqjyqkoQV5-Z8oD8nZZ1EQgmY5Hc">Simulação no Tinkercad</a>

<img width="100%" src="https://github.com/dayana-freitas/controladortemperaturaumidade/blob/main/imagens/capa.jpg" alt="Especificações do projeto">
