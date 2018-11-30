/************************************************************************************************
@ileName : simple_driver.c
@author	 : sayli waykar 
@teamLead: Rajesh Dommaraju 
@etails  : It is a simple HelloWorld driver to load a module in the kernel  
@icense  : SpanIdea Systems Pvt. Ltd. All rights reserved.
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
#define U_MODULE_DESCRIPTION	"INITIAL HELLO WORLD MODULE"
#define U_MODULE_VERSION	"0:1.0"

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

static int hello_init(void);
static void hello_exit(void);

/**********************************************************************************************
function	 : sample_module_init
description	 : This function is initialised when module gets inserted.
		   This function is passed as an argument to module_init.
input param      : void
return param     : signed integer
**********************************************************************************************/

static int sample_module_init(void)
{
	printk(KERN_INFO "Initialize init module: %s ,%d\n", __func__, __LINE__);
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
	printk(KERN_INFO "Exiting exit module : %s ,%d\n", __func__, __LINE__);
	return;
}

module_init(sample_module_init);
module_exit(sample_module_exit);

MODULE_LICENSE(U_MODULE_LICENSE);
MODULE_AUTHOR(U_MODULE_AUTHOR);
MODULE_DESCRIPTION(U_MODULE_DESCRIPTION);
MODULE_VERSION(U_MODULE_VERSION);
