#include <linux/module.h>
//#include <linux/init.h>
#define U_MODULE_LICENSE	"GPL"
#define U_MODULE_AUTHOR		"MILIN@SPANIDEA"
#define U_MODULE_DESCRIPTION	"INITIAL HELLO WORLD MODULE"
#define U_MODULE_VERSION	"0:1.0"

static int sample_module_init(void)
{
	printk(KERN_INFO "Initialize init module: %s ,%d\n", __func__, __LINE__);
	return 0;
}

void sample_module_exit(void)
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
