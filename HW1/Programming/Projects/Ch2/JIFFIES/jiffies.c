#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/jiffies.h>

#define PROC_NAME "jiffies"

struct proc_dir_entry *my_proc;

/* This function is called each time /proc/jiffies is read */
ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
    int rv = 0;
    char buffer[20]; // Adjusted buffer size
    static int completed = 0;

    if (completed) {
        completed = 0;
        return 0;
    }

    completed = 1;
    rv = sprintf(buffer, "%lu\n", jiffies);

    /* copies kernel space buffer to user space usr buf */
    copy_to_user(usr_buf, buffer, rv);

    return rv;
}

/* File operation on proc */
static const struct proc_ops proc_ops = {
    .proc_read = proc_read,
};

/* This function is called when the module is loaded. */
int proc_init(void)
{
    /* creates the /proc/jiffies entry */
    my_proc = proc_create(PROC_NAME, 0666, NULL, &proc_ops);
    if (my_proc == NULL) {
        printk(KERN_INFO "Error creating /proc/%s entry\n", PROC_NAME);
        return -ENOMEM;
    }
    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
    return 0;
}

/* This function is called when the module is removed. */
void proc_exit(void)
{
    /* removes the /proc/jiffies entry */
    remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple Linux kernel module to display jiffies");
MODULE_AUTHOR("Your Name");

