/*
 * func_CAN.c
 *
 *  Created on: Jul 8, 2023
 *      Author: David
 */

#include "main.h"
#include "stm32h7xx_hal.h"

#ifndef INC_FUNC_CAN_C_
#define INC_FUNC_CAN_C_


// BEGIN Funções de Escrita

	void send_info_CAN(FDCAN_HandleTypeDef hfdcan, uint8_t id, uint64_t value);

	void uint64_to_array_of_uint8(uint8_t *bytes, uint64_t value, uint8_t number_of_bytes);

	uint8_t minimum_number_of_bytes_to_represent_value(uint64_t value);

	void configure_message_header(FDCAN_TxHeaderTypeDef *TxHeader, uint8_t id, uint8_t num_of_bytes);

// END Funções de Escrita

//-----------------------------------------------------------------------------------------------------------//

// BEGIN Funções de Leitura

	uint64_t array_of_uint8_to_uint64(uint8_t *bytes, uint8_t num_of_bytes);

// END Funções de Leitura

//-----------------------------------------------------------------------------------------------------------//

// BEGIN Funções Auxiliares

	// A Biblioteca CAN reconhece o tamanho do pacote em bytes por uma notação não linear, as funções fazem as conversões necessarias
	uint32_t number_of_bytes_to_ST_notation(uint8_t num_of_bytes);
	uint8_t ST_notation_to_number_of_bytes(uint32_t ST_notation);

// END Funções Auixiliares




#endif /* INC_FUNC_CAN_C_ */
