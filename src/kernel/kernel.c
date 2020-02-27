#include <stddef.h>
#include <stdint.h>

#include "bios.h"

#include "lnterrupt.h"
#include "memory.h"
#include "printk.h"
#include "string.h"

#include "memory.c"

int main()
{
    bios_clear();
    bios_message("Start InOS successfully\n", OK);
    init_idt();

    init_memory();
    bios_exit();

    return 0;
}
