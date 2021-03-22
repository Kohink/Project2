#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/time.h>

MODULE_LICENSE("Dual BSD/GPL");
#define BUF_LEN 500//max length of read/write message
static struct proc_dir_entry* proc_entry;//pointer to procentry
static char msg[BUF_LEN];//buffer to store read/write message
static int procfs_buf_len;//variable to hold length of message
static long int oldSec, oldNano;
static bool hasPrevious = false;
const int secToNano = 999999999;

// read
static ssize_t procfile_read(struct file* file, char * ubuf, size_t count, loff_t *ppos)
{
    struct timespec current_time;
    current_time = current_kernel_time();

    long int seconds, nano;
    long int secDif, nanoDif;

    seconds = current_time.tv_sec;
    nano = current_time.tv_nsec;

    printk(KERN_INFO "proc_read\n");
    procfs_buf_len = strlen(msg);
    if (*ppos> 0 || count < procfs_buf_len) //check if data already read and if space in user buffer
        return 0;

    if (hasPrevious == true)
    {
	    long int checkForNegNano = nano - oldNano;
	
	if (checkForNegNano < 0)
    	{
    	    secDif = (seconds - oldSec) - 1;
	    nanoDif = secToNano + checkForNegNano;
    	}
	else
	{
	    secDif = seconds - oldSec;
	    nanoDif = checkForNegNano;
	}

        procfs_buf_len += sprintf(msg,"current time: %ld.%ld\nelapsed time: %ld.%09ld\n\n", seconds, nano, secDif, nanoDif);
    }
    else
    {
        procfs_buf_len += sprintf(msg, "current time: %ld.%ld\n\n", seconds, nano);
	hasPrevious = true;
    }

    oldSec = seconds;
    oldNano = nano;
    

    if (copy_to_user(ubuf, msg, procfs_buf_len)) //send data to user buffer
        return -EFAULT;
    *ppos= procfs_buf_len;//update position
    printk(KERN_INFO "gave to user %s\n", msg);

    return procfs_buf_len; //return number of characters read
}

// write
static ssize_t procfile_write(struct file* file, const char * ubuf, size_t count, loff_t *ppos)
{
    printk(KERN_INFO "proc_write\n");
    if (count > BUF_LEN)
        procfs_buf_len = BUF_LEN;
    else
        procfs_buf_len = count;
    copy_from_user(msg, ubuf, procfs_buf_len);
    printk(KERN_INFO "got from user: %s\n", msg);
    return procfs_buf_len;
}

static struct file_operations procfile_fops= 
{
    .owner = THIS_MODULE,
    .read = procfile_read, 
    .write = procfile_write,
};

// init
static int my_timer_init(void)
{
    proc_entry= proc_create("timer", 0666, NULL, &procfile_fops);
    if (proc_entry == NULL)
        return -ENOMEM;
    return 0;
}

// exit
static void my_timer_exit(void)
{
    proc_remove(proc_entry);
    return;
}

module_init(my_timer_init);
module_exit(my_timer_exit);