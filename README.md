# device_driver
#init_tutorial

#File : hello_world.c
#Description :
#		This file is a simple Hello World module. 
#		Contains basic module_init and module_exit functions.
#		Contains module parameter passing mechanism.
#		Contains major minor concept.
		
#Steps to compile :
#1.Run				make
#2.Insert module			sudo insmod hello_world.ko
#3.See the added module		cat /proc/modules | grep hello, lsmod(See the list of module)
#4.See the logs in dmesg 	dmesg | tail -10
#  kernel ring buffer	
#5.Remove module			sudo rmmod hello
#6.Run				make clean


#Keypoints to remember:
#1.static int hello_init();	user defined function
#2.void hello_exit();		user defined function
#3.module_init();		will pass the init function
#4.module_exit();		will pass the exit function

#Macros for module parameters:
#1.module_param();
#2.module_param_array();
#3.module_param_cb();

#Allocating Major-Minor number:
#1.statically allocating : int register_chrdev_region(dev_t dev, unsigned int first, char *name);.
#2. Dynamically allocating : int alloc_chrdev_region(dev_t *dev, unsigned int firstminor, unsigned int count, char *name);.
#3.Unregister major-minor number : void unregister_chrdev_region(dev_t first, unsigned int count);.
#4.To create major-minor structure(dev_t) : MKDEV(int major, int minor);.
#5.To get major-minor number : MAJOR(dev_t dev), and MINOR(dev_t dev);.

#Creating device file manually:
#1. mknod -m <permissions> <name> <dev_type> <major> <minor> - permissions and -m flag are optional can be modified by chmod afterwards, 
							      name should be the absolute path,
							      dev_type could be either c(character device file) or b(block device file)
							      major and minor whould be the major, minor number assigned to driver
#2. rm filename - To remove device file created manually

#Creating device file automatically:
#1. struct class * class_create(struct module * owner, const char * name) - owner > THIS_MODULE
									   name > name of this class
#2. struct device * device_create(struct class *, struct device *parent, dev_t dev, void * drvdata, const char *fmt, ...)
#		- struct class * > address of struct class which is returned from class_create API
#		- struct device *parent > pointer to the parent struct device of this new device, if any
#		- dev_t dev > value of dev(major-minor number)
#		- void *drvdata > Unknown
#		- const char *fmt > string for device name
		- ... > variable argument
#3. void class_destroy(struct class*);
#4. void device_destroy(struct class *, dev_t dev);
 
#structure where we will register event handler:
#const struct kernel_param_ops my_param_ops = 
#{
#        .set = &notify_param, // Use our setter ...[int(*set)(const char *val, struct kernem_param *kp)]
#        .get = &param_get_int, // .. and standard getter[int(*get)(char *buffer, const struct kernel_param *kp )]
#	.free = //[void(*free)(void *args)]
#};

#Directory/file which are important:
#1. /dev - 	   will have the device file, by using "ls -l" command we can get the major-minor number
#2. /proc/devices - driver with major number 
#3. /proc/modules - will give module list,lsmod will also give the list of module
#4. /sys/module -   will have our module entry under it when we use module_param();

#Important command:
#1. dmesg - 		   dmesg | tail -10, will get the kernel message, we can also see these messages under /var/log/syslog file as well.
#2. echo value > filepath - to change the value of parameter we will use it.

#Header file being used till now:
#1. linux/module.h - For all previous module only this header is enough.
#2. linux/fs.h     - for MKDEV, MAJOR, MINOR, register_chrdev_region(), unregister_chrdev_region(), alloc_chrdev_region().
#3. linux/device.h - Automatic creation of device file func, 

#Note : 
#	Make sure before you run "make" command be super user using "sudo -i".
#	param_set_int(val, kp); // Use helper for write variable
#	To create device file struct class is required.
#	__init and __exit: This demonstrates a feature of kernel 2.2 and later. 
#			   Notice the change in the definitions of the init and cleanup functions.
#			   The __init macro causes the init function to be discarded and its memory freed once the init function finishes for built-in drivers,
#			   but not loadable modules. If you think about when the init function is invoked, this makes perfect sense.

#			   There is also an __initdata which works similarly to __init but for init variables rather than functions.

#			   The __exit macro causes the omission of the function when the module is built into the kernel, and like __exit, has no effect for loadable modules.
#			   Again, if you consider when the cleanup function runs, this makes complete sense; built-in drivers don't need a cleanup function, while loadable modules do.
