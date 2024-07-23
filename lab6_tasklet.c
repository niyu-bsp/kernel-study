#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
// #define STATIC_METHOD
#ifdef STATIC_METHOD
void myfunc(struct tasklet_struct *arg)
{
    // while(1)   //不能加 死循环
    {
        mdelay(1000);
        printk(KERN_INFO "####%s:%d\n", __func__, __LINE__);
        // mdelay(1000);
    }
}
DECLARE_TASKLET(mytasklet, myfunc);
#else
void myfunc(unsigned long  arg)
{
    // while(1)   //不能加 死循环
    {
        mdelay(1000);
        printk(KERN_INFO "####%s:%d\n", __func__, __LINE__);
        // mdelay(1000);
    }
}
struct tasklet_struct mytasklet;
#endif


static int __init chardev_init(void)
{
    int ret = 0; 
    printk(KERN_INFO "####%s:%d\n", __func__, __LINE__);
#ifdef STATIC_METHOD
#else   
    tasklet_init(&mytasklet, myfunc, 0);
#endif
    tasklet_schedule(&mytasklet);
    printk(KERN_INFO "####%s:%d\n", __func__, __LINE__);
    return 0;
}

static void __exit chardev_exit(void)
{
    printk(KERN_INFO "####%s:%d\n", __func__, __LINE__);
    tasklet_kill(&mytasklet);  //会等待它的完成，然后kill it
   
    
}
module_init(chardev_init)
module_exit(chardev_exit)

MODULE_LICENSE("GPL");
