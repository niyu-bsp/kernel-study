#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#define TIMEOUT (5000)
static void func(struct timer_list *unused);
static DEFINE_TIMER(mytimer, func);
static void func(struct timer_list *unused)
{
    pr_info("####%s:%d\n", __func__, __LINE__);
}

static int __init chardev_init(void)
{
    

    printk(KERN_INFO "####%s:%d\n", __func__, __LINE__);
    mod_timer(&mytimer, jiffies + msecs_to_jiffies(TIMEOUT) );
    
    return 0;
}

static void __exit chardev_exit(void)
{
    printk(KERN_INFO "####%s:%d\n", __func__, __LINE__);
}
module_init(chardev_init)
module_exit(chardev_exit)

MODULE_LICENSE("GPL");
