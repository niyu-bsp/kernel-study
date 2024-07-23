#include <linux/module.h>

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/cdev.h>

#include <linux/fs.h>  // alloc_chardev_regsion

#include<linux/device.h>

static void work_func(struct work_struct *dummy)
{
	pr_info("####%s:%d ok\n", __func__, __LINE__);
}
static DECLARE_WORK(mywork, work_func);

static struct workqueue_struct * my_workqueue = NULL;

static int __init chardev_init(void)
{
   
   
    printk(KERN_INFO "####%s:%d\n", __func__, __LINE__);
    
    
    //中断处理中中执行
    //方法1: 加入全局队列
    //schedule_work(&mywork);
    //方法2：加入自己的队列
    my_workqueue = create_workqueue("my_workqueue");
    queue_work(my_workqueue, &mywork);
    
    return 0;
}


static void __exit chardev_exit(void)
{
    printk(KERN_INFO "####%s:%d\n", __func__, __LINE__);
    destroy_workqueue(my_workqueue);
}
module_init(chardev_init)
module_exit(chardev_exit)

MODULE_LICENSE("GPL");