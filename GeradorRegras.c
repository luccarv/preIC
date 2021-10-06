#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

//Tupla representando a nota com sua frequência e duração.
typedef struct _tuple {
	double frequencia;
	double duracao;
}tuple;

//Algoritmo de busca binária.
int binsearch(int *arr, int l, int r, int x) {
	if(r >= 1) {
		int m = l + (r-l)/2;
		if(arr[m] == x)
			return m;
		if(arr[m] > x)
			return binsearch(arr, l, m-1, x);
		return binsearch(arr, m+1, r, x);
	}
	return -1; 
}

//Carrega a melodia num vetor de notas.
tuple *getMelodia(int tam, int tempo) {
	//Calcula a duração da semínima em milissegundos.
	double semiMillis = (60.0/(double)tempo)*1000;

	//Vetor contendo os valores MIDI possíveis notas iniciais.
	int notasIniciais[5] = {48, 55, 60, 67, 72};

	//Vetor contendo os valores MIDI possíveis para todas as notas.
	int notas[15] = {48, 50, 52, 53, 55, 57, 59, 
					 60, 62, 64, 65, 67, 69, 71, 72};

	//Fator multiplicativo de cada figura.
	double fatorFiguras[7] = {0.0625, 0.125, 0.25, 0.5, 1, 2, 4};

	//Alocação do vetor de notas
	tuple *melodia = (tuple*)malloc(sizeof(tuple)*tam);

	//Escolhe a primeira nota entre os Dós e os Sóis aleatoriamente.
	int tmpNota = notasIniciais[rand() % 5];
	int tmpIdx = binsearch(notas, 0, 13, tmpNota);

	double duracao = fatorFiguras[rand()%6]*semiMillis;

	melodia[0].frequencia = (double)440*pow(2, (tmpNota-69)/12.0);
	melodia[0].duracao = duracao;


	//Escolhe as notas subsequentes aleatoriamente, desde que não pertençam a uma sequência proibida.
	for(int i = 1; i < tam-1; i++) {
		int isValid = 0; 

		while(!isValid) {
			//Índice que serve para pegar a nota seguinte.
			int prevIdx = tmpIdx;

			//Para somar +-4 ao índice.
			int incremento = 4 - (rand()%8+1);

			//Decide se pode somar +-4 ou +4 ou -4.
			if(tmpIdx == 0)
				tmpIdx += abs(incremento);
			else if(tmpIdx == 13)
				tmpIdx -= abs(incremento);
			else
				tmpIdx += incremento;

			//Limita o índice aos limites do vetor.
			if(tmpIdx < 0)
				tmpIdx = 0;
			if(tmpIdx > 13)
				tmpIdx = 13;

			//Se for um Mi ou um Si, muda o índice para o Fá ou o Dó superior
			if(tmpIdx == 2 || tmpIdx == 6 || tmpIdx == 9 || tmpIdx == 13) {
				tmpIdx++;
			}
				

			//Sequências proibidas que invalidam a seleção da nota.
			if( (notas[prevIdx] == 65 && notas[tmpIdx] == 71) || 
				(notas[prevIdx] == 71 && notas[tmpIdx] == 65) ||
				(notas[prevIdx] == 65 && notas[tmpIdx] == 59) ||
				(notas[prevIdx] == 53 && notas[tmpIdx] == 69) 
			)	continue;
			isValid = 1;
		}
		//Preenche o vetor de tuplas com as notas, contendo suas frequências e durações.
		duracao = fatorFiguras[rand()%6]*semiMillis;

		tmpNota = notas[tmpIdx];

		melodia[i].frequencia = 440*pow(2, (double)(tmpNota-69)/12.0);
		melodia[i].duracao = duracao;
	}
	//Insere Do4 ao final do vetor.
	tmpNota = notas[7];

	duracao = fatorFiguras[rand()%6]*semiMillis;

	melodia[tam-1].frequencia = 440*pow(2, (double)(tmpNota-69)/12.0);
	melodia[tam-1].duracao = duracao; 

	//Retorna a melodia 
	return melodia;
}

//Toca as frequências calculadas utilizando a função Beep()
void playMelodia(tuple *melodia, int size) {
	for(int i = 0; i < size; i++) {
		printf("Tocando frequencia %dHz, com duracao de %dms \n\n", (int)melodia[i].frequencia, (int)melodia[i].duracao);
		Beep((int)melodia[i].frequencia, (int)melodia[i].duracao);
	}

}

//O programa deve ser executado seguido dos argumentos "número de notas" e "semínimas por minuto".
int main(int argc, char *argv[]) {
    if(argc < 3) {
    	printf("Argumentos insuficientes.\n");
		return 0;
	}
	time_t t;

	srand((unsigned) time(&t));

	tuple *melodia = getMelodia(atoi(argv[1]), atoi(argv[2]));

	playMelodia(melodia, atoi(argv[1]));
	
	return 0;
}