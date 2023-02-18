#include "Arduino.h"
#include "driver/uart.h"
#include <stdio.h>
#include <string.h>
#include "Esp_UART_1.h"

static intr_handle_t handle_console;
uint8_t rxbuf_1[256];
uint16_t urxlen;

void IRAM_ATTR uart_intr_handle(void *arg)
{
  uint16_t rx_fifo_len, status;
  uint16_t i=0;
  status = UART1.int_st.val; // read UART interrupt Status
  rx_fifo_len = UART1.status.rxfifo_cnt; // read number of bytes in UART buffer
  while(rx_fifo_len)
  {
   rxbuf_1[i++] = UART1.fifo.rw_byte; // read all bytes
   rx_fifo_len--;
  }
 uart_clear_intr_status(EX_UART_NUM, UART_RXFIFO_FULL_INT_CLR|UART_RXFIFO_TOUT_INT_CLR);//clear UART interrupt status
 uart_write_bytes(EX_UART_NUM, (const char*)rxbuf_1, i);
  memset(rxbuf_1, '\0', i); 
}

void UART_1_init()
{
	 uart_config_t uart_config =
  {
    .baud_rate = 115200,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
  };
  uart_param_config(EX_UART_NUM, &uart_config);
 // uart_set_pin(EX_UART_NUM, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
  uart_set_pin(EX_UART_NUM,GPIO_NUM_4,GPIO_NUM_5,UART_PIN_NO_CHANGE,UART_PIN_NO_CHANGE);
  uart_driver_install(EX_UART_NUM, BUF_SIZE_1 * 2, 0, 0, NULL, 0);//Install UART driver, and get the queue.
  uart_isr_free(EX_UART_NUM);// release the pre registered UART handler/subroutine
  uart_isr_register(EX_UART_NUM,uart_intr_handle, NULL, ESP_INTR_FLAG_IRAM, &handle_console); // register new UART subroutine
  uart_enable_rx_intr(EX_UART_NUM); // enable RX interrupt
	
	
	
}