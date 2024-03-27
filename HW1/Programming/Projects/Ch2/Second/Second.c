#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/timer.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Auther Name");
MODULE_DESCRIPTION("Test Driver Module");

static int myInit(void);
static void myExit(void);

struct proc_dir_entry *my_proc;

unsigned long s_time, e_time;

/* Read operation on proc */
ssize_t read_data(struct file *fp, char *buf, size_t len, loff_t * off)
{
    /* Logic to read data once */
    static int finished = 0;
    if (finished) {
        finished = 0;
        return 0;
    }
    finished = 1;

    /* find out current time in HZ*/
    e_time = jiffies / HZ;

    sprintf(buf, "Kernel Module is running : %lu seconds \n", e_time - s_time);
    return strlen(buf);
}

/* File operation for Proc */
static const struct proc_ops proc_ops = {
    .proc_read = read_data,
};

static int __init myInit(void)
{
    /* Find out time in Hz when kernel module initialize */
    s_time = jiffies / HZ;
    my_proc = proc_create("seconds", 0666, NULL, &proc_ops);

    if (my_proc == NULL) {
        printk(KERN_INFO "Error to create proc File\n");
        return -1;
    }

    return 0;
}

static void myExit(void)
{
    /* remove proc file*/
    remove_proc_entry("seconds", NULL);
    return;
}

module_init(myInit);
module_exit(myExit);


