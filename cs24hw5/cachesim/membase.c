#include <assert.h>

#include "membase.h"


/* Returns nonzero if the input is a power of 2, or zero otherwise. */
uint32_t is_power_of_2(uint32_t n) {
    assert(n > 0);

    return !(n & (n - 1));
}


/* For a given n that is a power of 2, where 2^m = n, this function
 * returns m.
 */
uint32_t log_2(uint32_t n) {
    uint32_t m = 0;
    assert(is_power_of_2(n));
    while (n > 0) {
        n >>= 1;
        m++;
    }
    return m - 1;
}


/* Reads a byte from a specific memory address in the simulated memory. */
unsigned char read_byte(membase_t *mb, addr_t address) {
    return mb->read_byte(mb, address);
}


/* Writes a byte to a specific memory address in the simulated memory. */
void write_byte(membase_t *mb, addr_t address, unsigned char value) {
    mb->write_byte(mb, address, value);
}


/* This struct is used by read_float and write_float so that it can use the
 * read_int and write_int implementations.
 */
typedef union {
    uint32_t ival;
    float fval;
} convert_t;


/* Reads a signed integer from a specific index in the memory.  The index is
 * multiplied by 4 to determine the address to read the int from.  The value
 * is stored in little-endian format, as IA32 normally does.
 */
int32_t read_int(membase_t *mb, uint32_t index) {
    addr_t addr = index * 4;
    return read_byte(mb, addr) |
           read_byte(mb, addr + 1) <<  8 |
           read_byte(mb, addr + 2) << 16 |
           read_byte(mb, addr + 3) << 24;
}


/* Writes a signed integer to a specific index in the memory.  The index is
 * multiplied by 4 to determine the address to write the int to.  The value
 * is stored in little-endian format, as IA32 normally does.
 */
void write_int(membase_t *mb, uint32_t index, int32_t value) {
    addr_t addr = index * 4;
    write_byte(mb, addr, value & 0xFF);
    write_byte(mb, addr + 1, (value >>  8) & 0xFF);
    write_byte(mb, addr + 2, (value >> 16) & 0xFF);
    write_byte(mb, addr + 3, (value >> 24) & 0xFF);
}


/* Reads a float from a specific index in the memory.  The index is multiplied
 * by 4 to determine the address to read the float from.  The value is stored
 * in little-endian format, as IA32 normally does.
 */
float read_float(membase_t *mb, uint32_t index) {
    convert_t v;
    v.ival = read_int(mb, index);
    return v.fval;
}


/* Writes a float to a specific index in the memory.  The index is multiplied
 * by 4 to determine the address to write the float to.  The value is stored
 * in little-endian format, as IA32 normally does.
 */
void write_float(membase_t *mb, uint32_t index, float value) {
    convert_t v;
    v.fval = value;
    write_int(mb, index, v.ival);
}


/* This function can be used to emulate a hardware clock e.g. for tagging cache
 * lines in order to implement an LRU replacement policy when evicting cache
 * lines.  Every call to the function advances the clock by one tick, and then
 * returns the new value of the clock.
 */
uint64_t clock_tick() {
    static uint64_t clock = 0;

    return ++clock;
}

