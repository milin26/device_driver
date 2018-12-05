# device_driver
init_tutorial

File : hello_world.c
Description :
		This file is a simple Hello World module. 
		Contains basic module_init and module_exit functions.
		Contains module parameter passing mechanism.
		Contains major minor concept.
		
Steps to compile :
1.Run				make
2.Insert module			sudo insmod hello_world.ko
3.See the added module		cat /proc/modules | grep hello, lsmod(See the list of module)
4.See the logs in dmesg 	dmesg | tail -10
  kernel ring buffer	
5.Remove module			sudo rmmod hello
6.Run				make clean


Keypoints to remember:
1.static int hello_init();
2.void hello_exit();
3.module_init();
4.module_exit();

Macros for module parameters:
1.module_param();
2.module_param_array();
3.module_param_cb();

Allocating Major-Minor number:
1.statically allocating : int register_chrdev_region(dev_t dev, unsigned int first, char *name);.
2. Dynamically allocating : int alloc_chrdev_region(dev_t *dev, unsigned int firstminor, unsigned int count, char *name);.
3.Unregister major-minor number : void unregister_chrdev_region(dev_t first, unsigned int count);.
4.To create major-minor structure(dev_t) : MKDEV(int major, int minor);.
5.To get major-minor number : MAJOR(dev_t dev), and MINOR(dev_t dev);.

structure where we will register event handler:
const struct kernel_param_ops my_param_ops = 
{
        .set = &notify_param, // Use our setter ...[int(*set)(const char *val, struct kernem_param *kp)]
        .get = &param_get_int, // .. and standard getter[int(*get)(char *buffer, const struct kernel_param *kp )]
	.free = //[void(*free)(void *args)]
};

Directory/file which are important:
1. /dev - 	   will have the device file, by using "ls -l" command we can get the major-minor number
2. /proc/devices - driver with major number 
3. /proc/modules - will give module list,lsmod will also give the list of module
4. /sys/module -   will have our module entry under it when we use module_param();

Important command:
1. dmesg - 		   dmesg | tail -10, will get the kernel message, we can also see these messages under /var/log/syslog file as well.
2. echo value > filepath - to change the value of parameter we will use it.

Header file being used till now:
1. linux/module.h - For all previous module only this header is enough.
2. linux/fs.h     - for MKDEV, MAJOR, MINOR, register_chrdev_region(), unregister_chrdev_region(), alloc_chrdev_region().

Note : 
	Make sure before you run "make" command be super user using "sudo -i".
	param_set_int(val, kp); // Use helper for write variable

