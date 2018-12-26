# device_driver
#init_tutorial

------------------------------------------------------------------------------------------------------------------------------------------
#File : hello_world.c
#Description :
#		This file is a simple Hello World module. 
#		Contains basic module_init and module_exit functions.
#		Contains module parameter passing mechanism.
#		Contains major minor concept.
		
------------------------------------------------------------------------------------------------------------------------------------------
#Steps to compile :
#1.Run				make
#2.Insert module			sudo insmod hello_world.ko
#3.See the added module		cat /proc/modules | grep hello, lsmod(See the list of module)
#4.See the logs in dmesg 	dmesg | tail -10
#  kernel ring buffer	
#5.Remove module			sudo rmmod hello
#6.Run				make clean

------------------------------------------------------------------------------------------------------------------------------------------
#Keypoints to remember:
#1.static int hello_init();	user defined function
#2.void hello_exit();		user defined function
#3.module_init();		will pass the init function
#4.module_exit();		will pass the exit function
------------------------------------------------------------------------------------------------------------------------------------------
#Macros for module parameters:
#1.module_param();
#2.module_param_array();
#3.module_param_cb();

------------------------------------------------------------------------------------------------------------------------------------------
#Allocating Major-Minor number:
#1.statically allocating : int register_chrdev_region(dev_t dev, unsigned int first, char *name);.
#2. Dynamically allocating : int alloc_chrdev_region(dev_t *dev, unsigned int firstminor, unsigned int count, char *name);.
#3.Unregister major-minor number : void unregister_chrdev_region(dev_t first, unsigned int count);.
#4.To create major-minor structure(dev_t) : MKDEV(int major, int minor);.
#5.To get major-minor number : MAJOR(dev_t dev), and MINOR(dev_t dev);.

------------------------------------------------------------------------------------------------------------------------------------------
#Creating device file manually:
#1. mknod -m <permissions> <name> <dev_type> <major> <minor> - permissions and -m flag are optional can be modified by chmod afterwards, 
							      name should be the absolute path,
							      dev_type could be either c(character device file) or b(block device file)
							      major and minor whould be the major, minor number assigned to driver
#2. rm filename - To remove device file created manually

------------------------------------------------------------------------------------------------------------------------------------------
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

------------------------------------------------------------------------------------------------------------------------------------------
#Cdev structure operations:
#1. struct cdev my_dev;, struct file_operations my_ops;
#2. void cdev_init(struct cdev *cdev, struct file_operations *fops);
#3. int cdev_add(struct cdev *dev, dev_t num, unsigned int count);
#4. void cdev_del(struct cdev *dev);

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

------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------
Important fields of file_operations structure for us as of now:
1. struct module *owner:
2. ssize_t (*read) (struct file *, char _ _user *, size_t, loff_t *);
3  ssize_t (*write) (struct file *, const char _ _user *, size_t, loff_t *);
4. int (*ioctl) (struct inode *, struct file *, unsigned int, unsigned long);...this one was used in old version of kernel, now it changes to long unlocked_ioctl(struct file*,unsigned int, unsigned long)
5. int (*open) (struct inode *, struct file *);
6. int (*release) (struct inode *, struct file *);

------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------


------------------------------------------------------------------------------------------------------------------------------------------
#device file operation with memory allocation:
#1. void * kmalloc(size_t size, gfp_t flags);	//allocate memory dynamically in kernel area
#2. void kfree(const void *objp); 		//free memory allocated by kmalloc
#3. unsigned long copy_to_user(const __user void *to, void *from, unsigned long len);	//copy data from kernel to user space 
#4. unsigned long copy_from_user(void *to, const __user void *from, unsigned long n);	//copy data from user to kernel space
#5. void * vmalloc(unsigned long size);
#6. void vfree(const char *);
//for more in detail refer MI-II note.
------------------------------------------------------------------------------------------------------------------------------------------
#structure where we will register event handler:
#const struct kernel_param_ops my_param_ops = 
#{
#        .set = &notify_param, // Use our setter ...[int(*set)(const char *val, struct kernem_param *kp)]
#        .get = &param_get_int, // .. and standard getter[int(*get)(char *buffer, const struct kernel_param *kp )]
#	.free = //[void(*free)(void *args)]
#};

------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------
proc entry and related function

1. proc_create(const char *name, umode_t mode, struct proc_dir_entry *parent, const struct file_operations *proc_fops);
2. remove_proc_entry(const char *name, parent);
------------------------------------------------------------------------------------------------------------------------------------------

------------------------------------------------------------------------------------------------------------------------------------------
#Directory/file which are important:
#1. /dev - 	   	   will have the device file, by using "ls -l" command we can get the major-minor number
#2. /proc/devices - 	   driver with major number 
#3. /proc/modules - 	   will give module list,lsmod will also give the list of module
#4. /sys/module -   	   will have our module entry under it when we use module_param();
#5. /sys/devices/virtual - It hold the created struct class directory, under this directory device file(the file name which used in module)
#			   named directory will be there. 
#6. /var/lib/usbutils    - To check vendor-id of USB's.
------------------------------------------------------------------------------------------------------------------------------------------
#Important command:
#1. dmesg - 		   dmesg | tail -10, will get the kernel message, we can also see these messages under /var/log/syslog file as well.
#2. echo value > filepath - to change the value of parameter we will use it, can be use to write in device-node as well.
#3. getconf -a > get page size info and many other(ex d-type cache, i-type cache, and other cache related info as well)
#4. swapon -s > get swap memory size detail
#5. lscpu > get CPU related information
#6. ulimit -a > get sizes related info (ex. stack size on our system)
#7. free -m > get info of how much memory being used and free, -g for gigabytes, -m megabytes, -h human readable/understandable format
#8. press escape, then gg then =, then G to autoformat the file in vim/vi editor
#9. press escape then , :set number to set number in vi editor
#10. :set nonumber to remove line number from vi/vim editor
------------------------------------------------------------------------------------------------------------------------------------------
#Header file being used till now:
#1. linux/module.h - For all previous module only this header is enough.
#2. linux/fs.h     - for MKDEV, MAJOR, MINOR, register_chrdev_region(), unregister_chrdev_region(), alloc_chrdev_region().
#3. linux/device.h - Automatic creation of device file func, 
#4. linux/cdev.h   - Include to resolve the error for cdev_init(), cdev_add(), cdev_del() func.
#5. linux/slab.h   - include to resolve the error for kmalloc and kfree.
#6. linux/uaccess.h- include for copy_to_user and copy_from_user.
#7. linux/ioctl.h  - for ioctl system call
------------------------------------------------------------------------------------------------------------------------------------------
#Ways to communicate between user space and kernel space:
#	ioctl,sysfs,procfs,sysctl,UDP socket,netlink socket, configfs,debugfs

#ioctl for i/o control:	Interface to communicate between User space and Kernel space
#1. Steps need to follow to use ioctl
#	i] 	create ioctl command in driver
#			(#define IOCTL_COMMAND_NAME __IOX(magic number, command number, arg data type))
#	ii]	write ioctl function in driver and assign/register it to .unlocked_ioctl member of file_operations structure
#			(long ioctl_func(struct file *fp, unsigned int command, unsigned long arg))
#	iii]	create ioctl command in user space same as driver
#	v]	use ioctl system call from user space
#			long ioctl(struct file *, ioctl_command,Arg)
------------------------------------------------------------------------------------------------------------------------------------------
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
#	ioctl is used when we want to implement specific functionality in driver for device, otherwise just for read and write operation copy_to_user and copy_from_user is already there.

IMPORTANT TOPIC :

When one should use spinlock instead of mutex ?
ANS :

The Theory

In theory, when a thread tries to lock a mutex and it does not succeed, because the mutex is already locked, it will go to sleep, immediately allowing another thread to run.
It will continue to sleep until being woken up, which will be the case once the mutex is being unlocked by whatever thread was holding the lock before.
When a thread tries to lock a spinlock and it does not succeed, it will continuously re-try locking it, until it finally succeeds; thus it will not allow another thread
to take its place (however, the operating system will forcefully switch to another thread, once the CPU runtime quantum of the current thread has been exceeded, of course).

The Problem

The problem with mutexes is that putting threads to sleep and waking them up again are both rather expensive operations, they'll need quite a lot of CPU instructions and thus also take some time.
If now the mutex was only locked for a very short amount of time, the time spent in putting a thread to sleep and waking it up again might exceed the time the thread has actually slept by far and
it might even exceed the time the thread would have wasted by constantly polling on a spinlock. On the other hand, polling on a spinlock will constantly waste CPU time and if the lock is held for a longer amount of time, this will waste a lot more CPU time and it would have been much better if the thread was sleeping instead.

The Solution

Using spinlocks on a "single-core/single-CPU" system makes usually no sense, since as long as the spinlock polling is blocking the only available CPU core, no other thread can run 
and since no other thread can run, the lock won't be unlocked either. IOW, a spinlock wastes only CPU time on those systems for no real benefit. If the thread was put to sleep instead, 
another thread could have ran at once, possibly unlocking the lock and then allowing the first thread to continue processing, once it woke up again.

On a "multi-core/multi-CPU systems", with plenty of locks that are held for a very short amount of time only, the time wasted for constantly putting threads to sleep 
and waking them up again might decrease runtime performance noticeably. When using spinlocks instead, threads get the chance to take advantage of their full runtime 
quantum (always only blocking for a very short time period, but then immediately continue their work), leading to much higher processing throughput.

The Practice

Since very often programmers cannot know in advance if mutexes or spinlocks will be better (e.g. because the number of CPU cores of the target architecture is unknown), 
nor can operating systems know if a certain piece of code has been optimized for single-core or multi-core environments, most systems don't strictly distinguish between mutexes and spinlocks. 
In fact, most modern operating systems have "hybrid mutexes and hybrid spinlocks". What does that actually mean?

A "hybrid mutex behaves" like a spinlock at first on a multi-core system. If a thread cannot lock the mutex, it won't be put to sleep immediately, since the mutex might get unlocked pretty soon, 
so instead the mutex will first behave exactly like a spinlock. Only if the lock has still not been obtained after a certain amount of time (or retries or any other measuring factor), 
the thread is really put to sleep. If the same code runs on a system with only a single core, the mutex will not spinlock, though, as, see above, that would not be beneficial.

A "hybrid spinlock" behaves like a normal spinlock at first, but to avoid wasting too much CPU time, it may have a back-off strategy. It will usually not put the thread to sleep 
(since you don't want that to happen when using a spinlock), but it may decide to stop the thread (either immediately or after a certain amount of time) and allow another thread to run, 
thus increasing chances that the spinlock is unlocked (a pure thread switch is usually less expensive than one that involves putting a thread to sleep and waking it up again later on, though not by far).

Summary

If in doubt, use mutexes, they are usually the better choice and most modern systems will allow them to spinlock for a very short amount of time, if this seems beneficial. Using spinlocks can sometimes improve performance, but only under certain conditions and the fact that you are in doubt rather tells me, that you are not working on any project currently where a spinlock might be beneficial. You might consider using your own "lock object", that can either use a spinlock or a mutex internally (e.g. this behavior could be configurable when creating such an object), initially use mutexes everywhere and if you think that using a spinlock somewhere might really help, give it a try and compare the results (e.g. using a profiler), but be sure to test both cases, a single-core and a multi-core system before you jump to conclusions (and possibly different operating systems, if your code will be cross-platform).


proc_create :

struct proc_dir_entry *create_proc_entry( const char *name, mode_t mode,
 
                                             struct proc_dir_entry *parent );
 
 
 
struct proc_dir_entry {
 
    const char *name;           // virtual file name
 
    mode_t mode;                // mode permissions
 
    uid_t uid;              // File's user id
 
    gid_t gid;              // File's group id
 
    struct inode_operations *proc_iops; // Inode operations functions
 
    struct file_operations *proc_fops;  // File operations functions
 
    struct proc_dir_entry *parent;      // Parent directory
 
    ...
 
    read_proc_t *read_proc;         // /proc read function
 
    write_proc_t *write_proc;       // /proc write function
 
    void *data;             // Pointer to private data
 
    atomic_t count;             // use count
 
    ...
 
};
------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------
