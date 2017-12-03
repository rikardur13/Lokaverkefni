/*
 * uart.h
 *
 * Created: 21.1.2017 14:16:25
 *  Author: rikardur
 */ 
 

#include <asf.h>


#ifndef UART_H_
#define UART_H_

#include "global.h"
#include "interrupts.h"	
#include "tunerstudiocomm.h"

#define ATSAM3X8E_MCK	84000000
#define BAUDRATE		115200
#define CLOCKDIVISION	ATSAM3X8E_MCK/(BAUDRATE*16)
#define TXBUFFERSIZE	1024
#define RXBUFFERSIZE	1024

Pdc *PdcInterface;
pdc_packet_t PdcTxPacket;

volatile uint8_t TxString[TXBUFFERSIZE];
volatile uint16_t TxStringHead;
volatile uint16_t TxStringTail;

volatile uint8_t RxString[RXBUFFERSIZE];
volatile uint16_t RxStringHead;
volatile uint16_t RxStringTail;

volatile uint8_t RxFlag;

void uart_init(void);
void uart_tx_interrupt_init(void);
void uart_interrupt_transfer(char* str);
void uart_interrupt_transfer_specific(char * str, uint16_t BufferLength);
void uart_load_tx_buffer(uint8_t data);
void uart_enable_tx_interrupt(void);
void uart_enable_rx_interrupt(void);
void uart_disable_rx_interrupt(void);
void uart_rx_read_buffer(void);
void uart_transfer(uint8_t transmit);
void uart_print_int(uint32_t data);
void uart_new_line(void);
void uart_print_string(char * data);
uint8_t uart_receive(void);
uint8_t uart_load_pdc_tx_buffer(uint8_t * address, uint16_t size);
//void uart_debug_transfer_new_message(uint32_t Time, char *String, uint32_t Value);
void uart_add_int_to_char_array(uint8_t *array, uint32_t data, uint16_t *counter);



void uart_print_map_value(void);
void uart_print_iat_value(void);

#endif /* UART_H_ */