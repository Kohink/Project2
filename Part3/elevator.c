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
    char *current_floor_state[10];
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

///////////////////////////////////////// End of Struct Initialization /////////////////////////////////////////

//thread function
int thread_run(void *data) 
{
	struct thread_parameter *parm = data;

	while (!kthread_should_stop()) 
    {
        if(active_elevator == 1)
        {

        }
		ssleep(1);
		//parm->cnt++;
	}

	return 0;
}

//thread function
void thread_init_parameter(struct thread_parameter *parm) {

    printk(KERN_NOTICE "Initializing Thread\n");
	mutex_init(&parm->mutex);
	parm->kthread = kthread_run(thread_run, parm, "thread example %d";
}

/////////////////////////////////////////

//proc: open, read, release - done
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


int start_elevator(void)
{

	char *buf = kmalloc(sizeof(char) * 100, __GFP_RECLAIM);
	if (buf == NULL) 
	{
		printk(KERN_WARNING "start_elevator");
		return -ENOMEM;
	}
    if(active_elevator == 1 || done_elevator == 1 || inactive_elevator == 1)
    {
        return 1;
    }
    else
    {
        elevator.current_floor_state = "IDLE";
        elevator.curr_floor = 1;
        elevator.current_elevator_total = 0;
        elevator.serviced_passengers = 0;
        elevator.waiting_passengers = 0;
    }

    return 0;

}

int stop_elevator(void)
{
	if(active_elevator == 0 || inactive_elevator != 0 || done_elevator != 0)
    {
		return 1;
    }
    done_elevator = 0;
	return 0;
}

int issue_request(int start_floor, int destination_floor, int type)
{
    Passenger *pass;
    pass = kmalloc()(sizeof(Passenger), __GFP_RECLAIM);

    if (elevator_is_deactivating == 1 || elevator_stopped == 1 || elevator_is_active == 0)
    {
		return 1;
    }

	if (type != 0 || type!= 1 || type != 2 || start_floor == destination_floor || start_floor > 10 || start_floor < 1 || destination_floor > 10 || destination_floor < 1)
    {
		return 1;
    }

    if(pass == NULL)
    {
        return -ENOMEM;
    }

        pass->floor_boarded = start_floor;
        pass->floor_departing = destination_floor;
        pass->item_carrying = type;

        elevator.waiting_passengers +=1;

        list_add_tail(&pass->list, &elev[start_floor-1].list);
        
        elev[start_floor-1].data++;

        return 0;

}

//need to fix print a lot
int print_elevator(void) {
	int i;
	Animal *a;
	struct list_head *temp;

	char *buf = kmalloc(sizeof(char) * 100, __GFP_RECLAIM);
	if (buf == NULL) {
		printk(KERN_WARNING "print_elevator");
		return -ENOMEM;
	}

	/* init message buffer */
	strcpy(message, "");

	/* headers, print to temporary then append to message buffer */
	sprintf(buf, "Elevator state: %d\n", elevator.total_cnt);       strcat(message, buf);
    sprintf(buf, "Elevator status: %d\n", elevator.total_cnt);       strcat(message, buf);
	sprintf(buf, "Current floor: %d\n", elevator.total_length);   strcat(message, buf);
	sprintf(buf, "Number of passengers: %d\n", elevator.total_weight);   strcat(message, buf);
    sprintf(buf, "Number of passengers waiting: %d\n", elevator.total_weight);   strcat(message, buf);
	sprintf(buf, "Number of passengers serviced: %d\n", elevator.total_weight);   strcat(message, buf);

	/* print entries */
	i = 0;
	//list_for_each_prev(temp, &elevator.list) { /* backwards */
	list_for_each(temp, &elevator.list) { /* forwards*/
		a = list_entry(temp, Animal, list);

		/* newline after every 5 entries */
		if (i % 5 == 0 && i > 0)
			strcat(message, "\n");

		sprintf(buf, "%s ", a->name);
		strcat(message, buf);

		i++;
	}

	/* trailing newline to separate file from commands */
	strcat(message, "\n");

	kfree(buf);
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

    STUB_start_elevator = start_elevator;
    STUB_issue_request = issue_request;
    STUB_stop_elevator = stop_elevator;

	
	if (!proc_create(ENTRY_NAME, PERMS, NULL, &fops)) {
		printk(KERN_WARNING "elevator_init\n");
		remove_proc_entry(ENTRY_NAME, NULL);
		return -ENOMEM;
	}

	//elevators.total_cnt = 0;
	//elevators.total_length = 0;
	//6elevators.total_weight = 0;
	INIT_LIST_HEAD(&elevators.list);

	thread_init_parameter(&thread1);
	if (IS_ERR(thread1.kthread)) {
		printk(KERN_WARNING "error spawning thread");
		remove_proc_entry(ENTRY_NAME, NULL);
		return PTR_ERR(thread1.kthread);
	}

	elevator.current_floor_state = "OFFLINE";
	
	return 0;
}
module_init(elevator_init);

//need to fix this one below too
static void elevator_exit(void) 
{
    STUB_start_elevator = NULL;
	STUB_issue_request = NULL;
	STUB_stop_elevator = NULL;
    delete_elevator();
	remove_proc_entry(ENTRY_NAME, NULL);
    mutex_destroy(&thread.mutex);
}
module_exit(elevator_exit);