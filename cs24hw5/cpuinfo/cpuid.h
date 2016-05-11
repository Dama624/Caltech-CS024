#ifndef CPUID_H
#define CPUID_H


typedef struct regs_t {
    unsigned int eax;
    unsigned int ebx;
    unsigned int ecx;
    unsigned int edx;
} regs_t;


const char * get_brand_string(unsigned int brand_index);

unsigned int cpuid_0(char *vendor_string, unsigned int *max_ext_value);

void cpuid_4(unsigned int ecx, regs_t *regs);
void cpuid_n(unsigned int eax, regs_t *regs);

unsigned int num_cores_in_package(void);

typedef struct cpuid_1_info {

    unsigned int ext_family_id;

    unsigned int ext_model_id;

    unsigned int proc_type;

    unsigned int family_id;

    unsigned int model;

    unsigned int stepping_id;

    unsigned int brand_index;

    unsigned int clflush_line_size;

    unsigned int cache_line_size;

    unsigned int max_addressable_ids;

    unsigned int initial_apic_id;

    unsigned int flag_apic;

    unsigned int flag_x2apic;

    unsigned int flag_multithreading;

} cpuid_1_info;


void cpuid_1(cpuid_1_info *info);

void enumerate_caches(void);

#endif /* CPUID_H */

