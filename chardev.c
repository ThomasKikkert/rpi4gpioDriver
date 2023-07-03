#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched/mm.h>
#include <linux/sched.h>
#include <linux/crypto.h>
#include <linux/scatterlist.h>
#include <linux/vmalloc.h>
#include <crypto/hash.h>
#include <crypto/sha2.h>
#include <linux/pgtable.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/device.h>
#include <linux/fs.h>

static dev_t tdevice;
static struct cdev cdevice;
static struct class *class = NULL;
static struct device *device = NULL;

#define DEVICE_NAME "gpio-driver"

static int __init proc_init(void);
static void __exit proc_cleanup(void);
static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char __user *, size_t, loff_t *);

static struct file_operations fops = {
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_release,
    .owner = THIS_MODULE
};

static int dev_open(struct inode *inodep, struct file *fptr) {
    printk(KERN_INFO "opened rpi 4 gpio driver\n");

    return 0;
}

static int dev_release(struct inode *inodep, struct file *fptr) {
    printk(KERN_INFO "closed rpi 4 gpio driver\n");
    return 0;
}

static ssize_t dev_write(struct file *fptr, const char *buff, size_t len, loff_t *offset) {
    return 0;
}

static ssize_t dev_read(struct file *fptr, char *buff, size_t len, loff_t *offset) {
    /*
    char str[HASH_SIZE * 2 + 1];
    size_t strLen;
    int i;
    int ret;

    for (i = 0; i < HASH_SIZE; i++)
        sprintf(&str[i * 2], "%02x", (unsigned char)hash[i]);
    str[HASH_SIZE * 2] = '\0';

    strLen = strlen(str);
    ret = copy_to_user(buff, str, strLen);
    printk(KERN_CRIT "data send to user: %s\n", str);

    if (ret < 0)
    {
        printk(KERN_CRIT "Error coppy to user: %d\n", ret);
    }

    *offset += strLen - len;

    if (*offset > strLen)
    {
        printk(KERN_CRIT "Done reading\n");
        return 0;
    }
    return strLen; */
    return 0;
}

static int proc_init(void) {
    int ret;
    ret = alloc_chrdev_region(&tdevice, 0, 1, DEVICE_NAME);

    if (ret < 0)
    {
        printk(KERN_ERR "faild to allocat chardevices\n");
        return ret;
    }

    cdev_init(&cdevice, &fops);
    cdev_add(&cdevice, tdevice, 1);

    if (ret < 0)
    {
        printk(KERN_ERR "faild to add cdev!!\nerro code: %d\n", ret);
        unregister_chrdev_region(tdevice, 1);
        return ret;
    }

    class = class_create(THIS_MODULE, DEVICE_NAME);
    if (IS_ERR(class))
    {
        printk(KERN_ERR "faild to create chrdevice class\n");
        cdev_del(&cdevice);
        unregister_chrdev_region(tdevice, 1);
        return PTR_ERR(class);
    }

    device = device_create(class, NULL, tdevice, NULL, DEVICE_NAME);
    if (IS_ERR(device))
    {
        printk(KERN_ERR "faild to create chrdevice device\n");
        class_destroy(class);
        cdev_del(&cdevice);
        unregister_chrdev_region(tdevice, 1);
        return PTR_ERR(device);
    }

    printk(KERN_INFO "rpi 4 gpio driver: kernel module initialized\n");

    return 0;
}

static void proc_cleanup(void) {
    device_destroy(class, tdevice);
    class_destroy(class);
    cdev_del(&cdevice);
    unregister_chrdev_region(tdevice, 1);
    printk(KERN_INFO "rpi 4 gpio driver: performing cleanup of module\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thomas K.");
module_init(proc_init);
module_exit(proc_cleanup);
