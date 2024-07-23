#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>
LIST_HEAD(head);
struct mynode{
    struct list_head list;
    int value;
};

static int __init chardev_init(void)
{
    int i = 0; 
    struct mynode *node = NULL;
    printk(KERN_INFO "####%s:%d\n", __func__, __LINE__);
    for(i = 0; i < 5; i++)
    {
        node = kzalloc(sizeof(struct mynode), GFP_KERNEL);
        // INIT_LIST_HEAD(&node->list);
        node->value = i;
        list_add_tail(&node->list, &head);
    }

    list_for_each_entry(node, &head, list)
        pr_info("value:%d\n", node->value);
    
    return 0;
}

static void __exit chardev_exit(void)
{
    printk(KERN_INFO "####%s:%d\n", __func__, __LINE__);

    
    
}
module_init(chardev_init)
module_exit(chardev_exit)

MODULE_LICENSE("GPL");
