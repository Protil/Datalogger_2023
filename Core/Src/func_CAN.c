/*
 * func_CAN.c
 *
 *  Created on: Jul 8, 2023
 *      Author: David
 */

#include "func_CAN.h"

#include <stdint.h>


// BEGIN Funções de Escrita

void send_message_CAN_positive(FDCAN_HandleTypeDef hfdcan, uint8_t id, uint64_t value)
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

void send_message_CAN_negative(FDCAN_HandleTypeDef hfdcan, uint8_t id, int64_t value)
{
	uint8_t TxData[8] = {0};
	FDCAN_TxHeaderTypeDef TxHeader;
    uint8_t number_of_bytes;

    // Caso o valor seja negativo é necessario inverter-lo para a contagem de bytes
    // Caso isso não seja feito todo numero negativo retornara 8 bytes
    if (value < 0)
    {
    	number_of_bytes = minimum_normal_number_byte_size(-value);
    }
    else
    {
    	number_of_bytes = minimum_normal_number_byte_size(value);
    }

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

uint8_t minimum_number_of_bytes_to_represent_value(uint64_t value)
{
    uint8_t number_of_bytes = 0;

    uint64_t copy_value = value;

    // Desloca o numero de 8 em 8 bits até alcançar 0 e assim saber o tamanho minimo de bytes que pode representar o valor
    while (copy_value != 0)
    {
    	copy_value >>= 8;
        number_of_bytes++;
    }

    // Caso o valor seja 0, precisamos lidar com o fato que é necessario mandar pelo menos 1 bytes mostrando que o valor é 0 e não um pacote vazio
    if (number_of_bytes == 0)
    {
    	number_of_bytes = 1;
    }

    return number_of_bytes;
}

uint8_t minimum_normal_number_byte_size(uint64_t value)
{
    uint8_t numBytes = 0;

    uint64_t copy_value = value;

    // Desloca o numero de 8 em 8 bits até alcançar 0 e assim saber o tamanho minimo de bytes que pode representar o valor
    while (copy_value != 0)
    {
    	copy_value >>= 8;
        numBytes++;
    }

    // Garante que o numero de bytes retornado será o padrão de 1, 2, 4 ou 8
    if(numBytes != 1 && numBytes != 2 && numBytes != 4 && numBytes != 8)
    {
    	if (numBytes < 1)
    	{
    		numBytes = 1;
    	}
        if(numBytes < 2)
        {
            numBytes = 2;
        }
        else if(numBytes < 4)
        {
            numBytes = 4;
        }
        else if (numBytes < 8)
        {
            numBytes = 8;
        }
        else
        {
            Error_Handler();
        }
    }
    return numBytes;
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
		return FDCAN_DLC_BYTES_0;
	case 1:
		return FDCAN_DLC_BYTES_1;
	case 2:
		return FDCAN_DLC_BYTES_2;
	case 3:
		return FDCAN_DLC_BYTES_3;
	case 4:
		return FDCAN_DLC_BYTES_4;
	case 5:
		return FDCAN_DLC_BYTES_5;
	case 6:
		return FDCAN_DLC_BYTES_6;
	case 7:
		return FDCAN_DLC_BYTES_7;
	case 8:
		return FDCAN_DLC_BYTES_8;
	default:
		// Não é a melhor forma de se lidar com o erro de usar mais bytes do que a CAN suporta
		Error_Handler();
		return HAL_ERROR;
	}
}

uint8_t ST_notation_to_number_of_bytes(uint32_t ST_notation)
{
	switch (ST_notation)
	{
	case FDCAN_DLC_BYTES_0:
		return 0;
	case FDCAN_DLC_BYTES_1:
		return 1;
	case FDCAN_DLC_BYTES_2:
		return 2;
	case FDCAN_DLC_BYTES_3:
		return 3;
	case FDCAN_DLC_BYTES_4:
		return 4;
	case FDCAN_DLC_BYTES_5:
		return 5;
	case FDCAN_DLC_BYTES_6:
		return 6;
	case FDCAN_DLC_BYTES_7:
		return 7;
	case FDCAN_DLC_BYTES_8:
		return 8;
	default:
		// Não é a melhor forma de se lidar com o erro de usar mais bytes do que a CAN suporta
		Error_Handler();
		return HAL_ERROR;
	}
}

// END Funções Auixiliares

















