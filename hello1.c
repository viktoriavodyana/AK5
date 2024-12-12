#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include <linux/list.h>
#include "hello1.h"

MODULE_AUTHOR("Victoria");
MODULE_DESCRIPTION("Hello1 module");
MODULE_LICENSE("Dual BSD/GPL");

struct hello_info {
  struct list_head node;
  ktime_t start_time;
  ktime_t end_time;
};

static LIST_HEAD(hello_storage);

void print_hello(unsigned int count)
{
  unsigned int idx;
  struct hello_info *info;

  for (idx = 0; idx < count; idx++) {
    info = kmalloc(sizeof(*info), GFP_KERNEL);
    if (!info) {
      pr_err("Failed to allocate memory\n");
      return;
    }

    info->start_time = ktime_get();
    pr_info("Hello, world!\n");
    info->end_time = ktime_get();

    list_add_tail(&info->node, &hello_storage);
  }
}
EXPORT_SYMBOL(print_hello);

static int __init hello1_module_init(void)
{
  pr_info("hello1 module initialized\n");
  return 0;
}

static void __exit hello1_module_exit(void)
{
  struct hello_info *info, *next;
  s64 duration_ns;

  list_for_each_entry_safe(info, next, &hello_storage, node) {
    duration_ns = ktime_to_ns(ktime_sub(info->end_time, info->start_time));
    pr_info("Duration for print: %lld ns\n", duration_ns);
    list_del(&info->node);
    kfree(info);
  }

  pr_info("hello1 module exited\n");
}

module_init(hello1_module_init);
module_exit(hello1_module_exit);

