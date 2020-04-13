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

#define PAGE_SIZE 4096

struct mystruct {
   unsigned int arrayOfIntegers[8];
};

struct mystruct m;


static int param = 2000; 
module_param(param, int, 0660);


struct page* mypages;

void part1(void){
   printk(KERN_INFO "Starting Lab 10 - Page Allocation.\n");
   printk(KERN_INFO "Part 1:");
   printk(KERN_INFO "Parameter: %d\n.", param);
}

void part2(int temp){
   printk(KERN_INFO "Part 2:");
   printk(KERN_INFO "Struct size : %d. \nNumber of instances of my struct that would fit in with in a single kernel memory page: %d.\n", 
                      sizeof(m), temp);
}

void part3(int temp){
   printk(KERN_INFO "Part 3:");

   struct timespec t1, t2;
   int temp2 = param/temp + 1;
   int pow = 0;
   if(temp2 == 8) {
      pow = 3;
   } else if(temp2 == 16) {
      pow = 4;
   } else if(temp2 == 32) {
      pow = 5;
   }
   printk(KERN_INFO "Number of pages to be allocated: 2^%d=%d", pow, temp2);
   getnstimeofday(&t1);

   mypages = alloc_pages(GFP_KERNEL, pow);
   __free_pages(mypages, pow);

   getnstimeofday(&t2);
   printk(KERN_INFO "Diff between start and finish: %lu.",t2.tv_nsec - t1.tv_nsec);
}

int init_module(void) {
   int temp1 = PAGE_SIZE/sizeof(m);

   part1();
   part2(temp1);
   part3(temp1);

   
   return 0;
}

void cleanup_module(void) {
   printk(KERN_INFO "Cleanup module. End of lab 10.\n");
}
