#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

typedef short _stdcall (*PtrInp)(short EndPorta);
typedef void _stdcall (*PtrOut)(short EndPorta, short valor);

HINSTANCE hLib; //Inst�ncia para a DLL inpout32.dll.
PtrInp inportb;     //Inst�ncia para a fun��o Imp32().
PtrOut outportb;  //Inst�ncia para a fun��o Out32().


void bemVindo(); 	// prototipo da funcao de boas vindas
void menuPrincipal();
void recebeVariaveis(); // prototipo da funcao input sequencia
void sequencia(int quantPassos, int passos[], int atraso, int repeticao);	// prototipo da funcao de sequencia
void atrasoFuc(int atraso); // prototipo da funcao atraso
int carregaLib();

int main(){
	bemVindo();
	if(carregaLib() == -1) return -1;
	short escolha = 0;
	while(1){
		outportb(0x378, 0);
		system("cls");
		menuPrincipal();
		scanf("%d", &escolha);
		if(escolha == 1) recebeVariaveis(0);
		else if(escolha == 2) recebeVariaveis(1);
		else if(escolha == 3) break;
		else{
			printf("Escolha invalida");
			atrasoFuc(50);
		}
	}
	system("exit");

	return 0;
}
/** Funcao de boas vindas **/
void bemVindo(){
	system("title CONTROLE PORTA PARALELA"); // Muda o titulo da janela
	printf("Bem-vindo ao controle de porta paralela\n"); // Mensagem de boas vindas
	int i;
	for(i = 0; i < 39; i++){
		printf("=");
		atrasoFuc(5);
	}
	system("cls");
}
/** funcao que carrega input32.dll **/
int carregaLib(){

   //Carrega a DLL na mem�ria.
   hLib = LoadLibrary("inpout32.dll");

   if(hLib == NULL) //Verifica se houve erro.
   {
      printf("Erro. O arquivo inpout32.dll n�o foi encontrado.\n");
      getch();
      return -1;
   }

   //Obt�m o endere�o da fun��o Inp32 contida na DLL.
   inportb = (PtrInp) GetProcAddress(hLib, "Inp32");

   if(inportb == NULL) //Verifica se houve erro.
   {
      printf("Erro. A fun��o Inp32 n�o foi encontrada.\n");
      getch();
      return -1;
   }

   //Obt�m o endere�o da fun��o Out32 contida na DLL.
   outportb = (PtrOut) GetProcAddress(hLib, "Out32");

   if(outportb == NULL) //Verifica se houve erro.
   {
       printf("Erro. A fun��o Out32 n�o foi encontrada.\n");
       getch();
       return -1;
   }
}
/** funcao de exibicao do menu principal **/
void menuPrincipal(){
	printf("escolha\t\tfuncao\n\n"); // Cabecalho do menu Inicial
	/*** Itens do menu ***/
	printf("1\t\tIniciar sequencia em decimal\n");
	printf("2\t\tIniciar sequencia em binario\n");
	printf("3\t\tSair\n");
}
/** Funcao para definir a sequencia de LEDs acesos **/
void recebeVariaveis(int binario){
	system("cls");
	int quantPassos = 0; // Variavel que ira receber a quantidade de passos
	printf("Digite a quantidade de passos: "); // Mensagem com informacao para usuario
	scanf("%d", &quantPassos); // Recebe o valor digitado, sem tratamento
	int passos[quantPassos]; // Declara um array de tamanho "quantPassos"
	int i; // Declara um contador
	/*** Loop para zerar todas as posicoes do array ***/
	for(i = 0; i < quantPassos; i++){
		passos[i] = 0;
	}
	/*** Loop para armazenar cada passo ***/
	if(binario == 0)
		for(i = 0; i < quantPassos; i++){
			do{
			printf("Digite o %d passo (menor  que 255): ", i + 1);
			scanf("%d", &passos[i]);
		}while(passos[i] > 255);
	}
	else
		for(i = 0; i < quantPassos; i++){
			printf("Digite o %d passo (8 bits): ", i);
			int valorBinario;
			scanf("%d", &valorBinario);
			passos[i] = converteParaDecimal(valorBinario);
		}
	int atraso = 0; // Variavel para receber o tempo entre cada passo
	printf("Digite o intervalo entre cada passo (em ms, e apenas inteiros): ");
	scanf("%d", &atraso);
	int repeticao = 0;
	printf("Digite quantas vezes a sequencia se repetira: ");
	scanf("%d", &repeticao);
	sequencia(quantPassos, passos, atraso, repeticao);
}
// ** funcao que converte de binario para decimal **/
int converteParaDecimal(int valorBinario){
	int valorDecimal = 0;
	int potencia = 1;
	while(valorBinario > 0){
		valorDecimal += valorBinario % 10 * potencia;
		valorBinario /= 10;
		potencia *= 2;
	}
	return valorDecimal;
}
/** funcao que faz sequencia **/
void sequencia(int quantPassos, int passos[], int atraso, int repeticao){
	int i, j;
	system("cls");
	for(i = 0; i < repeticao; i++){
		printf("Sequencia em progresso");
		for(j = 0; j < quantPassos; j++){
			if(j % 3 == 0) printf(".");
			outportb(0x378, passos[j]);
			atrasoFuc(atraso);
		}
		system("cls");
	}
}
/** funcao de atraso **/
void atrasoFuc(int atraso){
	int x; // Define o contador
	clock_t Ini;  //Criacao da variavel relogio
	Ini = clock(); //Inicializacao do relogio
	 //Executa o while por 10ms antes de retornar o valor
	while(((clock() - Ini) / (CLOCKS_PER_SEC / 100)) <= atraso){
		x++;
	}
}
