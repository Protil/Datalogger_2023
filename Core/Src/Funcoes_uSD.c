/*
 * Funcoes_uSD.c
 *
 *  Created on: Jun 4, 2023
 *      Author: David
 */


#include "Funcoes_uSD.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>


static FATFS g_sFatFs;
FIL file;
FILINFO fileInfo;

// Usamos 8.3 format file name (SFN), que permite 9 bits no nome, isso signfica que só podemos colocar 8 letras no nome do arquivo, imagino que a 9 seja um caracter de terminacao ou o


FRESULT SD_Criar_Arquivo(const char* nomeArquivo)
{
	FRESULT fresult;

	fresult = f_mount(&g_sFatFs, "0:", 0);	/*mount SD card*/

	fresult = f_stat(nomeArquivo, &fileInfo);

	fresult = f_open(&file, nomeArquivo, FA_CREATE_ALWAYS ); /*Create file on SD card*/

	fresult = f_close(&file);

    f_close(&file);

    //O unmount é a função mount adaptada como abaixo, de acordo com http://elm-chan.org/fsw/ff/doc/mount.html
	fresult = f_mount(0, "0:", 0);	/*unmount SD card*/

	return fresult;
}



FRESULT SD_Criar_Diretorio(const char caminhoDiretorio)
{
	FRESULT fresult;

	// mount precisa estar aqui
	fresult = f_mount(&g_sFatFs, "0:", 0);	/*mount SD card*/

	fresult = f_mkdir(&caminhoDiretorio);

    f_close(&file);

	return fresult;
}

// Acho que rola de montar o cartão antes do que em cada função, ela só monta se já n tiver sido montado

FRESULT SD_Ler_Arquivo(const char* nomeArquivo, char* block)
{
	FRESULT fresult;

	uint controle;
	// Precisa montar o cartão
	fresult = f_mount(&g_sFatFs, "0:", 0);	/*mount SD card*/

	// Abrir o arquivo especifo
	fresult = f_open(&file, nomeArquivo, FA_READ);

	fresult = f_read(&file, block, 700, &controle);

	f_close(&file);

	fresult = f_mount(0, "0:", 0);	/*unmount SD card*/


	return fresult;
}

FRESULT SD_Escrever_Arquivo(const char* nomeArquivo, const char* informacao)
{
	FRESULT fresult;

	uint bw;
	fresult = f_mount(&g_sFatFs, "0:", 0);	/*mount SD card*/
	fresult = f_open(&file, nomeArquivo, FA_WRITE );
	fresult = f_write(&file, informacao, 10, &bw);
	fresult = f_mount(0, "0:", 0);	/*unmount SD card*/

    f_close(&file);

	return fresult;
}











