#define NRF51 1
//#define SIMPLE_BLINK 1
#include <stdbool.h>
#include <stddef.h>
#include <libopencm3/nrf/periph.h>
#include <libopencm3/nrf/clock.h>
#include <libopencm3/nrf/51/nvic.h>
#include <libopencm3/nrf/uart.h>
#include <libopencm3/nrf/gpio.h>
#include <libopencm3/nrf/rtc.h>

volatile bool uart0_tx_ready = true;
void print_line(uint32_t uart,char* s);


int main(void)
{
  //Configure nvic
  nvic_enable_irq(NVIC_UART0_IRQ);
  nvic_set_priority(NVIC_UART0_IRQ,0x10);

  //configure uart
  uart_disable(UART0);
  uart_configure(UART0,GPIO1,GPIO2,GPIO_UNCONNECTED,GPIO_UNCONNECTED,UART_BAUD_115200,false);
  uart_enable(UART0);
  UART_INTEN(UART0) |= UART_INTEN_TXDRDY;

  print_line(UART0,"Hello World!");

  while(1)
  {

  }
}

void print_line(uint32_t uart,char* s)
{
  size_t i = 0;
  while(s[i]!= '\0')
  {
    uart_start_tx(uart);
    uart_send(uart,s[i]);
    uart0_tx_ready = false;
    while(uart0_tx_ready == false)
    {

    }

    uart_stop_tx(uart);
    i++;
  }

  uart_start_tx(uart);
  uart_send(uart,'\n');
  uart0_tx_ready = false;
  while(uart0_tx_ready == false)
  {

  }

  uart_stop_tx(uart);


}
