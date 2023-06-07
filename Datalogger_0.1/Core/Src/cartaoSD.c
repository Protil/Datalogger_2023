/*
 * cartaoSD.c
 *
 *  Created on: 29 de mai de 2023
 *      Author: David
 */

#include "cartaoSD.h"
#include "gpio.h"
#include "sdmmc.h"


// Melhor jeito de pegar o argumento?
int CartaoSDPresente(SD_HandleTypeDef csd)
{
	int cartaoInserido = 0;

	// Não é necessariamente o erro de ausencia de cartão sd, posso conferir isso caso ele esteja preso no loop de resposta
	// Esse numero de erro não faz sentido nenhum (descobrir o que ele significa)
	if (hsd1.ErrorCode == 268435456)
	{
		cartaoInserido = 1;
		//HAL_GPIO_WritePin(GPIOD, Datalog1_Pin, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOD, Datalog1_Pin, GPIO_PIN_RESET);
	}

	return cartaoInserido;
}

// Primeiro Escrever depois Ler
void EscreverSD()
{

}

//
char* LerCartao()
{
	return "a";
}

