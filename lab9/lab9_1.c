#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/unistd.h>
#include <linux/sched.h>
#include <linux/ktime.h>
#include <linux/timekeeping.h>

#define TAG "week10"

static struct timer_list mytimer;
struct timespec tv; // structure to get current time
int count = 1;




void print_time(void) {
   getnstimeofday(&tv);
   long long hours = (tv.tv_sec/3600 + 6)%24;
   long long minutes = (tv.tv_sec/60)%60;
   long long seconds = tv.tv_sec%60; 
   pr_info("Time: %lld:%lld:%lld", hours, minutes, seconds);
}
void exampleTimerFunc(struct timer_list *t) {
   printk(KERN_INFO "My timer has been executed %d times!\n", count);
   print_time();
   count++;
   mytimer.expires = jiffies + msecs_to_jiffies(1000);
   timer_setup(&mytimer, exampleTimerFunc, 0);
      add_timer(&mytimer);

}


/*
this example works for linux kernel 4.14 and higher 
for older version please follow the link below:
https://stackoverflow.com/questions/53839625/adaptation-from-old-init-timer-to-new-timer-setup
*/
void exampleWithTimer(void) {
   //int ret;
   printk( "Starting timer to fire in 1000ms (%ld)\n", jiffies );
   mytimer.expires = jiffies + msecs_to_jiffies(1000);
   timer_setup(&mytimer, exampleTimerFunc, 0);

   //ret = mod_timer(&mytimer, jiffies + msecs_to_jiffies(200) );
   //if (ret) {
   //  printk("Error in mod_timer\n");
   //}

   add_timer(&mytimer);
}

int init_module(void) {
   printk(KERN_INFO "Starting %s\n", TAG);
   exampleWithTimer();
   return 0;
}

void cleanup_module(void) {
   printk(KERN_INFO "Cleanup %s\n", TAG);
   del_timer(&mytimer);
}
