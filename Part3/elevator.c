#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/linkage.h>
#include <linux/list.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/string.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>

MODULE_LICENSE("GPL");
#define ENTRY_NAME "elevator"
#define ENTRY_SIZE 10000
#define PERMS 0644
#define PARENT NULL

//extern int (*STUB_start_elevator)(void);
//extern int (*STUB_issue_request)(int, int, int, int);
//extern int (*STUB_stop_elevator)(void);
static struct file_operations fops;

//Elevator Structure and event tracking

int active_elevator = 0;
int inactive_elevator = 0;
int done_elevator = 0;

//holds information about current floor and amount of passengers waiting.
struct{
    int curr_floor;
    char current_floor_state[10];
    int current_elevator_total;
    int elevator;
    int waiting_passengers;
    int serviced_passengers;
    struct list_head list;
} elevator;

//holds information about the passenger
typedef struct passenger{
    int item_carrying;
    int floor_boarded;
    int floor_departing;
    struct list_head list;

} Passenger;

//holds list for elevator floors - will contain elevator information
struct floor{
    int data;
    struct list_head list;
}
struct floor elev[10]; //elevator floors;


//using kthreads for scheduling and running 
struct thread_param{
    struct task_struct *kthread;
    struct mutex mutex;
}
struct thread_param thread;

//proc: open, read, release
int elevator_proc_open(struct inode *sp_inode, struct file *sp_file) {
	read_p = 1;
	message = kmalloc(sizeof(char) * ENTRY_SIZE, __GFP_RECLAIM | __GFP_IO | __GFP_FS);
	if (message == NULL) {
		printk(KERN_WARNING "elevator_proc_open");
		return -ENOMEM;
	}
	
	add_elevator(get_random_int() % NUM_elevator_TYPES);
	return print_elevators();
}

ssize_t elevator_proc_read(struct file *sp_file, char __user *buf, size_t size, loff_t *offset) {
	int len = strlen(message);
	
	read_p = !read_p;
	if (read_p)
		return 0;
		
	copy_to_user(buf, message, len);
	return len;
}

int elevator_proc_release(struct inode *sp_inode, struct file *sp_file) {
	kfree(message);
	return 0;
}

//need to fix this one too
void delete_elevator(int type) 
{
	struct list_head move_list;
	struct list_head *temp;
	struct list_head *dummy;
	int i;
	Passenger *a;

	INIT_LIST_HEAD(&move_list);

	/* move items to a temporary list to illustrate movement */
	//list_for_each_prev_safe(temp, dummy, &elevator.list) { /* backwards */
	list_for_each_safe(temp, dummy, &elevator.list) { /* forwards */
		a = list_entry(temp, Animal, list);

		if (a->id == type) {
			//list_move(temp, &move_list); /* move to front of list */
			list_move_tail(temp, &move_list); /* move to back of list */
		}

	}	

	/* print stats of list to syslog, entry version just as example (not needed here) */
	i = 0;
	//list_for_each_entry_reverse(a, &move_list, list) { /* backwards */
	list_for_each_entry(a, &move_list, list) { /* forwards */
		/* can access a directly e.g. a->id */
		i++;
	}
	printk(KERN_NOTICE "animal type %d had %d entries\n", type, i);

	/* free up memory allocation of elevator */
	//list_for_each_prev_safe(temp, dummy, &move_list) { /* backwards */
	list_for_each_safe(temp, dummy, &move_list) { /* forwards */
		a = list_entry(temp, Animal, list);
		list_del(temp);	/* removes entry from list */
		kfree(a);
	}
}

//need to fix this one below
static int elevator_init(void) 
{
	fops.open = elevator_proc_open;
	fops.read = elevator_proc_read;
	fops.release = elevator_proc_release;
	
	if (!proc_create(ENTRY_NAME, PERMS, NULL, &fops)) {
		printk(KERN_WARNING "elevator_init\n");
		remove_proc_entry(ENTRY_NAME, NULL);
		return -ENOMEM;
	}

	elevators.total_cnt = 0;
	elevators.total_length = 0;
	elevators.total_weight = 0;
	INIT_LIST_HEAD(&elevators.list);
	
	return 0;
}
module_init(elevator_init);

//need to fix this one below too
static void elevator_exit(void) 
{
	remove_proc_entry(ENTRY_NAME, NULL);
}
module_exit(elevator_exit);