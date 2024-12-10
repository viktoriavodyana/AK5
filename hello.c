#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/slab.h>

MODULE_AUTHOR("Serhii Popovych <serhii.popovych@globallogic.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

struct TimeNode {
  struct TimeNode *next;
  ktime_t timestamp;
};

static unsigned int hello_count = 1;
static struct TimeNode *time_list_head;

static void init_time_list(void)
{
  time_list_head = kmalloc(sizeof(struct TimeNode), GFP_KERNEL);

  time_list_head->next = NULL;
  time_list_head->timestamp = ktime_get();
}

static void append_time_node(void)
{
  struct TimeNode *current_node = time_list_head;
  struct TimeNode *new_node = kmalloc(sizeof(struct TimeNode), GFP_KERNEL);

  while (current_node->next != NULL)
    current_node = current_node->next;

  new_node->next = NULL;
  new_node->timestamp = ktime_get();

  current_node->next = new_node;
}

static void log_time(ktime_t *timestamp)
{
  printk(KERN_EMERG "%lld\n", *timestamp);
}

static void free_time_list(void)
{
  struct TimeNode *current_node = time_list_head;
  struct TimeNode *next_node = NULL;

  while (current_node != NULL) {
    log_time(&(current_node->timestamp));

    next_node = current_node->next;
    kfree(current_node);
    current_node = next_node;
  }
}

static void update_time_list(void)
{
  if (time_list_head == NULL)
    init_time_list();
  else
    append_time_node();
}

static int __init hello_init(void)
{
  int i;

  if (hello_count > 10) {
    printk(KERN_EMERG "Error: hello_count too large (%d > 10)\n", hello_count);
    return -EINVAL;
  }

  if (hello_count == 0 || hello_count >= 5) {
    printk(KERN_EMERG "Warning: hello_count = %d "
        "(hello_count == 0 or 5 <= hello_count <= 10)\n", hello_count);
  }

  for (i = 0; i < hello_count; i++) {
    update_time_list();

    printk(KERN_EMERG "Hello, world!\n");
  }

  return 0;
}

static void __exit hello_exit(void)
{
  free_time_list();
}

module_param(hello_count, uint, 0444);
MODULE_PARM_DESC(hello_count, "Number of 'Hello, world!' outputs on load");

module_init(hello_init);
module_exit(hello_exit);

