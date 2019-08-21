#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/io.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tung Tran");
MODULE_DESCRIPTION("Turn on and off LED");
MODULE_INFO(intree, "Y");

#define LED1_GPIO 22 /* PTB0 */
#define LED1_IOMUX_BASE_ADDR 0x40048058
#define LED1_RGPIO_BASE_ADDR 0x400FF000

typedef union {
	struct {
		uint32_t ibe : 1;
		uint32_t obe : 1;
		uint32_t pue : 1;
		uint32_t pke : 1;
		uint32_t pus : 2;
		uint32_t dse : 3;
		uint32_t hys : 1;
		uint32_t ode : 1;
		uint32_t sre : 1;
		uint32_t speed : 2;
		uint32_t rfu1 : 6;
		uint32_t muxMode : 3;
		uint32_t rfu : 8;
	} ioMuxStr;
	uint32_t ioMuxReg;
} ioMuxUnion;

static void __iomem * io;
static uint32_t temp;

static int __init led_init(void)
{
	ioMuxUnion ioMux = { 
		.ioMuxStr.rfu = 0x00,
		.ioMuxStr.muxMode = 0x00,
		.ioMuxStr.rfu1 = 0x00,
		.ioMuxStr.speed = 0x01,
		.ioMuxStr.sre = 0x00,
		.ioMuxStr.ode = 0x00,
		.ioMuxStr.hys = 0x00,
		.ioMuxStr.dse = 0x06,
		.ioMuxStr.pus = 0x02,
		.ioMuxStr.pke = 0x00,
		.ioMuxStr.pue = 0x00,
		.ioMuxStr.obe = 0x01,
		.ioMuxStr.ibe = 0x00,
	};
	printk(KERN_ALERT "Hello world\n");
	io = ioremap(LED1_IOMUX_BASE_ADDR, 0x04);
	if(io == NULL) 	
    {
    	printk(KERN_ALERT "Fail map base address\n");
    	return -1;
    }
	/* Configure IOMUX */
	temp = ioread32(io);
	temp = ioMux.ioMuxReg;
	iowrite32(temp, io);

	/* Set PTB0 */
	io = ioremap(LED1_RGPIO_BASE_ADDR, 0x10);
	if(io == NULL) 	
    {
    	printk(KERN_ALERT "Fail map base address\n");
    	return -1;
    }
	// temp = ioread32(io+0x08);
	// temp &= ~(1 << LED1_GPIO);
	temp |= (1 << LED1_GPIO);
	iowrite32(temp, io + 0x08);
	return 0;
}

static void __exit led_exit(void)
{
	// temp = ioread32(io+0x04);
	// temp &= ~(1 << LED1_GPIO);
	temp |= (1 << LED1_GPIO);
	iowrite32(temp, io + 0x04);
	printk(KERN_ALERT "Goodbye world\n");
}

module_init(led_init);
module_exit(led_exit);
