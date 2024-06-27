/* 
	============================ CONTROLADOR DE TEMPERATURA E UMIDADE =============================

    LINK PARA SIMULAÇÃO NO TINKERCAD:
    https://www.tinkercad.com/things/lmZnEpiFMLK-controlador-de-temperatura-e-umidade?sharecode=qH96mGcl8HffHrVjqjyqkoQV5-Z8oD8nZZ1EQgmY5Hc
	
	Programação desenvolvida para uso de displays catodo comum em conjunto com resistradores de deslocamento.

	Resumo do Funcionamento:
	Umidade:
		Ativa a irrigação se a umidade for menor ou igual ao mínimo.
		Desativa a irrigação se a umidade for maior ou igual a 75% do intervalo entre mínimo e máximo.
	Temperatura:
		Ativa o resfriamento se a temperatura for maior ou igual ao máximo.
		Ativa o aquecimento se a temperatura for menor ou igual ao mínimo.

	Funções Principais:
		contador: Lê o estado dos botões e retorna um valor correspondente para indicar qual função está sendo selecionada (configuração de mínimos e máximos de temperatura/umidade ou retorno ao modo de exibição).

		incrementoDecremento: Controla o btn_incremento ou btn_decremento dos valores de temperatura e umidade conforme selecionado pelo usuário nos botões.

		atualizarDisplay: responsável por atualizar um display de 7 segmentos utilizando um registrador de deslocamento, controlado pelos pinos storePin, shiftPin e dataPin. 

		controlarSaidas: Decide se ativa ou desativa as saídas de controle (irrigação, resfriamento, aquecimento) baseado nos valores lidos dos sensores e nos limites configurados.
		
		shiftRegister = Função para ampliar o número de portas digitais no arduíno através do registrador de deslocamento. Cada CI 74595 está conectado a um display de 7 segmentos.

	Conexão do CI:
		O CI possuí 8 saídas comuns e uma negada sendo conectado ao display da seguinte forma:
		8 -> A | 7 -> B | 6 -> C | 5 -> D |4 -> E | 3 -> F | 2 -> G
		-	A saída 1 pode vir a ser conectada ao ponto e a saída negana não está sendo utilizada.
		-	A interpretação da função segue que a saída 8 é LSB e a saída 1 é MSB 

	Números em decimal inseridos na função shiftRegister para apresentar algarismos no display:
		0 = 0011 1111 = 63   
		1 = 0000 0110 = 6    
		2 = 0101 1011 = 91   
		3 = 0100 1111 = 79
		4 = 0110 0110 = 102
		5 = 0110 1101 = 109
		6 = 0111 1101 = 125
		7 = 0000 0111 = 7
		8 = 0111 1111 = 127
		9 = 0110 1111 = 111

	Conversão de valores para aparecer nos displays:
		dez_temp = temp/10;
		uni_temp = temp%10;
		Através da divisão e do resto da divisão o valor da temperatura/umidade é dividido em unidade e dezena, sendo que a unidade vai para o display 3 e a dezena para o display 2.
	
	Ativação dos botões para controle:
		Funções:
			0 - Mostrador (configurar para voltar ao mostrador)
			1 - temp_min
			2 - temp_max
			3 - umid_min
			4 - umid_max
		Funcionamento:
		Clicar e segurar no botão de função por 4 seg
		Vai aparecer o número da função no primeiro display
		Botão 2 incrementa o valor e salva na variável
		Botão 3 decrementa o valor e salva na variável

	A programação funciona em dois loops sendo que no primeiro enquanto nada estiver sendo programado pelo usuário o display ficará em loop alternando entre o valor da umidade representada por um "u"(28) no display 1 e a temperatura representada por um "t"(120) no display 1. Após o usuário precionar o botão de função por 4 seg, entra no segundo loop onde será possível definir valores de controle.
*/

#define heating         0 
#define cooling         1 
#define data1           2
#define shift1          3
#define store1          4
#define data2           5 
#define shift2          6  
#define store2          7  
#define data3           8
#define shift3          9  
#define store3          10 
#define irrigation      11 // Irrigação
#define btn_incremento  12 // Botões
#define btn_decremento  13
#define contador_saida2 16 // Contador
#define contador_saida1 17
#define contador_saida0 18
#define btn_funcao      19 // Botão de função
#define sensor_temp     A1 // Sensores
#define sensor_umid     A0 

int temp = 0; //Variáveis
int umid = 0;
int cont = 0;
int controle = 0;
int temp_min = 13;
int temp_max = 26;
int umid_min = 60;
int umid_max = 80;

int contador(int inputA3, int inputA4, int inputA5); // Protótipo das funções
int incrementoDecremento(int x, int variavel);
void atualizarDisplay(int storePin, int shiftPin, int dataPin, int dado);
void controlarSaidas(int umid, int temp);
void shiftRegister(int storePin, int dataPin, byte value); 

void setup()
{
	pinMode(store1, OUTPUT); // Registrador 1
	pinMode(data1,  OUTPUT);
	pinMode(shift1, OUTPUT);

	pinMode(store2, OUTPUT); // Registrador 2
	pinMode(data2,  OUTPUT);
	pinMode(shift2, OUTPUT);

	pinMode(store3, OUTPUT); // Registrador 3
	pinMode(data3,  OUTPUT);
	pinMode(shift3, OUTPUT);

	pinMode(irrigation,   OUTPUT); // Saída irrigação
	pinMode(heating,  OUTPUT); // Saídas via DMUX 
	pinMode(cooling, OUTPUT);

	pinMode(sensor_temp, INPUT); // Sensores
	pinMode(sensor_umid, INPUT); 
	
	pinMode(contador_saida2, INPUT); // Entradas de dados do contador
	pinMode(contador_saida1, INPUT);
	pinMode(contador_saida0, INPUT);

	pinMode(btn_funcao,     INPUT); // Botões de controle
	pinMode(btn_incremento, INPUT);
	pinMode(btn_decremento, INPUT);
}

void loop()
{
	do
	{	//Loop executa até que o usuário pressione o botão de função		
		temp = map(((analogRead(sensor_temp) - 20) *3.04), 0, 1023, -40, 125); // Mapeia os dados do sensor de temperatura para valores entre -40°C e 125°C
		umid = map(analogRead(sensor_umid), 0, 876, 0, 100); // Mapeia os dados do sensor de umidade para valores entre 0% e 100%

		controlarSaidas(umid, temp);

		digitalWrite(shift1, LOW);
		shiftRegister(store1, data1, 120);         
		digitalWrite(shift1, HIGH);
		atualizarDisplay(store2, shift2, data2, temp/10);
		atualizarDisplay(store3, shift3, data3, temp%10);
		delay(2000);

		controle = digitalRead(btn_funcao);

		digitalWrite(shift1, LOW);
		shiftRegister(store1, data1, 28);         
		digitalWrite(shift1, HIGH);
		atualizarDisplay(store2, shift2, data2, umid/10);
		atualizarDisplay(store3, shift3, data3, umid%10);
		delay(2000);
	
		controle = digitalRead(btn_funcao);

		controlarSaidas(umid, temp);
	} while (controle == 0);

	do
	{ // Loop após ser precionado o botão de função para ajuste de parâmetros
		cont = contador(contador_saida2, contador_saida1, contador_saida0);
		delay(50);
		temp_min = incrementoDecremento(1, temp_min);
		if (temp_max < temp_min) temp_max = temp_min;
		temp_max = incrementoDecremento(2, temp_max);
		umid_min = incrementoDecremento(3, umid_min);
		if (umid_max < umid_min) umid_max = umid_min;
		umid_max = incrementoDecremento(4, umid_max);

		if (cont == 0) // Retorna ao loop temperatura/umidade
		{
			atualizarDisplay(store2, shift2, data2, 10);
			atualizarDisplay(store3, shift3, data3, 10);
			if(digitalRead(btn_incremento) || digitalRead(btn_decremento)) break;
		}
	} while (controle == 1);
}

// ======================================== Funções ========================================

int contador(int inputA2, int inputA3, int inputA4){
    delay(200);
	// Função do contador: A cada click no botão o contador acrescenta 1. Conforme as saídas em binário o contador retorna o valor decimal no display.
	if (digitalRead(inputA2) == 0 && digitalRead(inputA3) == 0 && digitalRead(inputA4) == 0) {cont = 0;}
	if (digitalRead(inputA2) == 0 && digitalRead(inputA3) == 0 && digitalRead(inputA4) == 1) {cont = 1;}
	if (digitalRead(inputA2) == 0 && digitalRead(inputA3) == 1 && digitalRead(inputA4) == 0) {cont = 2;}
	if (digitalRead(inputA2) == 0 && digitalRead(inputA3) == 1 && digitalRead(inputA4) == 1) {cont = 3;}
	if (digitalRead(inputA2) == 1 && digitalRead(inputA3) == 0 && digitalRead(inputA4) == 0) {cont = 4;}
	atualizarDisplay(store1, shift1, data1, cont);
	return cont;
}

int incrementoDecremento(int x, int variavel) {
	while (cont == x)
	{
		atualizarDisplay(store2, shift2, data2, variavel/10);
		atualizarDisplay(store3, shift3, data3, variavel%10);
		delay(200);
		if(digitalRead(btn_incremento)){
			variavel++;
			delay(50);
		}
		if(digitalRead(btn_decremento)){
			variavel--;
			delay(50);
		}
		cont = contador(contador_saida2, contador_saida1, contador_saida0);
	}
	return variavel;
}

void atualizarDisplay(int storePin, int shiftPin, int dataPin, int dado) {
		digitalWrite(shiftPin, LOW);
		if (dado == 0) shiftRegister(storePin, dataPin, 63);
		else if (dado == 1) shiftRegister(storePin, dataPin, 6);
		else if (dado == 2) shiftRegister(storePin, dataPin, 91);
		else if (dado == 3) shiftRegister(storePin, dataPin, 79);
		else if (dado == 4) shiftRegister(storePin, dataPin, 102);
		else if (dado == 5) shiftRegister(storePin, dataPin, 109);
		else if (dado == 6) shiftRegister(storePin, dataPin, 125);
		else if (dado == 7) shiftRegister(storePin, dataPin, 7);
		else if (dado == 8) shiftRegister(storePin, dataPin, 127);
		else if (dado == 9) shiftRegister(storePin, dataPin, 111);
		else if (dado == 10) shiftRegister(storePin, dataPin, 0);
		digitalWrite(shiftPin, HIGH);
/*
	A função atualizarDisplay é responsável por atualizar um display de 7 segmentos utilizando um registrador de deslocamento, controlado pelos pinos storePin, shiftPin e dataPin.

	Objetivo da Função
	O objetivo da função atualizarDisplay é enviar um número decimal (dado) para um display de 7 segmentos utilizando um registrador de deslocamento conectado aos pinos especificados.

	Parâmetros da Função
	storePin: Pino conectado ao pino de armazenamento (Latch) do registrador de deslocamento.
	shiftPin: Pino conectado ao pino de controle de deslocamento (Clock).
	dataPin: Pino conectado ao pino de dados do registrador de deslocamento.
	dado: Número inteiro que representa o valor a ser exibido no display de 7 segmentos.

	Funcionamento Detalhado
	Baixar o Pino de Deslocamento (shiftPin): O primeiro passo é colocar o pino de deslocamento (shiftPin) em nível baixo (LOW). Isso prepara o registrador de deslocamento para receber novos dados.
	Selecionar o Valor do Display (dado): Em seguida, a função verifica o valor de dado e seleciona o padrão binário correspondente para ser enviado ao display de 7 segmentos através da função shiftRegister.
		Se dado for 0 a 9, então são enviados os valores correspondentes aos segmentos dos displays de 7 segmentos. Por exemplo, dado == 0 corresponde a enviar 63 para o registrador de deslocamento (representando o número 0 em binário para um display de 7 segmentos).
		Se dado for 10, então envia-se 0 para apagar o display de 7 segmentos, possivelmente para limpar a exibição anterior.
	Subir o Pino de Deslocamento (shiftPin): Após enviar o dado correspondente para o registrador de deslocamento, o pino de deslocamento (shiftPin) é colocado em nível alto (HIGH). Isso efetivamente move os dados do registrador para os segmentos do display de 7 segmentos.
*/
}

void controlarSaidas(int umid, int temp) {
	if (umid <= umid_min) {
		digitalWrite(irrigation, HIGH);
    } else if (umid >= (umid_min + (umid_max - umid_min) * 3 / 4)) {
        digitalWrite(irrigation, LOW);
    }

    if (temp >= temp_max) {
	    digitalWrite(cooling, HIGH);
    } else digitalWrite(cooling, LOW);
    if (temp <= temp_min) {
        digitalWrite(heating, HIGH);
    } else digitalWrite(heating, LOW);
	if (temp > temp_min && temp < temp_max) {
        digitalWrite(cooling, LOW);
        digitalWrite(heating, LOW);
    }
}

void shiftRegister(int storePin, int dataPin, byte value) {
    boolean control;
    digitalWrite(storePin, LOW); 
    digitalWrite(dataPin, LOW);
    
    for (int i = 0; i < 8; i++) {
        digitalWrite(storePin, LOW);
        control = (value & (1 << i)) ? HIGH : LOW;
        digitalWrite(dataPin, control);
        digitalWrite(storePin, HIGH);
    }
    digitalWrite(storePin, LOW); 

/*
	Objetivo da Função
	A função shiftRegister é responsável por enviar um byte de dados para um registrador de deslocamento conectado ao Arduino. Registradores de deslocamento como o 74HC595 são utilizados para expandir o número de saídas digitais do Arduino, permitindo o controle de múltiplos dispositivos (como displays de 7 segmentos) com menos pinos digitais.

	Parâmetros
	storePin: Pino conectado ao pino de armazenamento (Latch) do registrador de deslocamento.
	dataPin: Pino conectado ao pino de dados do registrador de deslocamento, usado para enviar dados sequencialmente.
	value: Byte de dados (0 a 255) que será enviado para o registrador de deslocamento.
	
	Funcionamento Detalhado
	Inicialização: Inicialmente, os pinos storePin e dataPin são configurados para saída (OUTPUT).
	Loop de Envio: A função itera 8 vezes através de um loop for, uma vez para cada bit do byte value
	Controle dos Bits: Dentro do loop, o bit atual do byte value é determinado usando operações de bits
		value & (1 << i): Esta expressão usa o operador shift (<<) para deslocar 1 para a esquerda i vezes. Isso cria uma máscara onde apenas o bit atual de value é testado.
		? HIGH : LOW: Se o resultado da operação & for verdadeiro (o bit é 1), control é definido como HIGH, caso contrário, é LOW.
	
	Envio dos Dados: O pino de dados (dataPin) recebe o valor control determinado para o bit atual.

	Armazenamento dos Dados: Após definir o bit atual no dataPin, o pino de armazenamento (storePin) é brevemente colocado em nível baixo (LOW) e depois em nível alto (HIGH). Isso move os dados do registrador de deslocamento para os pinos de saída, atualizando os displays ou dispositivos conectados.

	Finalização: Após o loop de 8 bits, o pino de armazenamento (storePin) é colocado em nível baixo (LOW) novamente para finalizar o processo de envio dos dados.
*/
}