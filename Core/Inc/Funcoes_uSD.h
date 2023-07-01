/*
 * Funcoes_uSD.h
 *
 *  Created on: Jun 4, 2023
 *      Author: David
 */

#include "main.h"
#include "stm32h7xx_hal.h"
#include "fatfs.h"

#ifndef INC_FUNCOES_USD_H_
#define INC_FUNCOES_USD_H_

FRESULT SD_Criar_Arquivo(const char*);
FRESULT SD_Criar_Diretorio(const char);
FRESULT SD_Ler_Arquivo(const char*, char*);
FRESULT SD_Escrever_Arquivo(const char* nomeArquivo, const char* informacao);

#endif /* INC_FUNCOES_USD_H_ */

