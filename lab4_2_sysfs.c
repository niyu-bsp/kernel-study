#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>
#include <linux/compiler_types.h>
#include <linux/kconfig.h>

static int value = 0;
static ssize_t my_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    int ret = 0;
    //  printk(KERN_INFO "%s:%d [%s] len=%ld",__func__, __LINE__, buf, n);
    ret = sprintf(buf, "%d", value);
	return ret;
}

static ssize_t my_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t n)
{
    int ret = 0;
    // echo 5 > /sys/kernel/test/attr1    buf:5 加一个换行，n=2,ret=1.所以直接回 
    ret = sscanf(buf, "%d",&value);
    printk(KERN_INFO "%s:%d [%s] len=%ld ret=%d",__func__, __LINE__, buf, n, ret);  

	return n;
}

struct kobject * myobj = NULL;

const struct kobj_attribute myattribute = __ATTR(attr1, 0644, my_show, my_store);
static int __init chardev_init(void)
{
    int ret = 0;
    printk(KERN_INFO "####%s:%d\n", __func__, __LINE__);
    myobj = kobject_create_and_add("test", kernel_kobj);
    ret = sysfs_create_file(myobj, &(myattribute.attr));
    return 0;
}

static void __exit chardev_exit(void)
{
    printk(KERN_INFO "####%s:%d\n", __func__, __LINE__);
    sysfs_remove_file(myobj, &(myattribute.attr));
    kobject_put(myobj);
    
    
}
module_init(chardev_init)
module_exit(chardev_exit)

MODULE_LICENSE("GPL");
