/// Syntacore SCR* infra
///
/// @copyright (C) Syntacore 2015-2019. All rights reserved.
/// @author mn-sc
///
/// @brief UART defs and inline funcs

#ifndef SC1_FPGA_UART_H
#define SC1_FPGA_UART_H

#include "platform_config.h"

#define PLF_UART_BAUDRATE 115200

// FPGA UART ports
#define SC1F_UART0_PORT 0xff010000

#define SC1F_UART_RXD       (0x00) // receive data
#define SC1F_UART_TXD       (0x04) // transmit data
#define SC1F_UART_STATUS    (0x08) // status register
#define SC1F_UART_CONTROL   (0x0c) // control register
#define SC1F_UART_DIV       (0x10) // baud rate divisor register

// UART control register bits        // enable interrupt for
#define SC1F_UART_CTL_IE    (1 << 8) // exception
#define SC1F_UART_CTL_IRRDY (1 << 7) // read ready
#define SC1F_UART_CTL_ITRDY (1 << 6) // transmision ready
#define SC1F_UART_CTL_ITMT  (1 << 5) // transmitter shift reg empty
#define SC1F_UART_CTL_ITOE  (1 << 4) // transmitter overrun
#define SC1F_UART_CTL_IROE  (1 << 3) // receiver overrun
#define SC1F_UART_CTL_IBRK  (1 << 2) // break detect
#define SC1F_UART_CTL_ITE   (1 << 1) // framing error
#define SC1F_UART_CTL_IPE   (1 << 0) // parity error

// UART status register bits
#define SC1F_UART_ST_E      (1 << 8) // 1: error, 0: ok
#define SC1F_UART_ST_RRDY   (1 << 7) // 1: exit data, 0: no data
#define SC1F_UART_ST_TRDY   (1 << 6) // 1: can trans, 0: busy

#ifndef __ASSEMBLER__

#include <stdint.h>

// uart low level i/o
static inline void sc1f_uart_write(uintptr_t uart_base, unsigned reg, uint16_t val)
{
    *(volatile uint32_t *)(uart_base + reg) = val;
}

static inline uint16_t sc1f_uart_read(uintptr_t uart_base, unsigned reg)
{
    return *(volatile uint32_t *)(uart_base + reg);
}

// inlines

static inline int sc1f_uart_rx_ready(void)
{
    return sc1f_uart_read(SC1F_UART0_PORT, SC1F_UART_STATUS) & SC1F_UART_ST_RRDY;
}

static inline int sc1f_uart_tx_ready(void)
{
    return sc1f_uart_read(SC1F_UART0_PORT, SC1F_UART_STATUS) & SC1F_UART_ST_TRDY;
}

static inline void sc1f_uart_tx_flush(void)
{
    while ((sc1f_uart_read(SC1F_UART0_PORT, SC1F_UART_STATUS) & SC1F_UART_ST_TRDY) == 0);
}

static inline void sc1f_uart_put(uint8_t v)
{
    while (!sc1f_uart_tx_ready());
    sc1f_uart_write(SC1F_UART0_PORT, SC1F_UART_TXD, v);
}

static inline int sc1f_uart_getch_nowait(void)
{
    if (sc1f_uart_rx_ready())
        return sc1f_uart_read(SC1F_UART0_PORT, SC1F_UART_RXD);
    return -1; // no input
}

static inline int sc1f_uart_getch(void)
{
    int c;

    do {
        c = sc1f_uart_getch_nowait();
    } while (c == -1);

    return c;
}

// complex uart init/io
void sc1f_uart_init(void);
void uart_print_info(void);
int sc1f_uart_putchar(int v);
void uart_puthex4(uint8_t val);
void uart_puthex8(uint8_t val);
void uart_puthex16(uint16_t val);
void uart_puthex32(uint32_t val);
void uart_puthex64(uint64_t val);
void uart_putdec(unsigned long v);
void uart_puts(const char *s);
unsigned long uart_read_hex(void);
unsigned uart_read_str(char *buf, unsigned size);
// inlines
static inline void uart_puthex(unsigned long val)
{
#if __riscv_xlen > 64
    uart_puthex32(val >> 32*3);
    uart_puthex32(val >> 32*2);
#endif
#if __riscv_xlen > 32
    uart_puthex32(val >> 32);
#endif
    uart_puthex32(val);
}
static inline int uart_putc(int c)
{
    return sc1f_uart_putchar(c);
}

static inline int uart_getc(void)
{
    return sc1f_uart_getch();
}

int console_putc(int ch) __attribute__((weak, alias("uart_putc")));
int console_getc(void) __attribute__((weak, alias("uart_getc")));
void scr_uart_init(void) __attribute__((weak, alias("sc1f_uart_init")));


#endif // __ASSEMBLER__

#endif // SC1_FPGA_UART_H
