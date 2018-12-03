# device_driver
init_tutorial

File : hello.c
Description :
		This file is a simple Hello World module. 
		Contains basic module_init and module_exit functions.
		
Steps to compile :
1.Run				make
2.Insert module			sudo insmod hello.ko
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

structure where we will register event handler:
const struct kernel_param_ops my_param_ops = 
{
        .set = &notify_param, // Use our setter ...[int(*set)(const char *val, struct kernem_param *kp)]
        .get = &param_get_int, // .. and standard getter[int(*get)(char *buffer, const struct kernel_param *kp )]
	.free = //[void(*free)(void *args)]
};


Note : 
	Make sure before you run "make" command be super user using "sudo -i".
	param_set_int(val, kp); // Use helper for write variable

