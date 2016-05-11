#include <stdio.h>
#include "cpuid.h"


int main() {
    unsigned int max_cpuid, max_ext_cpuid;
    char vendor_string[13];
    cpuid_1_info info1;

    max_cpuid = cpuid_0(vendor_string, &max_ext_cpuid);

    printf("Max value CPUID accepts:  %u (0x%02X)\n", max_cpuid, max_cpuid);
    printf("Max value CPUID accepts for extended information:  %u (0x%02X)\n",
        max_ext_cpuid, max_ext_cpuid);
    printf("\n");

    if (max_cpuid < 4) {
        printf("ERROR:  CPUID must support at least 4\n");
        return 1;
    }

    printf("Vendor string:  \"%s\"\n", vendor_string);

    cpuid_1(&info1);

    printf("Processor family ID:  %d    Model:  %d    Stepping ID:  %d\n",
        info1.family_id, info1.model, info1.stepping_id);
    printf("Brand index:  %d    %s\n", info1.brand_index,
        get_brand_string(info1.brand_index));
    printf("\n");

    printf("Instruction cache line size:  %d dwords (%d bytes)\n",
        info1.clflush_line_size, info1.cache_line_size);
    printf("\n");

    printf("Number of cores in the package:  %d\n", num_cores_in_package());
    printf("Max addressable IDs for logical processors in the package:  %d\n",
        info1.max_addressable_ids);
    printf("APIC:  %s\n", info1.flag_apic ? "yes" : "no");
    printf("x2APIC:  %s\n", info1.flag_x2apic ? "yes" : "no");
    printf("Initial APIC ID:  %d\n", info1.initial_apic_id);
    printf("Package supports multiple threads:  %s\n",
        info1.flag_multithreading ? "yes" : "no");
    printf("\n");

    enumerate_caches();

    return 0;
}


