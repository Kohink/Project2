#include <sys/syscall.h>

int main()
{
    fork();
    fork();
    fork();
    fork();
    return 0;
}