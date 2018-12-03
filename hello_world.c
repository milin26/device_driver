/************************************************************************************************
FileName : simple_driver.c
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

#include <linux/module.h>
/*******************************************************************************
			 LOCAL MACROS		
*******************************************************************************/

#define U_MODULE_LICENSE	"GPL"
#define U_MODULE_AUTHOR		"MILIN@SPANIDEA"
#define U_MODULE_DESCRIPTION	"MODULE PARAMETERS PASSING"
#define U_MODULE_VERSION	"0:1.1"

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

static int sample_module_init(void)
{
	int i=0;
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

static void sample_module_exit(void)
{
	printk(KERN_INFO "Exiting exit module success at %s ,%d\n", __func__, __LINE__);
	return;
}

module_init(sample_module_init);
module_exit(sample_module_exit);

MODULE_LICENSE(U_MODULE_LICENSE);
MODULE_AUTHOR(U_MODULE_AUTHOR);
MODULE_DESCRIPTION(U_MODULE_DESCRIPTION);
MODULE_VERSION(U_MODULE_VERSION);
