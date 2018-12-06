/************************************************************************************************
FileName : hello_world.c
Author	 : MILIN PATEL 
TeamLead: Rajesh Dommaraju 
Details  : It is a simple HelloWorld driver to load a module in the kernel, 
	   passing the parameters, update parameter from command line, 
	   get notification message as well for it.  
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
//#include <linux/module.h>
/*******************************************************************************
			 LOCAL MACROS		
*******************************************************************************/

#define U_MODULE_LICENSE	"GPL"
#define U_MODULE_AUTHOR		"MILIN@SPANIDEA"
#define U_MODULE_DESCRIPTION	"MAJOR MINOR NUMBER"
#define U_MODULE_VERSION	"0:1.2"

//#define MAJOR_MINOR_STATIC
#define MAJOR_MINOR_DYNAMIC	
#define DEVICE_NAME 		"stone_victor"
#define DEVICE_CLASS_NAME	"stone_class"
#define DEVICE_FILE_NAME	"stone_dev"
/*******************************************************************************
			 LOCAL TYPEDEFS		
*******************************************************************************/

/*******************************************************************************
			GLOBAL VARIABLES		
*******************************************************************************/

/*******************************************************************************
			LOCAL VARIABLES		
*******************************************************************************/

/*******************************************************************************
			LOCAL FUNCTIONS		
*******************************************************************************/
/*---------------static/dynamic major minor number allocation-----------------*/
#ifdef MAJOR_MINOR_DYNAMIC
	dev_t dev = 0;
#endif
#ifdef MAJOR_MINOR_STATIC
	dev_t dev = MKDEV(738, 0);
#endif

/*----------------------------------------------------------------------------*/
static struct class *dev_class;
/*---------------------------Module parameters--------------------------------*/
int valueETX, arr_valueETX[4];//1, 2
char *nameETX;//3
int cb_valueETX = 0;//4

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


static int sample_module_init(void);
static void sample_module_exit(void);

/**********************************************************************************************
function	 : sample_module_init
description	 : This function is initialised when module gets inserted.
		   This function is passed as an argument to module_init.
input param      : void
return param     : signed integer
**********************************************************************************************/

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

	printk(KERN_INFO "ValueETX = %d  \n", valueETX);
        printk(KERN_INFO "cb_valueETX = %d  \n", cb_valueETX);
        printk(KERN_INFO "NameETX = %s \n", nameETX);
        for (i = 0; i < (sizeof arr_valueETX / sizeof (arr_valueETX[0])); i++)
	{
                printk(KERN_INFO "Arr_value[%d] = %d\n", i, arr_valueETX[i]);
        }
	printk(KERN_INFO "Initialize init module Success at %s ,%d\n", __func__, __LINE__);
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
	device_destroy(dev_class, dev);		//remove device-file
	class_destroy(dev_class);		//destroy struct class
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
