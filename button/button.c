#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/interrupt.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tung Tran");
MODULE_DESCRIPTION("Press button to turn on and off LED");
MODULE_INFO(intree, "Y");

/* LED define */
#define LED1_GPIO 22 /* PTB0 */
#define LED1_IOMUX_BASE_ADDR 0x40048058
#define LED1_RGPIO_BASE_ADDR 0x400FF000

/* Button define */
#define SW1_GPIO 6 /* PTB16 GPIO1 */
#define LED1_IOMUX_BASE_ADDR 0x40048098
#define LED1_RGPIO_BASE_ADDR 0x400FF040
#define LED1_PORT_CTRL_BASE_ADDR 0x4004A000

/* Interrupt */
#define SW1_IRQ 139
#define PIN_CONTROL_REG 0x4004A018

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

typedef union {
    struct {
        uint32_t rfu : 16;
        uint32_t irqc : 4;
        uint32_t rfu1 : 4;
        uint32_t isf : 1;
        uint32_t rfu2 : 7;
    } pinCtrlStr;
    uint32_t pinCtrlReg;
} pinCtrlUnion;

static void __iomem * io;
static uint32_t temp;

irqreturn_t irq_handler(int irq, void *dev_id, struct pt_regs *regs)
{
    return IRQ_HANDLED;
}

static int __init led_init(void)
{
    int32_t err;

    ioMuxUnion ioLED = {
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

    ioMuxUnion ioSW = {
        .ioMuxStr.rfu = 0x00,
        .ioMuxStr.muxMode = 0x00,
        .ioMuxStr.rfu1 = 0x00,
        .ioMuxStr.speed = 0x01,
        .ioMuxStr.sre = 0x00,
        .ioMuxStr.ode = 0x00,
        .ioMuxStr.hys = 0x00,
        .ioMuxStr.dse = 0x01,
        .ioMuxStr.pus = 0x02,
        .ioMuxStr.pke = 0x00,
        .ioMuxStr.pue = 0x00,
        .ioMuxStr.obe = 0x00,
        .ioMuxStr.ibe = 0x01,
    };

    pinCtrlUnion pinSW = {
        .rfu = 0x00,
        .irqc = 0x09,
        .rfu1 = 0x00,
        .isf = 0x00,
        .rfu2 = 0x00,
    };

    printk(KERN_ALERT "Hello world\n");
    err = request_irq(SW1_IRQ, irq_handler, IRQF_SHARED, "sw1", (void *)(irq_handler));
    if (err < 0)
    {
         /* handle error*/
         printk(KERN_ALERT "%s: request_irq failed with %d\n", __func__, ret);
         return err;
    }
}

static void __exit led_exit(void)
{
    printk(KERN_ALERT "Goodbye world\n");
    free_irq(SW1_IRQ, NULL);
}

module_init(led_init);
module_exit(led_exit);