#ifndef __FAKESERIAL_H__

#define __FAKESERIAL_H__

#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#define FAKE_SERIAL_MODE_TEXT 0
#define FAKE_SERIAL_PORT 0

typedef void (*fake_irq_handler_t)(void);
u8 fake_inb(int port);
int fake_serial_init(int mode, fake_irq_handler_t irq_handler);
int fake_serial_exit(void);

#endif
