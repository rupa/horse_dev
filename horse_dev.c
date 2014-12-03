/*
 * horse device. adapted from
 * http://www.linuxdevcenter.com/linux/2007/07/05/examples/hello_dev.tar.gz
 */

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/random.h>

#include <asm/uaccess.h>

static ssize_t horse_read(
    struct file * file, char * buf, size_t count, loff_t *ppos
) {

    char horse[100];
    int len;

    int i;
    int j;
    int k;
    int l;

    get_random_bytes(&i, sizeof(int));
    get_random_bytes(&j, sizeof(int));
    get_random_bytes(&k, sizeof(int));
    get_random_bytes(&l, sizeof(int));

    i = i % 5;
    j = j % 20;
    k = k % 10;
    l = l % 3;

    strcat(horse, "Ne");
    strncat(horse, "eeeeeeeeeeeeeeeeeeee", i);
    strcat(horse, "i");
    strncat(horse, "iiiiiiiiiiiiiiiiiiii", j);
    strcat(horse, "gh");
    strncat(horse, "hhhhhhhhhhhhhhhhhhhh", k);
    strncat(horse, "!!!!!!!!!!!!!!!!!!!!", l);
    strcat(horse, "\n");

    len = strlen(horse);

    // We only support reading the whole string at once.
    if (count < len) return -EINVAL;
    /*
     * If file position is non-zero, then assume the string has
     * been read and indicate there is no more data to be read.
     */
    if (*ppos != 0) return 0;
    /*
     * Besides copying the string to the user provided buffer,
     * this function also checks that the user has permission to
     * write to the buffer, that it is mapped, etc.
     */
    if (copy_to_user(buf, horse, len)) return -EINVAL;
    // Tell the user how much data we wrote.
    *ppos = len;
    return len;
}

// The only file operation we care about is read.
static const struct file_operations horse_fops = {
    .owner = THIS_MODULE,
    .read = horse_read,
};

static struct miscdevice horse_dev = {
    // let kernel pick a minor number for us
    MISC_DYNAMIC_MINOR,
    // name ourselves /dev/horse.
    "horse",
    // functions to call when a program does stuff to the device.
    &horse_fops
};

static int __init
horse_init(void) {
    int ret;
    /*
     * Create the "horse" device in the /sys/class/misc directory.
     * Udev will automatically create the /dev/horse device using
     * the default rules.
     */
    ret = misc_register(&horse_dev);
    if (ret) printk(KERN_ERR "Unable to register horse device\n");
    return ret;
}

module_init(horse_init);

static void __exit
horse_exit(void) {
    misc_deregister(&horse_dev);
}

module_exit(horse_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("horse dev <horse@dev.horse>");
MODULE_DESCRIPTION("horse module");
MODULE_VERSION("dev");
