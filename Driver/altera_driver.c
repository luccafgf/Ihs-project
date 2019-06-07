#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("FPGA driver for SoundBoard");
MODULE_AUTHOR("Conducao do tio");

#define SWITCHES            1
#define BUTTONS             2
#define GREEN_LED           3
#define RED_LED             4
#define SEVEN_DISPLAYS_4    5
#define SEVEN_DISPLAYS_2    6


static uint32_t data;

//-- Hardware Handles
static void *hexport_green_led;
static void *hexport_red_led;
static void *hexport_seven_displays_4;
static void *hexport_seven_displays_2;
static void *inport_switches;   
static void *inport_buttons;   

//-- Char Driver Interface
static int   MAJOR_NUMBER = 91;
static int     char_device_open    ( struct inode * , struct file *);
static int     char_device_release ( struct inode * , struct file *);
static ssize_t char_device_read    ( struct file * , char *,       size_t , loff_t *);
static ssize_t char_device_write   ( struct file * , const char *, size_t , loff_t *);

static struct file_operations file_opts = {
      .read = char_device_read,
      .open = char_device_open,
     .write = char_device_write,
   .release = char_device_release
};


// Driver functions implementations
static int char_device_open(struct inode *inodep, struct file *filep) {
   printk(KERN_ALERT "altera_driver: device opened.\n");
   return 0;
}

static int char_device_release(struct inode *inodep, struct file *filep) {
   printk(KERN_ALERT "altera_driver: device closed.\n");
   return 0;
}

static ssize_t char_device_read(struct file *filep, char *buf, size_t opt, loff_t *off) {
	data = 0;
	if(opt == SWITCHES){
		data = ioread32(inport_switches);
	} else if(opt == BUTTONS) {
		data = ioread32(inport_buttons);
	}
	
	copy_to_user(buf, &data, sizeof(uint32_t));
	printk(KERN_ALERT "Mandando o dado: %d, referente ao dispositivo %d", data, opt);
	
	return 4;
}

static ssize_t char_device_write(struct file *filep, const char *buf, size_t opt, loff_t *off) {
	data = 0;
  printk(KERN_ALERT "Estamos aqui lindos");
	copy_from_user(&data, buf, sizeof(uint32_t));
	
	if(opt == GREEN_LED) {
		iowrite32(data, hexport_green_led);
	} else if(opt == RED_LED) {
		iowrite32(data, hexport_red_led);
	} else if(opt == SEVEN_DISPLAYS_4) {
		iowrite32(data, hexport_seven_displays_4);
	} else if(opt == SEVEN_DISPLAYS_2) {
		iowrite32(data, hexport_seven_displays_2);
	}
	
	printk(KERN_ALERT "Wrote in device %d the value: %d", opt, data);
	return 4;
}

//-- PCI Device Interface

static int pci_probe(struct pci_dev *dev, const struct pci_device_id *id);
static void pci_remove(struct pci_dev *dev);

static struct pci_device_id pci_ids[] = {
  { PCI_DEVICE(0x1172, 0x0004), },
  { 0, }
};
MODULE_DEVICE_TABLE(pci, pci_ids);

static struct pci_driver pci_driver = {
  .name     = "SoundBoard",
  .id_table = pci_ids,
  .probe    = pci_probe,
  .remove   = pci_remove,
};

static unsigned char pci_get_revision(struct pci_dev *dev) {
  u8 revision;

  pci_read_config_byte(dev, PCI_REVISION_ID, &revision);
  return revision;
}

static int pci_probe(struct pci_dev *dev, const struct pci_device_id *id) {
  int vendor;
  int retval;
  unsigned long resource;

  retval = pci_enable_device(dev);
  
  if (pci_get_revision(dev) != 0x01) {
    printk(KERN_ALERT "altera_driver: cannot find pci device\n");
    return -ENODEV;
  }

  pci_read_config_dword(dev, 0, &vendor);
  printk(KERN_ALERT "altera_driver: Found Vendor id: %x\n", vendor);

  resource = pci_resource_start(dev, 0);
  printk(KERN_ALERT "altera_driver: Resource start at bar 0: %lx\n", resource);

  hexport_green_led = ioremap_nocache(resource + 0XC0F0, 0x20);
  hexport_red_led = ioremap_nocache(resource + 0XC0B0, 0x20);
  hexport_seven_displays_4 = ioremap_nocache(resource + 0XC000, 0x20);
  hexport_seven_displays_2 = ioremap_nocache(resource + 0XC140, 0x20);
  inport_switches = ioremap_nocache(resource + 0XC040, 0x20);
  inport_buttons = ioremap_nocache(resource + 0XC080, 0x20);

  return 0;
}

static void pci_remove(struct pci_dev *dev) {
  iounmap(hexport_green_led);
  iounmap(hexport_red_led);
  iounmap(hexport_seven_displays_4);
  iounmap(hexport_seven_displays_2);

  iounmap(inport_switches);
  iounmap(inport_buttons);
}


//-- Global module registration

static int __init altera_driver_init(void) {
   int t = register_chrdev(MAJOR_NUMBER, "de2i150_altera", &file_opts);
   t = t | pci_register_driver(&pci_driver);

   if(t<0)
      printk(KERN_ALERT "altera_driver: error: cannot register char or pci.\n");
   else
     printk(KERN_ALERT "altera_driver: char+pci drivers registered.\n");

   return t;
}

static void __exit altera_driver_exit(void) {
  printk(KERN_ALERT "Goodbye from de2i150_altera.\n");

  unregister_chrdev(MAJOR_NUMBER, "de2i150_altera");
  pci_unregister_driver(&pci_driver);
}

module_init(altera_driver_init);
module_exit(altera_driver_exit);
