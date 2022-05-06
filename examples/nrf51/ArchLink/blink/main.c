/*
Description: Blink program for ArchBlink dev board with nrf51822 using libopencm3
https://github.com/finnurtorfa/nrf51/blob/master/lib/nrf51sdk/Nordic/nrf51822/Board/nrf6310/rtc_example/main.c

Pinout: GPIO1 => UART0_TX
GPIO2 => UART0_RX
GPIO30 => LED

*/

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

#define PORT_LED GPIO
#define PIN_LED GPIO30
#define RTC0_PRESC 0
#define RTC0_COMPARE 32767

volatile bool toggle_led = false;
volatile bool uart0_tx_ready = true;
void print_line(uint32_t uart,char* s);


int main(void)
{
  //Configure nvic
  nvic_enable_irq(NVIC_UART0_IRQ);
  nvic_set_priority(NVIC_UART0_IRQ,0x10);

  nvic_enable_irq(NVIC_RTC0_IRQ);
  nvic_set_priority(NVIC_RTC0_IRQ,0x20);

  //configure uart
  uart_disable(UART0);
  uart_configure(UART0,GPIO1,GPIO2,GPIO_UNCONNECTED,GPIO_UNCONNECTED,UART_BAUD_115200,false);
  uart_enable(UART0);
  UART_INTEN(UART0) |= UART_INTEN_TXDRDY;
  //Configure led pin

  gpio_mode_setup(PORT_LED, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, PIN_LED);
  gpio_set(PORT_LED, PIN_LED);

  //Configure LFCLK
  clock_set_lfclk_src(CLOCK_LFCLK_SRC_XTAL);
  RTC_INTEN(RTC0) |= RTC_INTEN_COMPARE(0);
  clock_start_lfclk(true);
  //
  //
  // //Configure RTC

  rtc_set_prescaler(RTC0,RTC0_PRESC);
  rtc_set_compare(RTC0,0,RTC0_COMPARE);
  //  //rtc_clear(RTC0);
  // //
  rtc_start(RTC0);

  print_line(UART0,"Blink test");

  while(1)
  {

    #ifdef SIMPLE_BLINK
    for (int i = 0; i < LITTLE_BIT; i++) {
      __asm__("nop");
    }
    gpio_toggle(PORT_LED, PIN_LED);
    #endif

    if(toggle_led)
    {
      print_line(UART0,"blink!");
      toggle_led = false;
      gpio_toggle(PORT_LED, PIN_LED);
    }
  }
}

//RTC0 interrupt service routine
//

void rtc0_isr(void)
{

  if(RTC_EVENT_COMPARE(RTC0,0))
  {
    rtc_clear(RTC0);
    RTC_EVENT_COMPARE(RTC0,0) = 0;
    toggle_led = true;
  }

}

void uart0_isr(void)
{
  if(UART_EVENT_TXDRDY(UART0))
  {
    /* The interrupt flag isn't cleared by hardware, we have to do it. */
    UART_EVENT_TXDRDY(UART0) = 0;
    uart0_tx_ready = true;
  }
  //UART_INTENCLR(UART0) = UART_INTEN_TXDRDY;
  //gpio_toggle(PORT_LED, PIN_LED);
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
