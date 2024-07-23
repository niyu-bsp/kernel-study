#include <linux/module.h>

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/cdev.h>

#include <linux/fs.h>  // alloc_chardev_regsion

#include<linux/device.h>


#include<linux/uaccess.h>
#include<linux/slab.h>

#include <linux/ioctl.h>
static dev_t devt;
static struct class* myclass;
static struct cdev mycdev;

struct task_struct *task = NULL;
struct kernel_siginfo info;
static ssize_t  my_read (struct file *filep, char __user * buf, size_t len, loff_t * off)
{
    printk(KERN_INFO "####%s:%d \n", __func__, __LINE__);
    memset(&info, 0, sizeof(struct kernel_siginfo));
    info.si_signo = SIGUSR1;
    info.si_code = SI_QUEUE;
 
    if (task != NULL) {
        printk(KERN_INFO "Sending signal to app\n");
        if(send_sig_info(SIGUSR1, &info, task) < 0) {
            printk(KERN_INFO "Unable to send signal\n");
        }
    }
    return 0;
}

static int my_open (struct inode *i, struct file *f)
{    
     
     task = get_current();
     pr_info("####%s:%d PID:%d\n",__func__, __LINE__, task->pid);
     return 0;
}
static int my_release (struct inode *i, struct file *f)
{
    
    printk(KERN_INFO "####%s:%d\n", __func__, __LINE__);
    return 0;
}

static ssize_t  my_write (struct file *filep, const char __user * buf,size_t len, loff_t * off)
{   ssize_t ret = 0;
   
    printk(KERN_INFO "####%s:%d ret=%ld len= %ld\n", __func__, __LINE__, ret,len);
    return ret;
}

struct file_operations myops =
{
    .owner = THIS_MODULE,
    .open  = my_open,
    .read = my_read,
    .write = my_write,
    .release = my_release
};
static int __init chardev_init(void)
{
   
    int ret = 0;
    struct device *mydevice;

    printk(KERN_INFO "####%s:%d\n", __func__, __LINE__);
    // 知识点1: 分配 major minor 
    ret = alloc_chrdev_region(&devt, 0,5, "test2");
    // ret = register_chrdev_region(MKDEV(55,0),5, "test");
    
    
    // 知识点2: 为主次编号 创建字符设备  可以使用mknod  -m 0644 /root/test c 242 0 代替
    //          device_create 的默认目录是/dev/; mknod 的路径可以变更
    myclass = class_create(THIS_MODULE,"myclass"); 
    mydevice = device_create(myclass,NULL,devt,NULL, "%s", "test1");

    // 知识点3: 为主次编号 创建操作函数
    cdev_init(&mycdev, &myops);
    cdev_add(&mycdev, devt, 1);
    

    
    return 0;
}


static void __exit chardev_exit(void)
{
    printk(KERN_INFO "####%s:%d\n", __func__, __LINE__);
    cdev_del(&mycdev);
    device_destroy(myclass, devt); 
    class_destroy(myclass); 
    unregister_chrdev_region(devt,5);

}
module_init(chardev_init)
module_exit(chardev_exit)

MODULE_LICENSE("GPL");
