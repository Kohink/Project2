#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/semaphore.h>
#include <linux/list.h>
#include <linux/string.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

/*

MODULE_LICENSE("Dual BSD/GPL");
#define BUF_LEN 100//max length of read/write message
static structproc_dir_entry* proc_entry;//pointer to procentry
static char msg[BUF_LEN];//buffer to store read/write message
static intprocfs_buf_len;//variable to hold length of message

static structfile_operationsprocfile_fops= 
{
    .owner = THIS_MODULE,
    .read = procfile_read, 
    .write = procfile_write,
};

// read
static ssize_tprocfile_read(structfile* file, char * ubuf, size_tcount, loff_t*ppos)
{
    printk(KERN_INFO "proc_read\n");
    procfs_buf_len= strlen(msg);
    if (*ppos> 0 || count < procfs_buf_len) //check if data already read and if space in user buffer
        return 0;
    if (copy_to_user(ubuf, msg, procfs_buf_len)) //send data to user buffer
        return -EFAULT;
    *ppos= procfs_buf_len;//update position
    printk(KERN_INFO "gave to user %s\n", msg);
return procfs_buf_len; //return number of characters read

// write
static ssize_tprocfile_write(structfile* file, constchar * ubuf, size_tcount, loff_t* ppos)
{
    printk(KERN_INFO "proc_write\n");
    if (count > BUF_LEN)
        procfs_buf_len= BUF_LEN;
    else
        procfs_buf_len= count;
    copy_from_user(msg, ubuf, procfs_buf_len);
    printk(KERN_INFO "got from user: %s\n", msg);
    return procfs_buf_len;

// init
static inthello_init(void)
{
    proc_entry= proc_create("hello", 0666, NULL, &procfile_fops);
    if (proc_entry== NULL)
        return -ENOMEM;
return 0;

// exit
static void hello_exit(void)
{
    proc_remove(proc_entry);
    return;
}

*/