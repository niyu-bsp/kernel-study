#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>

int thread_func(void* data)
{
    while(1)
    {
        mdelay(1000);
        printk(KERN_INFO "####%s:%d\n", __func__, __LINE__);
        if( kthread_should_stop())
        {
            break;
        }

    }
    return 0;

}
struct task_struct *mytask = NULL;
static int __init chardev_init(void)
{
    int ret = 0; 
    
    printk(KERN_INFO "####%s:%d\n", __func__, __LINE__);
#if 0
    mytask = kthread_create(thread_func, NULL, "mythread");
    if( !mytask )
    {
        
        pr_info("%s:%d kthread_create failed\n", __func__, __LINE__);
        return -1;
    }
    wake_up_process(mytask);
#else
    mytask = kthread_run(thread_func, NULL, "mythread");
    if( !mytask )
    {
        
        pr_info("%s:%d kthread_create failed\n", __func__, __LINE__);
        return -1;
    }
#endif
    return 0;
}

static void __exit chardev_exit(void)
{
    printk(KERN_INFO "####%s:%d\n", __func__, __LINE__);
    if(mytask)
    {
        kthread_stop(mytask);
    }
    
}
module_init(chardev_init)
module_exit(chardev_exit)

MODULE_LICENSE("GPL");
