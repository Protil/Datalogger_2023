/*
 * cartaoSD.h
 *
 *  Created on: 29 de mai de 2023
 *      Author: David
 */

#ifndef INC_CARTAOSD_H_
#define INC_CARTAOSD_H_

// Confere se o cartão está presente para controlar a LED da placa
int CartaoSDPresente();

//
void EscreverSD();

//

char* LerCartao();


#endif /* INC_CARTAOSD_H_ */
