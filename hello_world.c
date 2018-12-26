/************************************************************************************************
FileName : hello_world.c
Author	 : MILIN PATEL 
TeamLead: Rajesh Dommaraju 
Details  : It is a simple HelloWorld driver to load a module in the kernel, 
passing the parameters, update parameter from command line, 
get notification message as well for it, insert file operation and
cdev structure.  
License  : SpanIdea Systems Pvt. Ltd. All rights reserved.
 ************************************************************************************************/

/*******************************************************************************
  INCLUDES
 *******************************************************************************/
//#include<linux/kernel.h>
//#include<linux/init.h>
#include<linux/module.h>
#include <linux/fs.h>		//included to resolve the error of major-minor functions and MKDEV,MAJOR,MINOR as well
#include <linux/device.h>	//include to resolve the error for class_create(), device_create(), class_destroy(), device_destroy() func.
#include <linux/cdev.h>		//include to resolve the error for cdev_init(), cdev_add(), cdev_del() func.
#include <linux/slab.h>		//include to resolve the error for kmalloc()
#include <linux/uaccess.h>	//copy_to_user(), copy_from_user()
#include <linux/ioctl.h>	//ioctl
#include <linux/proc_fs.h>	//include to resolve the error for proc_create()
/*******************************************************************************
  LOCAL MACROS		
 *******************************************************************************/

#define U_MODULE_LICENSE	"GPL"
#define U_MODULE_AUTHOR		"MILIN@SPANIDEA"
#define U_MODULE_DESCRIPTION	"IOCTL"
#define U_MODULE_VERSION	"0:1.5"

//#define MAJOR_MINOR_STATIC
#define MAJOR_MINOR_DYNAMIC	
#define DEVICE_NAME 		"stone_victor"
#define DEVICE_CLASS_NAME	"stone_class"
#define DEVICE_FILE_NAME	"stone_dev"
#define PROC_ENTRY		"stone_proc"
#define MEM_SIZE		1024
#define LKM_WR_VALUE		_IOW('.', '.', int32_t*)
#define LKM_RD_VALUE		_IOR('.', '@', int32_t*)

/*---------------static/dynamic major minor number allocation-----------------*/
#ifdef MAJOR_MINOR_DYNAMIC
dev_t dev = 0;
#endif
#ifdef MAJOR_MINOR_STATIC
dev_t dev = MKDEV(738, 0);
#endif

/*----------------------------------------------------------------------------*/
static struct class *dev_class;
static struct cdev my_dev;
/*---------------------------Module parameters--------------------------------*/
int valueETX, arr_valueETX[4];//1, 2
char *nameETX;//3
int cb_valueETX = 0;//4
uint8_t *mem_buffer = NULL;
int byte = 0;
int32_t value = 0;
char array[20] = "try_proc_array\n";
static int len = 1;

module_param(valueETX, int, S_IWUSR |S_IRUSR);//1
module_param_array(arr_valueETX, int, NULL, S_IWUSR |S_IRUSR);//2
module_param(nameETX, charp, S_IWUSR |S_IRUSR);//3

/*---------------------------Module_param_cb()--------------------------------*/
int notify_param(const char *val, const struct kernel_param *kp)
{
	int res = param_set_int(val, kp); // Use helper for write variable
	if(res==0)
	{
		printk(KERN_INFO "val = %s", val);
		printk(KERN_INFO "Call back function called...\n");
		printk(KERN_INFO "New value of cb_valueETX = %d\n", cb_valueETX);
		return 0;
	}
	else
	{
		return -1;

	}
}
const struct kernel_param_ops my_param_ops = 
{
	.set = &notify_param, // Use our setter ...[int(*set)(const char *val, const struct kernel_param *kp)]
	.get = &param_get_int, // .. and standard getter[int(*get)(char *buffer, const struct kernel_param *kp)]
	//[void(*free)(void *arg)]
};
module_param_cb(cb_valueETX, &my_param_ops, &cb_valueETX,S_IWUSR |S_IRUSR);//4
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


static int     sample_module_init(void);
static void    sample_module_exit(void);
static int     sample_module_open(struct inode *inode, struct file *filep);
static int     sample_module_release(struct inode *inode, struct file *filep);
static ssize_t sample_module_read(struct file *filep, char __user *buff, size_t len, loff_t *off);
static ssize_t sample_module_write(struct file *filep, const char __user *buff, size_t len, loff_t *off);

static long    sample_module_ioctl(struct file *filep, unsigned int command_name, unsigned long arg);

static int     sample_proc_module_open(struct inode *inode, struct file *file);
static int     sample_proc_module_release(struct inode *inode, struct file *file);
static ssize_t sample_proc_module_read(struct file *filep, char __user *buff, size_t len, loff_t *off);
static ssize_t sample_proc_module_write(struct file *filep, const char *buff, size_t len, loff_t *off);

static struct file_operations my_ops = {
	.owner   	= THIS_MODULE,
	.open    	= sample_module_open,
	.read    	= sample_module_read,
	.write   	= sample_module_write,
	.unlocked_ioctl = sample_module_ioctl,
	.release 	= sample_module_release,  
};

static struct file_operations proc_fops = {
	.open		= sample_proc_module_open,
	.release	= sample_proc_module_release,
	.read		= sample_proc_module_read,
	.write		= sample_proc_module_write
};
/**********************************************************************************************
function	 : sample_module_init
description	 : This function is initialised when module gets inserted.
This function is passed as an argument to module_init.
input param      : void
return param     : signed integer
 **********************************************************************************************/

static int sample_module_open(struct inode *inode, struct file *filep)
{
	//	if((mem_buffer = (uint8_t *)kmalloc(MEM_SIZE, GFP_KERNEL)) == NULL)
	//	{
	//		printk(KERN_INFO "failed to allocate memmory\n");
	//		return -1;
	//	}	
	//	printk(KERN_INFO "memory allocated at %p\n", mem_buffer);
	printk(KERN_INFO "%s function has been called...\n", __FUNCTION__);
	return 0;
}

static int sample_module_release(struct inode *inode, struct file *filep)
{
	kfree((const void *)mem_buffer);
	printk(KERN_INFO "%s has been called...\n", __func__);
	return 0;
}

static ssize_t sample_module_read(struct file *filep, char __user *buff, size_t len, loff_t *off)
{
	if(len > MEM_SIZE)
	{
		printk(KERN_WARNING "user data size is more than memory while reading!!\n");
	}else;
	if((byte = copy_to_user(buff, mem_buffer, len)) > 0)	//no need of typecasting at buff
	{
		printk(KERN_INFO "%d bytes could not be coppied...\n", byte);
	}
	else
	{
		printk(KERN_INFO "data read done!\n");
	}
	printk(KERN_INFO "%s has been called...\n", __func__);
	return len;
}

static ssize_t sample_module_write(struct file *filep, const char __user *buff, size_t len, loff_t *off)
{
	if((mem_buffer = (uint8_t *)kmalloc(len, GFP_KERNEL)) == NULL)
	{
		printk(KERN_INFO "failed to allocate memmory\n");
		return -1;
	}	
	if(len > MEM_SIZE)
	{
		printk(KERN_WARNING "user data size is more than memory while writing!!\n");
	}else;
	if ((byte = copy_from_user(mem_buffer, buff, len)) > 0)		//no need of typecasting at buff
	{
		printk(KERN_INFO "%d bytes could not be coppied...\n", byte);

	}
	else
	{
		printk(KERN_INFO "data write done!\n");
	}
	printk(KERN_INFO "%s has been called...\n", __func__);
	return len;
}

static int sample_proc_module_open(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "%s called...", __func__);
	return 0;
}

static int sample_proc_module_release(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "%s called...", __func__);
	return 0;
}

static ssize_t sample_proc_module_read(struct file *filep, char __user *buff, size_t length, loff_t *off)
{
	printk(KERN_INFO "proc file read.....\n");
	if(len)
		len=0;
	else{
		len=1;
		return 0;
	}
	copy_to_user(buff,array,20);

	return length;
}

static ssize_t sample_proc_module_write(struct file *filep, const char *buff, size_t len, loff_t *off)
{
	printk(KERN_INFO "proc file wrote.....\n");
	copy_from_user(array,buff,len);
	return len;
}

static long sample_module_ioctl(struct file *filep, unsigned int command_name, unsigned long arg)
{
	printk(KERN_INFO "command_name = %d in %s function...\n", command_name, __func__);
	switch(command_name) {
		case LKM_WR_VALUE:
			copy_from_user(&value ,(int32_t*) arg, sizeof(value));
			printk(KERN_INFO "Value = %d\n", value);
			break;
		case LKM_RD_VALUE:
			copy_to_user((int32_t*) arg, &value, sizeof(value));
			break;
	}
	return 0;
}

static int __init sample_module_init(void)
{
	int i=0;
#ifdef MAJOR_MINOR_DYNAMIC
	if(alloc_chrdev_region(&dev, 0, 1, DEVICE_NAME) == 0)	//allocate major-minor dynamically
#endif
		//#elif
#ifdef MAJOR_MINOR_STATIC
		if(register_chrdev_region(dev, 1, DEVICE_NAME) == 0)	//allocate major-minor statically
#endif
		{
#ifdef MAJOR_MINOR_STATIC
			printk(KERN_INFO "Static allocating of major-minor success\n");
#endif
#ifdef MAJOR_MINOR_DYNAMIC
			printk(KERN_INFO "Dynamic allocating of major-minor success\n");
#endif
			printk(KERN_INFO "Major = %d,Minor  = %d\n",MAJOR(dev), MINOR(dev));
		}
		else
		{
#ifdef MAJOR_MINOR_STATIC
			printk(KERN_INFO "Static allocating of major-minor failed\n");
#endif
#ifdef MAJOR_MINOR_DYNAMIC
			printk(KERN_INFO "Dynamic allocating of major-minor failed\n");
#endif

		}
	/*Creating cdev structure*/
	cdev_init(&my_dev, &my_ops);

	if(cdev_add(&my_dev, dev, 1) < 0)
	{
		printk(KERN_INFO "cdev_add function call failed");
		unregister_chrdev_region(dev, 1);
		return -1;
	}
	else
	{
		printk(KERN_INFO "cdev_add function call success");
	}
	dev_class = class_create(THIS_MODULE, DEVICE_CLASS_NAME);
	if(dev_class == NULL)
	{
		printk(KERN_INFO "Cannot create the struct class for device\n");
		unregister_chrdev_region(dev,1);
		return -1;

	}
	else
		printk(KERN_INFO "struct class creation success\n");
	if(device_create(dev_class, NULL, dev, NULL, DEVICE_FILE_NAME) == NULL)
	{
		//device_create function call will return struct device pointer...
		printk(KERN_INFO "Cannot create the Device\n");
		class_destroy(dev_class);
		return -1;
	}
	else
		printk(KERN_INFO "device file creation success\n");

	/*Creating Proc entry*/
	proc_create(PROC_ENTRY, 0666, NULL, &proc_fops);

	printk(KERN_INFO "ValueETX = %d  \n", valueETX);
	printk(KERN_INFO "cb_valueETX = %d  \n", cb_valueETX);
	printk(KERN_INFO "NameETX = %s \n", nameETX);
	for (i = 0; i < (sizeof arr_valueETX / sizeof (arr_valueETX[0])); i++)
	{
		printk(KERN_INFO "Arr_value[%d] = %d\n", i, arr_valueETX[i]);
	}
	printk(KERN_INFO "Init module Initialization Success at %s ,%d\n", __func__, __LINE__);
	return 0;
}

/**********************************************************************************************
function	 : sample_module_exit
description	 : This function is initialised when module gets removed.
This function is passed as an argument to module_exit.
input param      : void
return param     : void
 **********************************************************************************************/

static void __exit sample_module_exit(void)
{
	remove_proc_entry(PROC_ENTRY,NULL);
	device_destroy(dev_class, dev);		//remove device-file
	class_destroy(dev_class);		//destroy struct class
	cdev_del(&my_dev);
	unregister_chrdev_region(dev, 1);	//unregister major-minor
	printk(KERN_INFO "unreg of module successfully\n");
	printk(KERN_INFO "Exiting exit module success at %s ,%d\n", __func__, __LINE__);
	return;
}

module_init(sample_module_init);
module_exit(sample_module_exit);

MODULE_LICENSE(U_MODULE_LICENSE);
MODULE_AUTHOR(U_MODULE_AUTHOR);
MODULE_DESCRIPTION(U_MODULE_DESCRIPTION);
MODULE_VERSION(U_MODULE_VERSION);
