#include "fakeserialhelper.h"
#include <linux/kfifo.h>
#include <linux/semaphore.h>

#define DEVICE_NAME "fakeserial"
#define FAKESERIAL_FIFO_SIZE PAGE_SIZE

/* Prototypes */
ssize_t mi_read(struct file *file, char __user *s, size_t size, loff_t *loff);
void push_next_bytes(void);

/* Structures */
struct kfifo fifo;
struct semaphore semaphore;
struct semaphore fifo_semaphore;

struct file_operations mi_fops = {
	.owner = THIS_MODULE,
	.read = &mi_read
};

struct miscdevice mi_miscdev = {
	MISC_DYNAMIC_MINOR,
	DEVICE_NAME,
	&mi_fops
};

/* Functions */
void push_next_bytes(void) {
	u8 bytes = fake_inb(FAKE_SERIAL_PORT);
	printk(KERN_DEBUG "Escribo byte \n");
	kfifo_in(&fifo, &bytes, sizeof(bytes));
	up(&fifo_semaphore);
}

ssize_t mi_read(struct file *file, char __user *s, size_t size, loff_t *loff) {
	u8 byte;

	down_interruptible(&semaphore);
	/* I have the semaphore */

	int i;
	for (i = 0; i < size; i++) {
		down_interruptible(&fifo_semaphore);
		printk(KERN_DEBUG "Leo byte \n");
		unsigned int elements_copied = kfifo_out(&fifo, &byte, 1);

		if (elements_copied == 0) {
			break;
		}

		copy_to_user(s + i, &byte, sizeof(byte));
	}

	/* Release the semaphore */
	up(&semaphore);

	return i;
}


static int __init fakeserial_init(void) {
	/* Initialize structures */
	kfifo_alloc(&fifo, PAGE_SIZE, GFP_KERNEL);
	// Allow the first to pass
	sema_init(&semaphore, 1);
	sema_init(&fifo_semaphore, 0);

	/* Initizlize fake serial */
	fake_serial_init(FAKE_SERIAL_MODE_TEXT, (fake_irq_handler_t) push_next_bytes);

	/* Register device */
	misc_register(&mi_miscdev);

    printk(KERN_DEBUG "fakeserial: Initialized!\n");
    return 0;
}

static void __exit fakeserial_exit(void) {
	fake_serial_exit();
	misc_deregister(&mi_miscdev);
}

module_init(fakeserial_init);
module_exit(fakeserial_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alumno");
MODULE_DESCRIPTION("Driver del dispositivo serial ficticio");
