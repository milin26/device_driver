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

Cdev structure operations:
1. struct cdev *my_dev;, struct file_operations *my_ops;
2. void cdev_init(struct cdev *cdev, struct file_operations *fops);
3. int cdev_add(struct cdev *dev, dev_t num, unsigned int count);
4. void cdev_del(struct cdev *dev);

cdev structure:
struct cdev { 
    struct kobject kobj; 
    struct module *owner; 
    const struct file_operations *ops; 
    struct list_head list; 
    dev_t dev; 
    unsigned int count; 
};

file_operations structure:
struct file_operations {
    struct module *owner;
    loff_t (*llseek) (struct file *, loff_t, int);
    ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
    ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
    ssize_t (*read_iter) (struct kiocb *, struct iov_iter *);
    ssize_t (*write_iter) (struct kiocb *, struct iov_iter *);
    int (*iterate) (struct file *, struct dir_context *);
    int (*iterate_shared) (struct file *, struct dir_context *);
    unsigned int (*poll) (struct file *, struct poll_table_struct *);
    long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);
    long (*compat_ioctl) (struct file *, unsigned int, unsigned long);
    int (*mmap) (struct file *, struct vm_area_struct *);
    int (*open) (struct inode *, struct file *);
    int (*flush) (struct file *, fl_owner_t id);
    int (*release) (struct inode *, struct file *);
    int (*fsync) (struct file *, loff_t, loff_t, int datasync);
    int (*fasync) (int, struct file *, int);
    int (*lock) (struct file *, int, struct file_lock *);
    ssize_t (*sendpage) (struct file *, struct page *, int, size_t, loff_t *, int);
    unsigned long (*get_unmapped_area)(struct file *, unsigned long, unsigned long, unsigned long, unsigned long);
    int (*check_flags)(int);
    int (*flock) (struct file *, int, struct file_lock *);
    ssize_t (*splice_write)(struct pipe_inode_info *, struct file *, loff_t *, size_t, unsigned int);
    ssize_t (*splice_read)(struct file *, loff_t *, struct pipe_inode_info *, size_t, unsigned int);
    int (*setlease)(struct file *, long, struct file_lock **, void **);
    long (*fallocate)(struct file *file, int mode, loff_t offset,
              loff_t len);
    void (*show_fdinfo)(struct seq_file *m, struct file *f);
#ifndef CONFIG_MMU
    unsigned (*mmap_capabilities)(struct file *);
#endif
    ssize_t (*copy_file_range)(struct file *, loff_t, struct file *,
            loff_t, size_t, unsigned int);
    int (*clone_file_range)(struct file *, loff_t, struct file *, loff_t,
            u64);
    ssize_t (*dedupe_file_range)(struct file *, u64, u64, struct file *,
            u64);
};

Important fields of file_operations structure for us as of now:
1. struct module *owner:
2. ssize_t (*read) (struct file *, char _ _user *, size_t, loff_t *);
3  ssize_t (*write) (struct file *, const char _ _user *, size_t, loff_t *);
4. int (*ioctl) (struct inode *, struct file *, unsigned int, unsigned long);
5. int (*open) (struct inode *, struct file *);
6. int (*release) (struct inode *, struct file *);

#structure where we will register event handler:
#const struct kernel_param_ops my_param_ops = 
#{
#        .set = &notify_param, // Use our setter ...[int(*set)(const char *val, struct kernem_param *kp)]
#        .get = &param_get_int, // .. and standard getter[int(*get)(char *buffer, const struct kernel_param *kp )]
#	.free = //[void(*free)(void *args)]
#};

#Directory/file which are important:
#1. /dev - 	   	   will have the device file, by using "ls -l" command we can get the major-minor number
#2. /proc/devices - 	   driver with major number 
#3. /proc/modules - 	   will give module list,lsmod will also give the list of module
#4. /sys/module -   	   will have our module entry under it when we use module_param();
#5. /sys/devices/virtual - It hold the created struct class directory, under this directory device file(the file name which used in module)
#			   named directory will be there. 
#Important command:
#1. dmesg - 		   dmesg | tail -10, will get the kernel message, we can also see these messages under /var/log/syslog file as well.
#2. echo value > filepath - to change the value of parameter we will use it, can be use to write in device-node as well.

#Header file being used till now:
#1. linux/module.h - For all previous module only this header is enough.
#2. linux/fs.h     - for MKDEV, MAJOR, MINOR, register_chrdev_region(), unregister_chrdev_region(), alloc_chrdev_region().
#3. linux/device.h - Automatic creation of device file func, 
#4. linux/cdev.h   - Include to resolve the error for cdev_init(), cdev_add(), cdev_del() func.
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
