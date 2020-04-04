#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/hrtimer.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/jiffies.h>
#include <linux/unistd.h>
#include <linux/sched.h>
#include <linux/ktime.h>
#include <linux/timekeeping.h>

MODULE_LICENSE("GPL");

#define INTERVAL_BETWEEN_CALLBACKS (100 * 10000000LL) //1000ms (scaled in ns), i.e 1 second

int fib1=0;
int fib2=1;
int temp = 0;
int count = 0;
struct timespec tv; // used to get current time.

// Define a hrtimer
static struct hrtimer timer;
static ktime_t kt;

static s64 starttime_ns;

static enum hrtimer_restart my_hrtimer_callback( struct hrtimer *timer )
{
    printk(KERN_INFO "My timer has been executed %d times!\n", count);
    count++;
    printk("Fibonacci: %d \n\n", fib2);
    s64 now_ns = ktime_to_ns(ktime_get());
    kt = ktime_set(0, fib2*INTERVAL_BETWEEN_CALLBACKS);

    hrtimer_forward(timer, timer->_softexpires, kt);

    temp = fib2;
    fib2 = fib2+fib1;
    fib1 = temp;

    return HRTIMER_RESTART;
}
 
// Function that activates when we insert module in kernel
// Sets hrtimer for the first use.
static int init_module_hrtimer( void )
{
	printk("mod_hrtimer: installing module...\n");
    // define a ktime variable with the interval time defined on top of this file
	kt = ktime_set(0, INTERVAL_BETWEEN_CALLBACKS);
    // init a high resolution timer named 'timer'
    hrtimer_init(&timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
    //activate the high resolution timer
    hrtimer_start(&timer, kt, HRTIMER_MODE_REL);
    // set the callback function for timer
    timer.function = my_hrtimer_callback;
    return 0;
}
 

// when write sudo rmmod ctimer
// this function activates
// it prints message The timer was still in use... depending on ret value
// ret value will be none zero if timer was active 
// and deletes timer 
static void cleanup_module_hrtimer( void )
{
	int ret;
	ret = hrtimer_cancel( &timer );
}
 
module_init(init_module_hrtimer);
module_exit(cleanup_module_hrtimer);
