#include "fakeserialhelper.h"

#define FAKE_TIMER_DELAY 1  // In seconds

static fake_irq_handler_t fake_irq_handler;
static struct timer_list fake_timer;
static int inb_index = 0;

// Private functions

static void fake_timer_function(unsigned long data);

static void fake_set_timer(void) {
    init_timer(&fake_timer);
    fake_timer.expires = jiffies + FAKE_TIMER_DELAY*HZ;
    fake_timer.data = 0;
    fake_timer.function = fake_timer_function;
    add_timer(&fake_timer);
}

static void fake_timer_function(unsigned long data) {
    printk(KERN_DEBUG "fakeserial: Timer function called!\n");
    fake_irq_handler();
    fake_set_timer();
}

// Exported functions

u8 fake_inb(int port) {
    int i = (inb_index % 93);
    inb_index++;
    return (u8)('!' + i);
}

int fake_serial_init(int mode, fake_irq_handler_t irq_handler) {
    fake_irq_handler = irq_handler;
    fake_set_timer();

    printk(KERN_DEBUG "fakeserial: Timer started!\n");

    return 0;
}

int fake_serial_exit(void) {
    del_timer(&fake_timer);
    return 0;
}
