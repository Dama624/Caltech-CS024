#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cpuid.h"


#define NUM_BRAND_STRINGS 24

static const char * BRAND_STRINGS[] = {
    "(unsupported)",                /* 0x00 */
    "Intel Celeron",                /* 0x01 */
    "Intel Pentium III",            /* 0x02 */
    "Intel Pentium III Xeon",       /* 0x03 */
    "Intel Pentium III",            /* 0x04 */
    NULL,
    "Mobile Intel Pentium III-M",   /* 0x06 */
    "Mobile Intel Celeron",         /* 0x07 */
    "Intel Pentium 4",              /* 0x08 */
    "Intel Pentium 4",              /* 0x09 */
    "Intel Celeron",                /* 0x0A */
    "Intel Xeon",                   /* 0x0B */
    "Intel Xeon MP",                /* 0x0C */
    NULL,
    "Mobile Intel Pentium 4-M",     /* 0x0E */
    "Mobile Intel Celeron",         /* 0x0F */
    NULL,
    "Mobile Genuine Intel",         /* 0x11 */
    "Intel Celeron M",              /* 0x12 */
    "Mobile Intel Celeron",         /* 0x13 */
    "Intel Celeron",                /* 0x14 */
    "Mobile Genuine Intel",         /* 0x15 */
    "Intel Pentium M",              /* 0x16 */
    "Mobile Intel Celeron"          /* 0x17 */
};


const char * get_brand_string(unsigned int brand_index) {
    if (brand_index >= NUM_BRAND_STRINGS)
        return NULL;

    return BRAND_STRINGS[brand_index];
}


void cpuid_1(cpuid_1_info *info) {
    regs_t regs;

    assert(info != NULL);

    /* Call the CPUID operation with eax = 1. */
    cpuid_n(1, &regs);

    info->ext_family_id = (regs.eax >> 20) & 0xFF;
    info->ext_model_id = (regs.eax >> 16) & 0x0F;
    info->proc_type = (regs.eax >> 12) & 0x03;
    info->family_id = (regs.eax >> 8) & 0x0F;
    info->model = (regs.eax >> 4) & 0x0F;
    info->stepping_id = regs.eax & 0x0F; 

    info->brand_index = regs.ebx & 0xFF;
    info->clflush_line_size = (regs.ebx >> 8) & 0xFF;
    info->cache_line_size = info->clflush_line_size * 8;
    info->max_addressable_ids = (regs.ebx >> 16) & 0xFF;
    info->initial_apic_id = (regs.ebx >> 24) & 0xFF;

    info->flag_x2apic = (regs.ecx >> 21) & 1;

    info->flag_apic = (regs.edx >> 9) & 1;
    info->flag_multithreading = (regs.edx >> 28) & 1;
}


unsigned int num_cores_in_package(void) {
    regs_t regs;
    cpuid_4(0, &regs);
    return 1 + ((regs.ebx >> 26) & 0x1F);
}


static const char * CACHE_TYPES[] = {
    "no more caches",
    "data cache",
    "instruction cache",
    "unified cache"
};


void enumerate_caches(void) {
    unsigned int index;
    regs_t regs;

    unsigned int cache_type, cache_level;
    unsigned int cache_proc_ids, package_proc_ids;
    unsigned int line_size, partitions, ways_assoc, n_sets;

    index = 0;
    while (1) {
        cpuid_4(index, &regs);

        cache_type = regs.eax & 0x1F;
        if (cache_type == 0)
            break;

        cache_level = (regs.eax >> 5) & 0x07;

        cache_proc_ids = 1 + ((regs.eax >> 14) & 0x0FFF);
        package_proc_ids = 1 + ((regs.eax >> 26) & 0x3F);

        line_size = 1 + (regs.ebx & 0x0FFF);
        partitions = 1 + ((regs.ebx >> 12) & 0x3FF);
        ways_assoc = 1 + ((regs.ebx >> 22) & 0x3FF);
        n_sets = 1 + regs.ecx;

        printf("Cache index %u:  type %u (%s), level %u, procIDs %u, "
            "totIDs %u\n", index, cache_type, CACHE_TYPES[cache_type],
            cache_level, cache_proc_ids, package_proc_ids);
        printf("    Line size %uB, partitions %u, associativity %u, sets %u\n",
            line_size, partitions, ways_assoc, n_sets);
        printf("    Cache size:  %u bytes\n", line_size * ways_assoc * n_sets);

        index++;
    }
}

