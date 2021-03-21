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

static struct file_operations fops;

extern int (*STUB_start_elevator)(void);
extern int (*STUB_issue_request)(int, int, int, int);
extern int (*STUB_stop_elevator)(void);

//Elevator Structure and event tracking

int active_elevator = 0;
int inactive_elevator = 0;
int done_elevator = 0;

//holds information about current floor and amount of passengers waiting.
struct{
    int curr_floor;
    char current_floor_state[10];
    int current_elevator_total;
    int animal;
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



