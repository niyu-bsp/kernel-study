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
static int mem_size = 1024;
static char *kern_buff;
static int flag = 1;
static ssize_t  my_read (struct file *filep, char __user * buf, size_t len, loff_t * off)
{
    ssize_t ret = 0;
    if( flag )
    {
        flag = 0;
    }
    else
    {
        flag = 1;
        return 0;
    }
    ret = copy_to_user(buf, kern_buff,len);     // 返回未拷贝成功的字节数，0：success
    printk(KERN_INFO "####%s:%d ret=%ld len=%ld\n", __func__, __LINE__,ret, len);
    return len;
}
static ssize_t  my_write (struct file *filep, const char __user * buf,size_t len, loff_t * off)
{   ssize_t ret = 0;
    ret = copy_from_user(kern_buff, buf, len); // 返回未拷贝成功的字节数，0：success
    printk(KERN_INFO "####%s:%d ret=%ld len= %ld\n", __func__, __LINE__, ret,len);
    return ret;
}
static int my_open (struct inode *i, struct file *f)
{    
     printk(KERN_INFO "####%s:%d ok\n", __func__, __LINE__);
     return 0;
}
static int my_release (struct inode *i, struct file *f)
{
    
    printk(KERN_INFO "####%s:%d\n", __func__, __LINE__);
    return 0;
}
/* 知识点 ioctl
#define _IOC(dir,type,nr,size) \   (((0 +8)+8)+14), (0 +8), 0,
	(((dir)  << _IOC_DIRSHIFT) | \              (((0 +8)+8)+14)
	 ((type) << _IOC_TYPESHIFT) | \             (0 +8)
	 ((nr)   << _IOC_NRSHIFT) | \                0
	 ((size) << _IOC_SIZESHIFT))                ((0 +8)+8))
31---30---16---8---0
  dir  size type nr

*/
// 知识点:arg 是用户空间的参数 的地址
struct data_t{
        
        char name[10];
        int age;
};

#define WR_DATA _IOW('a', 1, struct data_t)
#define RD_DATA _IOR('a', 1, struct data_t)
struct data_t data = {0};
static long my_unlock_ioctl (struct file *filep, unsigned int cmd, unsigned long arg)
{
    
    int ret = 0;
    switch(cmd)
    {
        case WR_DATA:
            ret = copy_from_user((void*)&data, (void *)arg, sizeof(struct data_t));
            data.age +=10;
            printk("%s:%d name:%s, age:%d\n", __func__, __LINE__,data.name, data.age);
            break;
        case RD_DATA:
            ret = copy_to_user((void*)arg, (void*)&data, sizeof(struct data_t));
            printk("%s:%d name:%s, age:%d\n", __func__, __LINE__,data.name, data.age);
            break;
        default:
            break;
    }
    return 0;
}
struct file_operations myops =
{
    .owner = THIS_MODULE,
    .open  = my_open,
    .release = my_release,
    .read = my_read,
    .write = my_write,
    .unlocked_ioctl = my_unlock_ioctl,
};
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
    
    // 知识点4:创建内核空间 缓存
    //        GFP全称： Get Free Pages
    kern_buff = (char *)kzalloc(mem_size, GFP_KERNEL);
    if( !kern_buff )
    {
        printk(KERN_INFO "kmalloc failed\n");
        return -1;
    }
    
    return 0;
}


static void __exit chardev_exit(void)
{
    printk(KERN_INFO "####%s:%d\n", __func__, __LINE__);
    kfree(kern_buff);
    cdev_del(&mycdev);
    device_destroy(myclass, devt); 
    class_destroy(myclass); 
    unregister_chrdev_region(devt,5);

}
module_init(chardev_init)
module_exit(chardev_exit)

MODULE_LICENSE("GPL");
/*
    int register_chrdev_region(dev_t first, unsigned int count, char *name);
    int alloc_chrdev_region   (dev_t *dev, unsigned int firstminor, unsigned int count, char *name);
*   void unregister_chrdev_region(dev_t first, unsigned int count);
    mknod  -m 0644  /dev/test c 242 0 (需要完整路径) //device_create() 中默认根目录在/dev/
*/