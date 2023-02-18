
#define BUF_SIZE_1 (1024)
#define EX_UART_NUM UART_NUM_1
#define RD_BUF_SIZE (BUF_SIZE_1)

void IRAM_ATTR uart_intr_handle(void *arg);
void UART_1_init();