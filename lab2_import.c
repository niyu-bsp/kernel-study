#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>

//知识点:
extern int chardev_test(void);

static int __init import_init(void)
{
    printk(KERN_INFO "###%s %d\n", __func__, __LINE__);
    chardev_test();
    return 0;
}

static void __exit import_exit(void)
{
    printk(KERN_INFO "###%s %d\n", __func__, __LINE__);
}

module_init(import_init)
module_exit(import_exit)
MODULE_LICENSE("GPL");