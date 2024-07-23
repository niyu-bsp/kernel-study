#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/fs.h>  // alloc_chardev_regsion

#include<linux/device.h>

static void work_func(struct work_struct *dummy)
{
	pr_info("####%s:%d ok\n", __func__, __LINE__);
}
//方法1:
// static DECLARE_WORK(mywork, work_func);

//方法2:
static struct work_struct mywork; //需要执行INIT_WORK


static int __init chardev_init(void)
{
    printk(KERN_INFO "####%s:%d\n", __func__, __LINE__);
    
    INIT_WORK(&mywork, work_func);   
    //中断处理中中执行
    schedule_work(&mywork);
    
    
    return 0;
}


static void __exit chardev_exit(void)
{
    printk(KERN_INFO "####%s:%d\n", __func__, __LINE__);

}
module_init(chardev_init)
module_exit(chardev_exit)

MODULE_LICENSE("GPL");