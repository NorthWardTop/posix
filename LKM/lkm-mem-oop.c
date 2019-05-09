#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/export.h>
#include <linux/file.h>
#include <linux/blkdev.h>
#include <linux/poll.h>
#include <linux/cdev.h>
#include <linux/jiffies.h>
#include <linux/percpu.h>
#include <linux/uio.h>
#include <linux/idr.h>
#include <linux/bsg.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/fs.h>

#define DEV_NAME		"mem_dev"
#define MEM_SIZE		1024

struct mem{
	unsigned char *mem_name;
	int mem_major;
	int mem_minor;
	struct class *mem_class;
	struct file_operations *mem_fops;
	unsigned char *data[MEM_SIZE];
	unsigned short data_len;
};

static struct mem *pmem_dev;

static int mem_open(struct inode *inode, struct file *filp)
{
	printk("memory device open\n");
	//进行对象的注册, 每个设备的对象那个注册在该设备文件的私有数据域
	filp->private_data = pmem_dev;


	return 0;
}


static int mem_release (struct inode *inode, struct file *filp)
{
	printk("memory device close\n");
	filp->private_data = NULL;
	return 0;
}


static ssize_t 
mem_read (struct file *filp, char __user *buf, size_t len, loff_t *pos)
{
	printk("memory device read\n");
	unsigned long long int tmp = (unsigned long long int)*pos;
	unsigned int count = len;
	int ret = -1;
	struct mem *dev = (struct mem *)filp->private_data;
	//判断起始位置越界
	if(tmp >= MEM_SIZE) {
		return 0; //读了0字节
	}
	//判断读长度越界 |----p---s
	//将要写的数目>已有的空间
	if(count > MEM_SIZE - tmp) {
		count =MEM_SIZE - tmp;
	}
	if(copy_to_user(buf, filp->private_data+tmp, count)) {
		ret = -EFAULT;
	} else {
		*pos += count; //文件读指针后移
		ret = count;
		printk("mem_dev write %u bytes\n", count);
	}
	return ret;

}


static ssize_t mem_write (struct file *filp, const char __user *buf, size_t len, loff_t *pos)
{
	printk("memory device write\n");
	unsigned long long int tmp = (unsigned long long int)*pos;
	unsigned int count  = len;
	int ret = -1;
	struct mem *dev = filp->private_data;
	//判断起始位置越界
	if(tmp>=MEM_SIZE) {
		return 0; //写了0字节
	}
	//判断读长度越界 |----p---s
	//将要读的数目>已有的tmp
	if(count > tmp) {
		count = tmp;
	}
	if(copy_from_user(filp->private_data+tmp, buf, count)) {
		ret = -EFAULT;
	} else {
		*pos -= count; //文件读指针前移
		ret = count;
		printk("mem_dev read %u bytes\n", count);
	}
	return ret;
}

static int mem_setup_dev(struct mem *dev, int index)
{
	dev->mem_minor = index;
	memset(dev->data, 0, MEM_SIZE);
	dev->data_len = 0;

	return 0;
}

static struct file_operations mem_fops = {
	.owner		= THIS_MODULE,
	.open 		= mem_open,
	.release 	= mem_release,
	.read 		= mem_read,
	.write		= mem_write
};

static int __init mem_dev_init(void)
{
	int ret=-1;
	printk("memory device loaded\n");
	//分配对象
	pmem_dev = kzalloc(sizeof(struct mem), GFP_KERNEL);
	if(!pmem_dev) {
		printk("mem_dev object alloc failed!\n");
		ret= -ENOMEM;
		goto err_nomem;
	}
	//设置对象
	pmem_dev->mem_name=kzalloc(sizeof(DEV_NAME),GFP_KERNEL);
	if(!pmem_dev->mem_name) {
		printk("object name alloc failed!\n");
		ret = -ENOMEM;
		goto err_nomem_name;
	}
	memcpy(pmem_dev->mem_name, DEV_NAME, sizeof(DEV_NAME));
	pmem_dev->mem_fops = &mem_fops;
	pmem_dev->mem_major = register_chrdev(0, pmem_dev->mem_name, pmem_dev->mem_fops);
	if(pmem_dev->mem_major<0) {
		printk("device register failed!\n");
		ret = pmem_dev->mem_major;
		goto err_reg;
	}
	ret=mem_setup_dev(pmem_dev, 0);
	if(ret<0) {
		ret = -EINVAL;
		printk("err setup object");
		goto err_setup;
	}
	
	pmem_dev->mem_class = class_create(THIS_MODULE, pmem_dev->mem_name);
	if(IS_ERR(pmem_dev->mem_class)) {
		printk("device class create failed!\n");
		ret = PTR_ERR(pmem_dev->mem_class);
		goto err_class_create;

	device_create(pmem_dev->mem_class, NULL, \
	MKDEV(pmem_dev->mem_major, pmem_dev->mem_minor), NULL, pmem_dev->mem_name);
	
	return 0;

err_setup:
err_class_create:
	unregister_chrdev(pmem_dev->mem_major, pmem_dev->mem_name);
err_reg:
	kfree(pmem_dev->mem_name);
err_nomem_name:
	kfree(pmem_dev);
err_nomem:
	return ret;
}

static void __exit mem_dev_exit(void)
{
	printk("removed memory device!\n");
	device_destroy(pmem_dev->mem_class, MKDEV(pmem_dev->mem_major, pmem_dev->mem_minor));
	unregister_chrdev(pmem_dev->mem_major, pmem_dev->mem_name);
	kfree(pmem_dev->mem_name);
	kfree(pmem_dev);
} 


module_init(mem_dev_init);
module_exit(mem_dev_exit);

MODULE_AUTHOR("yonghui.lee <yonghuilee.cn@gmail.com>");
MODULE_DESCRIPTION("GEC6818 LED Device Driver");
MODULE_LICENSE("GPL");
MODULE_VERSION("v2.0");



