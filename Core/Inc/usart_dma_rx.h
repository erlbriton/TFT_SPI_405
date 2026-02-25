/*
 * usart_dma_rx.h
 *
 *  Created on: Apr 13, 2025
 *      Author: erlbriton
 */

#ifndef INC_USART_DMA_RX_H_
#define INC_USART_DMA_RX_H_

#define RX_BUFFER_SIZE 64
#define PACKET_SIZE    20

extern uint8_t rxBuffer[RX_BUFFER_SIZE];
extern uint8_t buff_new[PACKET_SIZE];

#endif /* INC_USART_DMA_RX_H_ */
