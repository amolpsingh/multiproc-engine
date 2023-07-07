// implement:
//  void uart_init(void)
//
//  int uart_can_get8(void);
//  int uart_get8(void);
//
//  int uart_can_put8(void);
//  void uart_put8(uint8_t c);
//
//  int uart_tx_is_empty(void) {
//
// see that hello world works.
//
<<<<<<< HEAD
// enable and disable: AUX_MU_CNTL_REG pg 17: set to 1 to enable
// can TX? space_available in AUX_MU_STAT_REG 
// can RX? symbol available in AUX_MU_STAT_REG
// tx empty? bit 9 of AUX_MU_STAT_REG and Transmitter idle in AUX_MU_LSR_REG
// clear? 2:1 in page 13
// data to read & location to transmit data: AUX_MU_IO_REG 7:0 pg 11

// Give three different register / fields to check if there is at least one byte in the RX FIFO.
// IER, LSR, STAT
// Give three different register / fields to check if there is at least one byte of space in the TX FIFO.
// 
// Write the expression to read a byte from the RX FIFO (assume you don't have to check if data is there)

// Write the expression to write a byte to the TX FIFO (assume you don't have to check if there is space for more data).

// What value do you write to what register (and its address) to clear the RX and TX FIFOs?

// What register and value to write to set the UART data size to 8-bits.

// Compute the value to write to the AUX_MU_BAUD register to set the UART baudrate to 115200. Show your work solving for this value. Assume a 250Mhz clock (250,000,000 cycles per second).

#include "rpi.h"
#include "gpio.c"

#define AUX_ENABLES 0x20215004
#define AUX_MU_IO_REG 0x20215040
#define AUX_IER_REG 0x20215044
#define AUX_IIR_REG 0x20215048
#define AUX_MU_LCR_REG 0x2021504C
#define AUX_MU_LSR_REG 0x20215054
#define AUX_MU_CNTL_REG 0x20215060
#define AUX_MU_STAT_REG 0x20215064
#define AUX_MU_BAUD 0x20215068
#define TX_PIN 14
#define RX_PIN 15
// called first to setup uart to 8n1 115200  baud,
// no interrupts.
//  - you will need memory barriers, use <dev_barrier()>
// enable aux to disable uart
//  later: should add an init that takes a baud rate.
void uart_init(void) {
    dev_barrier();

    //turn on pins and enable correctly
    gpio_set_function(TX_PIN, GPIO_FUNC_ALT5);
    gpio_set_function(RX_PIN, GPIO_FUNC_ALT5);

    //enable aux
    uint32_t curr_enb = GET32(AUX_ENABLES);
    PUT32(AUX_ENABLES, curr_enb|1);

    dev_barrier();

    //disable the transmitter and reciever
    uint32_t curr_cnt = GET32(AUX_MU_CNTL_REG);
    PUT32(AUX_MU_CNTL_REG, curr_cnt&(~3));
    // PUT32(AUX_MU_CNTL_REG, 0);

    dev_barrier();

    //disable interrupts
    uint32_t curr_ier = GET32(AUX_IER_REG);
    PUT32(AUX_IER_REG, curr_ier&(~3));

    dev_barrier();

    //clear fifo queues
    uint32_t curr_iir = GET32(AUX_IIR_REG);
    PUT32(AUX_IIR_REG, 0b110);

    dev_barrier();

    //write in baud rate
    uint32_t curr_lcr = GET32(AUX_MU_LCR_REG);
    PUT32(AUX_MU_LCR_REG, curr_lcr|0b11);
    PUT32(AUX_MU_BAUD, 270);

    //enable the tx and rx
    uint32_t curr_cntl = GET32(AUX_MU_CNTL_REG);
    PUT32(AUX_MU_CNTL_REG, curr_cntl|(3));

}

// disable the uart.
void uart_disable(void) {
    //disable aux
    uart_flush_tx();

    // uint32_t curr_cntl = GET32(AUX_MU_CNTL_REG);
    // PUT32(AUX_MU_CNTL_REG, curr_cntl&(~3));

    //set first enable pin to 0
    uint32_t curr_enb = GET32(AUX_ENABLES);
    PUT32(AUX_ENABLES, curr_enb&(~1));
=======
//
#include "rpi.h"


// called first to setup uart to 8n1 115200  baud,
// no interrupts.
//  - you will need memory barriers, use <dev_barrier()>
//
//  later: should add an init that takes a baud rate.
/*
unsigned value = GET32(GPIO_BASE + offset);
    unsigned mask = 0b111 << ((pin % 10) * 3);
    value &= ~mask;
    value |= 0x00000001 << ((pin % 10) * 3);
    PUT32(GPIO_BASE + offset, value);

    congifugre gpio pins
    configure uart 
    nuke all registers we dont use

*/
void uart_init(void) {
    dev_barrier();
    // configure gpio 
    gpio_set_function(14, GPIO_FUNC_ALT5);
    gpio_set_function(15, GPIO_FUNC_ALT5);
    // enable aux
    dev_barrier();
    unsigned auxenb = 0x20215004;
    unsigned value = GET32(auxenb);  
    unsigned mask = 0b1;
    value &= ~mask;
    value |= 0x1;
    PUT32(auxenb, value);
    dev_barrier();

    // disable txdr and rxdr
    unsigned auxmucntlreg = 0x20215060;
    PUT32(auxmucntlreg, 0b00);


    // clear receiver
    unsigned auxmuiirreg = 0x20215048;
    PUT32(auxmuiirreg, 0b110);
  
    // disable interrupts
    unsigned auxmuierreg = 0x20215044;
    PUT32(auxmuierreg, 0b00);
 

    // set data size bit mode
    unsigned auxmulcrreg = 0x2021504C;
    PUT32(auxmulcrreg, 0b11);



    // set baud
    unsigned auxmubaudreg = 0x20215068;
    PUT32(auxmubaudreg, 270);

    
    // enable tx, rx
    auxmucntlreg = 0x20215060;
    PUT32(auxmucntlreg, 0b11);
    dev_barrier(); 


    // how to enable it -> rx seee if data there, tx space empty
    // flush - wait for all data sent
    // what about other pins or env? 
    // any time you write to two deices, but a dev barrier
    /*
    high four digits are not the right ones
    there is a gap 
    before u can use miniuart must set auxenb 
    auxenb will be zero if reset 
    ENABLE/disable - cntl regma
    can tx / transmit - lsr transmitter empty, 
    stat reg 1 space available 
    can rx - stat reg bit 0 
    completely empty and done - bit 9 stat reg, transmitter done
    flushing tx rx - iir reg 2:1 
    aux read modify write
    dev barrier 
    enable
    read / write low bite io_reg 
    how to set the baud -> baudreg -> 270 pg 19 

    */ 
}
void uart_flush_tx(void); 
// disable the uart.
void uart_disable(void) {
    dev_barrier();
    uart_flush_tx(); 
    unsigned auxenb = 0x20215004;
    unsigned value = GET32(auxenb);  
    unsigned mask = 0b1;
    value &= ~mask;
    PUT32(auxenb, value);   
    dev_barrier();
>>>>>>> 2d44cd1047f9ea9ed3c3e00a23188520d0aa1dd5
}


// returns one byte from the rx queue, if needed
// blocks until there is one.
int uart_get8(void) {
<<<<<<< HEAD
    uint32_t val;
    // while((val = GET32(AUX_MU_CNTL_REG)&1) == 0) continue;
    while(!uart_has_data()) continue;
    uint32_t curr_io = GET32(AUX_MU_IO_REG);
    return (curr_io & 0xff);
=======
    dev_barrier();
    while (!uart_has_data()) {
        continue; 
    }
    unsigned auxmuioreg = 0x20215040;
    unsigned value1 = GET32(auxmuioreg);
    return (value1 & 0xFF);

>>>>>>> 2d44cd1047f9ea9ed3c3e00a23188520d0aa1dd5
}

// 1 = space to put at least one byte, 0 otherwise.
int uart_can_put8(void) {
<<<<<<< HEAD
    uint32_t curr_cntl_trans_enb = GET32(AUX_MU_STAT_REG)&0b10;
    if (curr_cntl_trans_enb == 0b10) {
        return 1;
    }
    return 0;
=======
    dev_barrier();
    unsigned auxmustatreg = 0x20215064;
    unsigned value = GET32(auxmustatreg);
    return (value & 0b10) >> 1;
  
>>>>>>> 2d44cd1047f9ea9ed3c3e00a23188520d0aa1dd5
}

// put one byte on the tx qqueue, if needed, blocks
// until TX has space.
// returns < 0 on error.
int uart_put8(uint8_t c) {
    dev_barrier();
<<<<<<< HEAD
    while (!uart_can_put8()) continue;
    uint32_t curr_io = GET32(AUX_MU_IO_REG);
    //clear last 8 bits
    curr_io &= (~0xff);
    //turn on message
    curr_io |= c;
    PUT32(AUX_MU_IO_REG,curr_io);
    return curr_io;
}

// simple wrapper routines useful later.
// 1 = at least one byte on rx queue, 0 otherwise
int uart_has_data(void) {
    uint32_t val = GET32(AUX_MU_STAT_REG)&1;
    return (val == 1);
=======
    while (!uart_can_put8()) {
        ;
    }
    unsigned auxmuioreg = 0x20215040;
    PUT32(auxmuioreg, c);
    return 1;
    dev_barrier();
    // how do we error check it? 
}

// simple wrapper routines useful later.

// 1 = at least one byte on rx queue, 0 otherwise
int uart_has_data(void) {
    dev_barrier();
    unsigned auxmustatreg = 0x20215064;
    return (GET32(auxmustatreg) & 0b1);

>>>>>>> 2d44cd1047f9ea9ed3c3e00a23188520d0aa1dd5
}

// return -1 if no data, otherwise the byte.
int uart_get8_async(void) { 
<<<<<<< HEAD
    if(!uart_has_data())
        return -1;
    return uart_get8();
=======
    dev_barrier();
    if(!uart_has_data())
        return -1;
    return uart_get8();
 
>>>>>>> 2d44cd1047f9ea9ed3c3e00a23188520d0aa1dd5
}

// 1 = tx queue empty, 0 = not empty.
int uart_tx_is_empty(void) {
<<<<<<< HEAD
    uint32_t curr_lsr = GET32(AUX_MU_STAT_REG);
    return ((curr_lsr>>9)&1) == 1;
=======
    dev_barrier();
    unsigned auxmustatreg = 0x20215064;
    unsigned value = GET32(auxmustatreg);
    unsigned value_check = value & (0b1 << 9);
    return value_check >> 9; 
   
>>>>>>> 2d44cd1047f9ea9ed3c3e00a23188520d0aa1dd5
}

// flush out all bytes in the uart --- we use this when 
// turning it off / on, etc.
void uart_flush_tx(void) {
<<<<<<< HEAD
    while(!uart_tx_is_empty()){
    }
=======
    dev_barrier();
    while(!uart_tx_is_empty()) {
        ;
    }
        
>>>>>>> 2d44cd1047f9ea9ed3c3e00a23188520d0aa1dd5
}
