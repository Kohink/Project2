#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/syscalls.h>

// System call stub
long (*STUB_start_elevator)(void) = NULL;
EXPORT_SYMBOL(STUB_start_elevator);

long (*STUB_issue_request)(int, int, int) = NULL;
EXPORT_SYMBOL(STUB_issue_request);

long (*STUB_stop_elevator)(void) = NULL;
EXPORT_SYMBOL(STUB_stop_elevator);


// System call wrapper
SYSCALL_DEFINE1(start_elevator, void, elevator) {
    printk(KERN_NOTICE "Inside SYSCALL_DEFINE1 block. %s: ", __FUNCTION__);
    if (STUB_start_elevator != NULL)
        return STUB_start_elevator();
    else
        return -ENOSYS;
}

SYSCALL_DEFINE1(issue_request, int, int, int, elevator) {
    printk(KERN_NOTICE "Inside SYSCALL_DEFINE1 block. %s: Your int is %d\n", __FUNCTION__, elevator);
    if (STUB_issue_request != NULL)
        return STUB_issue_request(elevator);
    else
        return -ENOSYS;
}

SYSCALL_DEFINE1(stop_elevator, void, elevator) {
    printk(KERN_NOTICE "Inside SYSCALL_DEFINE1 block. %s: ", __FUNCTION__);
    if (STUB_stop_elevator != NULL)
        return STUB_stop_elevator();
    else
        return -ENOSYS;
}