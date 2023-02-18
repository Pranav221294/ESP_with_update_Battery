#include <Arduino.h>
#include "driver/uart.h"

SemaphoreHandle_t uart0_handle;

void UART0_Interrupt() {
  // Read data from UART0
  uint8_t data[20];
  int len = uart_read_bytes(UART_NUM_0, data, 20, 0);
  if (len > 0) {
    // Process received data
  }
  // Clear interrupt status
  uart_clear_intr_status(UART_NUM_0, UART_RXFIFO_FULL_INT_CLR | UART_RXFIFO_TOUT_INT_CLR);
}

void setup() {
  // Configure UART0 for 115200 baud, 8-bit data, no parity, and 1 stop bit
  uart_config_t uart_config = {
    .baud_rate = 115200,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
  };
  uart_param_config(UART_NUM_0, &uart_config);

  // Set UART0 pins (TX: GPIO1, RX: GPIO3)
  uart_set_pin(UART_NUM_0, 1, 3, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

  // Install UART0 driver
  uart_driver_install(UART_NUM_0, 2048, 0, 0, NULL, 0);

  // Enable UART0 receive interrupt
  uart_enable_rx_intr(UART_NUM_0);
  // Register interrupt handler
  esp_intr_alloc(ETS_UART0_INTR_SOURCE, ESP_INTR_FLAG_LEVEL1 | ESP_INTR_FLAG_IRAM, UART0_Interrupt, NULL, &uart0_handle);
}

void loop() {
  // Do other tasks
}
