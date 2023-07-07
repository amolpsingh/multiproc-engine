/*****************************************************************
 * bootloader implementation.  all the code you write will be in
 * this file.  <get-code.h> has useful helpers.
 *
 * if you need to print: use boot_putk.  only do this when you
 * are *not* expecting any data.
 */
#include "rpi.h"
#include "get-code.h"
#include "memmap.h"

// wait until:
//   (1) there is data (uart_has_data() == 1): return 1.
//   (2) <timeout> usec expires, return 0.
//
// look at libpi/staff-src/timer.c
//   - call <timer_get_usec()> to get usec
//   - look at <delay_us()> : for how to correctly 
//     wait for <n> microseconds given that the hardware
//     counter can overflow.
static unsigned 
has_data_timeout(unsigned timeout) {
    //boot_todo("has_data_timeout: implement this routine");
    uint32_t s = timer_get_usec();
    while (1) {
	if (uart_has_data() == 1) return 1;
	uint32_t e = timer_get_usec();
	if ((e - s) >= timeout) break;
    }
    return 0;

}

// iterate:
//   - send GET_PROG_INFO to server
//   - call <has_data_timeout(<usec_timeout>)>
//       - if =1 then return.
//       - if =0 then repeat.
//
// NOTE:
//   1. make sure you do the delay right: otherwise you'll
//      keep blasting <GET_PROG_INFO> messages to your laptop
//      which can end in tears.
//   2. rember the green light that blinks on your ttyl device?
//      Its from this loop (since the LED goes on for each 
//      received packet)
//note : usec_timeout is already in milliseconds
static void wait_for_data(unsigned usec_timeout) {
    //boot_todo("wait_for_data: implement this routine");
    boot_put32(GET_PROG_INFO);
    while (has_data_timeout(usec_timeout) == 0) {
        boot_put32(GET_PROG_INFO);
    }
    return;
}

// IMPLEMENT this routine.
//
// Simple bootloader: put all of your code here.
int get_code(uint32_t *code_addr) {
    // 0. keep sending GET_PROG_INFO every 300ms until 
    // there is data: implement this.
    wait_for_data(300 * 1000);

    /****************************************************************
     * Add your code below: 2,3,4,5,6
     */

    // 2. expect: [PUT_PROG_INFO, addr, nbytes, cksum] 
    //    we echo cksum back in step 4 to help debugging.
    //boot_todo("wait for laptop/server response: echo checksum back");
    uint32_t boot_got;
    while ((boot_got = boot_get32()) != PUT_PROG_INFO) {}
    char *addr = (char *)boot_get32();
    uint32_t nbytes = boot_get32();
    uint32_t cksum = boot_get32();

    // 3. If the binary will collide with us, abort with a BOOT_ERROR. 
    // 
    //    for today: assume that code must be below where the 
    //    booloader code gap starts.  make sure both the start and 
    //    end is below <get_code>'s address.
    // 
    //    more general: use address of PUT32 and __PROG_END__ to detect: 
    //    see libpi/memmap and the memmap.h header for definitions.
    //boot_todo("check that binary will not hit the bootloader code");
    if ( (uint32_t)addr + nbytes >= (uint32_t)&PUT32) {
	boot_err(BOOT_ERROR, "binary collided!\n");	
    }

    // 4. send [GET_CODE, cksum] back.
    //boot_todo("send [GET_CODE, cksum] back\n");
    boot_put32(GET_CODE);
    boot_put32(cksum);

    // 5. we expect: [PUT_CODE, <code>]
    //  read each sent byte and write it starting at 
    //  <addr> using PUT8
    //
    // common mistake: computing the offset incorrectly.
    //boot_todo("boot_get8() each code byte and use PUT8() to write it to memory");
    while ( (boot_got = boot_get32()) != PUT_CODE ) {}
    char *put_addr = addr;
    for (int i = 0; i < nbytes; i++) {
    	uint8_t boot_got8 = uart_get8();
    	PUT8((uint32_t)put_addr, boot_got8);
	put_addr++;
    }

    // 6. verify the cksum of the copied code using:
    //         boot-crc32.h:crc32.
    //    if fails, abort with a BOOT_ERROR.
    //boot_todo("verify the checksum of copied code");
    if (crc32(addr, nbytes) != cksum) {
        boot_err(BOOT_ERROR, "cksum wrong!\n");
    }

    // 7. send back a BOOT_SUCCESS!
    boot_putk("UART Teddy Zhang: success: Received the program!");
    //boot_todo("fill in your name above");

    // woo!
    boot_put32(BOOT_SUCCESS);

    // We used to have these delays to stop the unix side from getting 
    // confused.  I believe it's b/c the code we call re-initializes the 
    // uart.  Instead we now flush the hardware tx buffer.   If this
    // isn't working, put the delay back.  However, it makes it much faster
    // to test multiple programs without it.
    // delay_ms(500);
    uart_flush_tx();

    *code_addr = (uint32_t)addr;
    return 1;
}
