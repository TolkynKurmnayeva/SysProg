#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/unistd.h>
#include <linux/sched.h>
#include <linux/ktime.h>
#include <linux/timekeeping.h>
#include <linux/gfp.h>
#include <linux/mm_types.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>


#define PAGE_SIZE 4096

struct mystruct {
   unsigned int arrayOfIntegers[8];
};

struct mystruct m;

static int param = 2000; //default value = 2000
module_param(param, int, 0660);

// Part 4
void kmallocAlloc(int size) {
   struct timespec t1, t2;
   getnstimeofday(&t1);
   int i;
   char *buf;
   for(i = 0; i < param; i++) {

       buf = kmalloc(size, GFP_KERNEL);
       kfree(buf);
   }

   getnstimeofday(&t2);
   printk(KERN_INFO "Diff between start and finish: %lu." ,t2.tv_nsec - t1.tv_nsec);
}

int init_module(void) {
   printk(KERN_INFO "Starting Lab 10 - KMalloc.\n");

   int temp1 = PAGE_SIZE/sizeof(m);
   kmallocAlloc(sizeof(m));
   return 0;
}

void cleanup_module(void) {
   printk(KERN_INFO "Cleanup module. End of lab 10.\n");
}
