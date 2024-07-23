#include <linux/module.h>

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/cdev.h>

#include <linux/fs.h>  // alloc_chardev_regsion

#include<linux/device.h>
#include <linux/kthread.h>
#include <linux/interrupt.h>

static dev_t devt;
static struct class* myclass;
static struct cdev mycdev;
static ssize_t  my_read (struct file *filep, char __user * buf, size_t len, loff_t * off)
{
    printk(KERN_INFO "####%s:%d ok\n", __func__, __LINE__);
    struct irq_desc *desc;
    desc = irq_to_desc(11);
    if (!desc) 
    {
        return -EINVAL;
    }
    __this_cpu_write(vector_irq[59], desc);
    asm("int $0x38");
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
    //  mycondition = 1;
    //  wake_up(&my_waitqueue);
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
static irqreturn_t my_irq_handle(int irq, void *pdev)
{
    printk(KERN_INFO "####%s:%d\n", __func__, __LINE__);
	// int port = (irq == IXP4XX_GPIO_IRQ(GPIO_HSS1_DCD_N));
	// int i = gpio_get_value(port ? GPIO_HSS1_DCD_N : GPIO_HSS0_DCD_N);
	// set_carrier_cb_tab[port](pdev, !i);
	return IRQ_HANDLED;
}
#define IRQ_NUM  (11)
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
    
    ret = request_irq(IRQ_NUM, my_irq_handle,IRQF_SHARED, "myirq", NULL);
    if(ret)
    {
        printk(KERN_INFO "request_irq failed\n");
    }
    else
    {
        printk(KERN_INFO "request_irq succusful\n");
    }
    
    return 0;
}


static void __exit chardev_exit(void)
{
    printk(KERN_INFO "####%s:%d\n", __func__, __LINE__);

    free_irq(IRQ_NUM, NULL);
    // kthread_stop(mytask);
    cdev_del(&mycdev);
    device_destroy(myclass, devt); 
    class_destroy(myclass); 
    unregister_chrdev_region(devt,5);

}
module_init(chardev_init)
module_exit(chardev_exit)

MODULE_LICENSE("GPL");