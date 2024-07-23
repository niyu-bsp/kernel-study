/***************************************************************************//**
*  \file       driver.c
*
*  \details    Simple hello world driver
*
*  \author     EmbeTronicX
*
* *******************************************************************************/
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/moduleparam.h>
/*
** 1. 参数传递
*/
int argc;
char *name;
int list1[10];

module_param(argc, int,   S_IRUSR|S_IWUSR);
module_param(name, charp, S_IRUSR|S_IWUSR);
module_param_array(list1, int,NULL,S_IRUSR|S_IWUSR);

/* 
* 2.参数会调module_param 间接调用 module_param_cb
*/
int my_set(const char *val, const struct kernel_param *kp)
{
    printk(KERN_INFO "## %s %d\n", __func__, __LINE__);
	return kstrtoint(val, 0, (int *)kp->arg);
}

int my_get(char *buffer, const struct kernel_param *kp)
{
    printk(KERN_INFO "## %s %d\n", __func__, __LINE__);
	return scnprintf(buffer, PAGE_SIZE, "%i\n", *(int*)kp->arg);
}

int cb_var1;
const struct kernel_param_ops myops = {
    .get = my_get,
    .set = my_set
};
param_check_int(cb_var1, &(cb_var1));
module_param_cb(cb_var1, &myops, &cb_var1, 0644);\
__MODULE_PARM_TYPE(cb_var1, "int");
// 知识点: module_param 最终调用module_param_cb,回调函数为默认的: param_ops_##type
//          |--module_param_named
//                  |--module_param_cb 
// #define module_param_named(name, value, type, perm)			   \
// 	param_check_##type(name, &(value));				   \
// 	module_param_cb(name, &param_ops_##type, &value, perm);		   \
// 	__MODULE_PARM_TYPE(name, #type)
/* kernel_param_ops end */

static int __init hello_world_init(void)
{
    int i = 0 ;
    printk(KERN_INFO "argc:%d\n", argc);
    printk(KERN_INFO "name:%s\n", name);
    for(; i< sizeof(list1) / sizeof(int); i++)
    {
        printk(KERN_INFO "%d\n", list1[i]);
    }
    printk(KERN_INFO "cb_var1:%d\n", cb_var1);

    printk(KERN_INFO "Kernel Module Inserted Successfully...\n");
    return 0;
}

/*
** Module Exit function
*/
static void __exit hello_world_exit(void)
{
    printk(KERN_INFO "Kernel Module Removed Successfully...\n");
}
 
module_init(hello_world_init);
module_exit(hello_world_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("EmbeTronicX <embetronicx@gmail.com>");
MODULE_DESCRIPTION("A simple hello world driver");
MODULE_VERSION("2:1.0");