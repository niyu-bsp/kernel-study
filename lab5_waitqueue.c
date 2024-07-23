#include <linux/module.h>

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/cdev.h>

#include <linux/fs.h>  // alloc_chardev_regsion

#include<linux/device.h>
#include <linux/kthread.h>
static struct task_struct * mytask;
static int mycondition = 0;
DECLARE_WAIT_QUEUE_HEAD(my_waitqueue);
static dev_t devt;
static struct class* myclass;
static struct cdev mycdev;
static ssize_t  my_read (struct file *filep, char __user * buf, size_t len, loff_t * off)
{
    printk(KERN_INFO "####%s:%d ok\n", __func__, __LINE__);
    return 0;
}
static ssize_t  my_write (struct file *filep, const char __user * buf,size_t len, loff_t * off)
{   
    printk(KERN_INFO "####%s:%d ok\n", __func__, __LINE__);
    return 0;
}
static int my_open (struct inode *i, struct file *f)
{    
     printk(KERN_INFO "####%s:%d ok\n", __func__, __LINE__);
     mycondition = 1;
     wake_up(&my_waitqueue);
     return 0;
}
static int my_release (struct inode *i, struct file *f)
{
    
    printk(KERN_INFO "####%s:%d\n", __func__, __LINE__);
    return 0;
}

struct file_operations myops =
{
    .owner = THIS_MODULE,
    .open  = my_open,
    .release = my_release,
    .read = my_read,
    .write = my_write,
};
static int mythread(void *data)
{
    static int index = 0;
     printk(KERN_INFO "####%s:%d ok\n", __func__, __LINE__);

     while(1)
     {
        printk(KERN_INFO "####%s:%d  index=%d befor\n", __func__, __LINE__, index);
        wait_event(my_waitqueue, mycondition == 1);
        printk(KERN_INFO "####%s:%d  index=%d after\n", __func__, __LINE__, index);
        mycondition = 0;
        if( index == 3)
        {
            break;
        }
        index ++;
     }
     printk(KERN_INFO "####%s:%d  index=%d end\n", __func__, __LINE__, index);
     return 0;
}
static int __init chardev_init(void)
{
   
    int ret = 0;
    struct device *mydevice;
    printk(KERN_INFO "####%s:%d\n", __func__, __LINE__);
    // 知识点1: 分配 major minor 
    ret = alloc_chrdev_region(&devt, 0,5, "test2");
    // ret = register_chrdev_region(MKDEV(55,0),5, "test");
    
    if( ret == 0)
    {
        printk(KERN_INFO "xx_chrdev_region successful major %d minor:%d\n", MAJOR(devt), MINOR(devt));
    }
    else
    {
        printk(KERN_INFO "xx_chrdev_region failed\n");
    }
    
    // 知识点2: 为主次编号 创建字符设备  可以使用mknod  -m 0644 /root/test c 242 0 代替
    //          device_create 的默认目录是/dev/; mknod 的路径可以变更
    myclass = class_create(THIS_MODULE,"myclass"); 
    mydevice = device_create(myclass,NULL,devt,NULL, "%s", "test1");

    // 知识点3: 为主次编号 创建操作函数
    cdev_init(&mycdev, &myops);
    cdev_add(&mycdev, devt, 1);
    mytask = kthread_create(mythread, NULL, "mythread");
    if(mytask)
    {
        wake_up_process(mytask);
    }
    
    return 0;
}


static void __exit chardev_exit(void)
{
    printk(KERN_INFO "####%s:%d\n", __func__, __LINE__);

    kthread_stop(mytask);
    cdev_del(&mycdev);
    device_destroy(myclass, devt); 
    class_destroy(myclass); 
    unregister_chrdev_region(devt,5);

}
module_init(chardev_init)
module_exit(chardev_exit)

MODULE_LICENSE("GPL");