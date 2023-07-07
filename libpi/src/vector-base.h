#ifndef __VECTOR_BASE_SET_H__
#define __VECTOR_BASE_SET_H__
#include "libc/bit-support.h"
#include "asm-helpers.h"
<<<<<<< HEAD
#include <stdio.h>
=======
>>>>>>> 2d44cd1047f9ea9ed3c3e00a23188520d0aa1dd5

/*
 * vector base address register:
 *   3-121 --- let's us control where the exception jump table is!
 *
 * defines: 
 *  - vector_base_set  
 *  - vector_base_get
 */

// return the current value vector base is set to.
<<<<<<< HEAD
// MCR p15, 0, <Rd>, c12, c0, 0
static inline void *vector_base_get(void) {
    void *out;
    asm volatile ("MRC p15, 0, %0, c12, c0, 0": "=r" (out) ::);
    prefetch_flush();
    return out;
=======
static inline void *vector_base_get(void) {
    void *base;
    asm volatile("MRC p15, 0, %0, c12, c0, 0" : "=r" (base));
    return base;
>>>>>>> 2d44cd1047f9ea9ed3c3e00a23188520d0aa1dd5
}

// check that not null and alignment is good.
static inline int vector_base_chk(void *vector_base) {
<<<<<<< HEAD
    unsigned mask = 0b11111;
    if ((unsigned)vector_base == 0) return 0;
    // if ((unsigned)vector_base %64 == 0) return 0;
    if (((unsigned)vector_base & mask) != 0) return 0;
    
    return 1;
}

=======
    if ((unsigned)(vector_base) == 0) return 0;
    if (((unsigned)vector_base & 0b11111) != 0) return 0;
    return 1;
}
>>>>>>> 2d44cd1047f9ea9ed3c3e00a23188520d0aa1dd5
// set vector base: must not have been set already.
static inline void vector_base_set(void *vec) {
    if(!vector_base_chk(vec))
        panic("illegal vector base %p\n", vec);

    void *v = vector_base_get();
    if(v) 
        panic("vector base register already set=%p\n", v);
<<<<<<< HEAD
    
    asm volatile ("MCR p15, 0, %0, c12, c0, 0": : "r" (vec));
    prefetch_flush();
    // todo("set vector base");
=======
    asm volatile ("MCR p15, 0, %0, c12, c0, 0" : : "r" (vec));

>>>>>>> 2d44cd1047f9ea9ed3c3e00a23188520d0aa1dd5

    v = vector_base_get();
    if(v != vec)
        panic("set vector=%p, but have %p\n", vec, v);
}

// reset vector base and return old value: could have
// been previously set.
static inline void *
vector_base_reset(void *vec) {
    if(!vector_base_chk(vec))
        panic("illegal vector base %p\n", vec);

    void *old_vec = 0;
<<<<<<< HEAD

    old_vec = vec;

    asm volatile ("MCR p15, 0, %0, c12, c0, 0": : "r" (vec));

=======
    old_vec = vector_base_get();
    //asm volatile ("MRC p15, 0, %0, c12, c0, 0" : "=r" (old_vec));
    // vector_base_set(vec);
    asm volatile ("MCR p15, 0, %0, c12, c0, 0" : : "r" (vec));
   // printk("%x\n", old_vec);
    // printk(vector_base_get());
    // todo("check that <vec> is good reset vector base");
>>>>>>> 2d44cd1047f9ea9ed3c3e00a23188520d0aa1dd5
    assert(vector_base_get() == vec);
    return old_vec;
}
#endif
