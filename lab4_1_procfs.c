#include <linux/module.h>

#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/proc_fs.h>
#include <linux/uaccess.h>
int	my_open(struct inode *i, struct file *filep)
{
    pr_info("%s:%d\n", __func__, __LINE__);
    return 0;
}
int	my_release(struct inode *i, struct file *filep)
{
    pr_info("%s:%d\n", __func__, __LINE__);
    return 0;
}
static char kbuf[1024] = { "Init" };
static int flag = 1;
ssize_t	my_read(struct file *filep, char __user *buf, size_t len, loff_t *off)
{
    size_t ret = 0;
    if(flag)
    {
        flag=0;
    }
    else
    {
        flag=1;
        return 0;
    }
    
    pr_info("%s:%d len:%ld\n", __func__, __LINE__, len);
    ret = copy_to_user(buf,kbuf,strlen(kbuf));
    pr_info("%s:%d ret:%ld \n", __func__, __LINE__, ret );
    return strlen(kbuf);
}
ssize_t	my_write(struct file *filep, const char __user *buf, size_t len, loff_t *offset)
{
    int ret = 0;
    pr_info("%s:%d len:%ld\n", __func__, __LINE__, len);
    ret = copy_from_user(kbuf,buf,len);
    return len;
}
struct proc_ops ops = {
    .proc_open = my_open,
    .proc_release = my_release,
    .proc_read = my_read,
    .proc_write = my_write,
};
struct proc_dir_entry *my_pro_dir = NULL;
struct proc_dir_entry *my_entry = NULL;

static int __init chardev_init(void)
{
    printk(KERN_INFO "####%s:%d\n", __func__, __LINE__);
    my_pro_dir = proc_mkdir("myprocdir", NULL);
    //知识点:
    my_entry=  proc_create("test", 0666,my_pro_dir, &ops);
    // 其他方式     proc_create_single("cmdline", 0, NULL, cmdline_proc_show);
    //             proc_create_seq("consoles", 0, NULL, &consoles_op);
    return 0;
}


static void __exit chardev_exit(void)
{
    printk(KERN_INFO "####%s:%d\n", __func__, __LINE__);
    proc_remove(my_pro_dir);
}
module_init(chardev_init)
module_exit(chardev_exit)

MODULE_LICENSE("GPL");
