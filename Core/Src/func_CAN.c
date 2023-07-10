/*
 * func_CAN.c
 *
 *  Created on: Jul 8, 2023
 *      Author: David
 */

#include "func_CAN.h"

#include <stdint.h>


// BEGIN Funções de Escrita

void send_info_CAN(FDCAN_HandleTypeDef hfdcan, uint8_t id, uint64_t value)
{
	uint8_t TxData[8] = {0};
	FDCAN_TxHeaderTypeDef TxHeader;
    uint8_t number_of_bytes = minimum_number_of_bytes_to_represent_value(value);


    uint64_to_array_of_uint8(TxData, value, number_of_bytes);
	configure_message_header(&TxHeader, id, number_of_bytes);

  // Boa pratica colocar dentro de um if (eu acho)
  if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan, &TxHeader, TxData)!= HAL_OK)
  {
	Error_Handler();
  }
}

// Função feita com base em https://www.educative.io/answers/how-to-convert-an-integer-into-a-specific-byte-array-in-cpp
void uint64_to_array_of_uint8(uint8_t *bytes, uint64_t value, uint8_t number_of_bytes)
{
  for (int byte_index = 0; byte_index < number_of_bytes; byte_index++)
  {
    bytes[byte_index] = (value >> ((number_of_bytes - byte_index - 1) * 8)) & 0xFF;
  }
}

uint8_t minimum_number_of_bytes_to_represent_value(uint64_t num)
{
    uint8_t number_of_bytes = 0;

    uint64_t copy_number = num;

    // Desloca o numero de 8 em 8 bits até alcançar 0 e assim saber o tamanho minimo de bytes que pode representar o valor
    while (copy_number != 0)
    {
    	copy_number >>= 8;
        number_of_bytes++;
    }

    // Caso o valor seja 0, precisamos lidar com o fato que é necessario mandar pelo menos 1 bytes mostrando que o valor é 0 e não um pacote vazio
    if (number_of_bytes == 0)
    {
    	number_of_bytes = 1;
    }

    return number_of_bytes;
}

void configure_message_header(FDCAN_TxHeaderTypeDef *TxHeader, uint8_t id, uint8_t num_of_bytes)
{
	uint32_t dataLength_ST_notation = number_of_bytes_to_ST_notation(num_of_bytes);

	TxHeader->Identifier = id;
	TxHeader->IdType = FDCAN_STANDARD_ID;
	TxHeader->TxFrameType = FDCAN_DATA_FRAME;
	TxHeader->DataLength = dataLength_ST_notation;
	TxHeader->ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	TxHeader->BitRateSwitch = FDCAN_BRS_OFF;
	TxHeader->FDFormat = FDCAN_FD_CAN;
	TxHeader->TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	TxHeader->MessageMarker = 0;
}

// END Funções de Escrita

//-----------------------------------------------------------------------------------------------------------//


// BEGIN Funções de Leitura

uint64_t array_of_uint8_to_uint64(uint8_t *bytes, uint8_t num_of_bytes)
{
    uint64_t total_value = 0;
    uint64_t copy_of_byte_to_uint64;

    for(int byte_index = 0; byte_index < num_of_bytes; byte_index++)
    {
        // Quando não fazemos essa copia, o limite é de 4 bytes pois estoura o uint8_8, como descrito nesse post
        // https://stackoverflow.com/questions/4201301/warning-left-shift-count-width-of-type
    	copy_of_byte_to_uint64 = bytes[byte_index];

    	// Link para o entendimento https://learn.microsoft.com/en-us/cpp/cpp/left-shift-and-right-shift-operators-input-and-output?view=msvc-170
        total_value += ( copy_of_byte_to_uint64 << ((num_of_bytes - byte_index - 1) * 8));
    }

   return total_value;
}

// END Funções de Leitura

//-----------------------------------------------------------------------------------------------------------//


// BEGIN Funções Auxiliares

// Caso o valor seja de 11 bytes, o retorno sera 0, isso devera ser consertado com o uso da FD_CAN, uma sugestão seria tratar o dado pra 11 virar 12, switch não suporta comparações no case
uint32_t number_of_bytes_to_ST_notation(uint8_t num_of_bytes)
{
	switch (num_of_bytes)
	{
	case 0:
		return 0x00000000U;
	case 1:
		return 0x00010000U;
	case 2:
		return 0x00020000U;
	case 3:
		return 0x00030000U;
	case 4:
		return 0x00040000U;
	case 5:
		return 0x00050000U;
	case 6:
		return 0x00060000U;
	case 7:
		return 0x00070000U;
	case 8:
		return 0x00080000U;
	case 12:
		return 0x00090000U;
	case 16:
		return 0x000A0000U;
	case 20:
		return 0x000B0000U;
	case 24:
		return 0x000C0000U;
	case 32:
		return 0x000D0000U;
	case 48:
		return 0x000E0000U;
	case 64:
		return 0x000F0000U;
	default:
		return 0x00000000U;
	}
}

uint8_t ST_notation_to_number_of_bytes(uint32_t ST_notation)
{
	switch (ST_notation)
	{
	case 0x00000000U:
		return 0;
	case 0x00010000U:
		return 1;
	case 0x00020000U:
		return 2;
	case 0x00030000U:
		return 3;
	case 0x00040000U:
		return 4;
	case 0x00050000U:
		return 5;
	case 0x00060000U:
		return 6;
	case 0x00070000U:
		return 7;
	case 0x00080000U:
		return 8;
	case 0x00090000U:
		return 12;
	case 0x000A0000U:
		return 16;
	case 0x000B0000U:
		return 20;
	case 0x000C0000U:
		return 24;
	case 0x000D0000U:
		return 32;
	case 0x000E0000U:
		return 48;
	case 0x000F0000U:
		return 64;
	default:
		return 0;
	}
}

// END Funções Auixiliares

















