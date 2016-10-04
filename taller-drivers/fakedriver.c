#include "fakeserialhelper.h"
#include <linux/kfifo.h>
#include <linux/semaphore.h>

#define DEVICE_NAME "fakeserial"
#define FAKESERIAL_FIFO_SIZE PAGE_SIZE

static int __init fakeserial_init(void) {
    printk(KERN_DEBUG "fakeserial: Initialized!\n");
    return 0;
}

static void __exit fakeserial_exit(void) {
}

module_init(fakeserial_init);
module_exit(fakeserial_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alumno");
MODULE_DESCRIPTION("Driver del dispositivo serial ficticio");
