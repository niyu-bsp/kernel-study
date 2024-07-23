#include <linux/module.h>

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/cdev.h>

static int __init chardev_init(void)
{
    printk(KERN_INFO "####%s:%d\n", __func__, __LINE__);
    return 0;
}
// 知识点: 测试EXPORT_SYMBOL
int chardev_test(void)
{
    int i = 0;
    for(i = 0; i< 5 ; i++)
    {
        printk(KERN_INFO "####%s:%d\n", __func__, __LINE__);
    }
    return 0;
}

static void __exit chardev_exit(void)
{
    printk(KERN_INFO "####%s:%d\n", __func__, __LINE__);
}
module_init(chardev_init)
module_exit(chardev_exit)

MODULE_LICENSE("GPL");
EXPORT_SYMBOL(chardev_test);
/*
    int register_chrdev_region(dev_t first, unsigned int count, char *name);
    int alloc_chrdev_region   (dev_t *dev, unsigned int firstminor, unsigned int count, char *name);
*   void unregister_chrdev_region(dev_t first, unsigned int count);
    mknod  -m 0644  /dev/test c 242 0 (需要完整路径) //device_create() 中默认根目录在/dev/
*/