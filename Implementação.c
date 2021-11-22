//ANALISE E PROJETO DE ALGORITMOS II
//Trabalho 02 - Um escalonamento de tres maquinas
//Nome: João Vitor de Faria
//Matricula: 2019006030
//Solucao baseada no codigo disponibilizado pelo professor.

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

//essa funcao recebe tres maquinas e retorna a maior delas
int define_maior(int a, int b, int c)
{
	if ( a >= b )
	{
		if ( a >= c)return a;
		else return c;
	}
	else
	{
		if( b >= c) return b;
		else return c;
	}
}

//Essa heuristica coloca o item sempre na maquina menos carregada
int heuristica(int num_tarefas, int * custo_tarefas, int tarefa_atual, int maquina_1, int maquina_2, int maquina_3, int * guardada_em, int *melhor_valor_solucao, int* melhor_solucao)
{
	int i;
	for(i = tarefa_atual; i < num_tarefas; i++)
	{
		if(maquina_1<=maquina_2){
			if(maquina_1<=maquina_3){
				guardada_em[i] = 1;
				maquina_1 += custo_tarefas[i];
			}else{
				guardada_em[i] = 3;
				maquina_3 += custo_tarefas[i];
			}
		}else{
			if(maquina_2<=maquina_3){
				guardada_em[i] = 2;
				maquina_2 += custo_tarefas[i];
			}else{
				guardada_em[i] = 3;
				maquina_3 += custo_tarefas[i];
			}
		}
	}
	int maior = define_maior(maquina_1, maquina_2, maquina_3);
	if(maior < *melhor_solucao)
	{
		//encontrei uma solucao melhor
		*melhor_solucao = maior;
		for(i = 0; i < num_tarefas; i++)melhor_valor_solucao[i] = guardada_em[i];
	}
	return maior;
}

void escalonamento(int num_tarefas, int * custo_tarefas, int tarefa_atual, int * guardado_em, int *melhor_valor_solucao, int *melhor_solucao)
{
	if (tarefa_atual == num_tarefas)
	{
		//tenho uma solucao completa
		int maquina_1 = 0;
		int maquina_2 = 0;
		int maquina_3 = 0;
		int i;

		for(i = 0; i < num_tarefas; i++)
		{
			if(guardado_em[i] == 1)maquina_1 += custo_tarefas[i];
			if(guardado_em[i] == 2)maquina_2 += custo_tarefas[i];
			if(guardado_em[i] == 3)maquina_3 += custo_tarefas[i];
		}
	
		int maior = define_maior(maquina_1, maquina_2, maquina_3);

	
		if(maior < *melhor_solucao)
		{
			//encontrei uma solucao melhor
			*melhor_solucao = maior;
			for(i = 0; i < num_tarefas; i++)
				melhor_valor_solucao[i] = guardado_em[i];
		}
		return;
	}
	//calculando a carga das maquinas nessa solucao parcial,
	//e quanto sobra de item
	int maquina_1 = 0;
	int maquina_2 = 0;
	int maquina_3 = 0;

	int i;
	for(i = 0; i < tarefa_atual; i++)
	{
		if(guardado_em[i] == 1) maquina_1 += custo_tarefas[i];
		if(guardado_em[i] == 2) maquina_2 += custo_tarefas[i];
		if(guardado_em[i] == 3) maquina_3 += custo_tarefas[i];
	}
	//****************************************************
	//calculando um limitante superior (limitante primal), usando uma heuristica para
	//encontrar uma solucao valida, dada uma solucao parcial.
	//note que eh um problema de minimizacao entao a heuristica vai devolver uma
	//solucao maior ou igual a otima
	heuristica(num_tarefas, custo_tarefas, tarefa_atual, maquina_1, maquina_2, maquina_3, guardado_em, melhor_valor_solucao, melhor_solucao);

	//****************************************************
	//calculando um limitante inferir, vamos relaxar a restricao de integralidade.
	//Intuitivamente, o resto eh a soma dos itens que faltam alocar, se pudessemos
	//fracionar as tarefas, e colocar elas maquinas
	int resto = 0;
	for(i = tarefa_atual; i < num_tarefas; i++)
	{
		resto += custo_tarefas[i];
	}
	int maior = define_maior(maquina_1, maquina_2, maquina_3);
	//aqui entende-se que eu vou completar a maquina menos carregada
	//ate ficar igual a maquina mais carregada
	resto = resto - (maior - maquina_1);
	resto = resto - (maior - maquina_2);
	resto = resto - (maior - maquina_3);
	//esse resto é a diferença entre maior e menor maquina// so calcular o maior e menor e fazer o mesmo.
	//se sobrou itens, utopicamente poderiamos dividir o que sobrou exatamente
	//igual entre as maquinas. Como os tempos sao inteiros, o tempo certamente
	//sera inteiro e por isso eu posso tirar o teto

	if( resto > 0) maior += ceil((double)resto / 3);

	//Poda por limitante BOUND
	if(maior >= *melhor_solucao)
	{
		//Esse ramo nao eh promissor, pode matar
		return ;
	}
	//****************************************************
	//BRANCHING
	guardado_em[tarefa_atual] = 1;
	escalonamento(num_tarefas, custo_tarefas, tarefa_atual + 1, guardado_em, melhor_valor_solucao, melhor_solucao);
	guardado_em[tarefa_atual] = 2;
	escalonamento(num_tarefas, custo_tarefas, tarefa_atual + 1, guardado_em, melhor_valor_solucao, melhor_solucao);
	guardado_em[tarefa_atual] = 3;
	escalonamento(num_tarefas, custo_tarefas, tarefa_atual + 1, guardado_em, melhor_valor_solucao, melhor_solucao);
}

void resolve(int num_tarefas, int * custo_tarefas)
{
	//guardada_em[i] guarda a maquina em que a tarefa i estah guardada, 1, 2 ou 3
	int * guardada_em = (int*)malloc(sizeof(int) * num_tarefas);
	int * melhor_valor_solucao = (int*)malloc(sizeof(int) * num_tarefas);
	int melhor_solucao = INT_MAX;

	escalonamento(num_tarefas, custo_tarefas, 0, guardada_em, melhor_valor_solucao, &melhor_solucao);
	printf("%d\n", melhor_solucao);
	return;
}

int main()
{
	int num_tarefas;
	int * custo_tarefas;
	
	scanf("%d", &num_tarefas);

	custo_tarefas = (int *)malloc(sizeof(int) * num_tarefas);

	int i;
	for(i = 0; i < num_tarefas; i++)
		scanf("%d", &(custo_tarefas[i]));

	resolve(num_tarefas, custo_tarefas);
	return 0;
}

