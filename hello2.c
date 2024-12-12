#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include "hello1.h"

MODULE_AUTHOR("Your Name <your.email@example.com>");
MODULE_DESCRIPTION("Hello2 module");
MODULE_LICENSE("Dual BSD/GPL");

static unsigned int hello_count = 1;
module_param(hello_count, uint, 0444);
MODULE_PARM_DESC(hello_count, "Number of times to print 'Hello, world!'");

static int __init hello2_init(void)
{
    if (hello_count > 10) {
        printk(KERN_EMERG "Error: hello_count too large (%d > 10)\n", hello_count);
        return -EINVAL;
    }

    if (hello_count == 0 || hello_count >= 5) {
        printk(KERN_EMERG "Warning: hello_count = %d "
               "(hello_count == 0 or 5 <= hello_count <= 10)\n", hello_count);
    }

    print_hello(hello_count);

    return 0;
}

static void __exit hello2_exit(void)
{
    printk(KERN_INFO "hello2 module unloaded\n");
}

module_init(hello2_init);
module_exit(hello2_exit);

