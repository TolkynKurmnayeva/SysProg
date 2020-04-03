#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/random.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/spinlock.h>

#define TAG "Lab8"
#define MAX_NUM 100

static struct task_struct *t1;
static struct task_struct *t2;
static struct task_struct *t3;
static struct task_struct *t4;

DEFINE_SPINLOCK(lock);

int a1[MAX_NUM];
int a2[MAX_NUM];

struct sortParams {
    int start;
    int end;
};


void printAfter(void) {
    int i = 0;
    printk(KERN_INFO "After :\n");
    for(i = 0; i < 100; i++) {
        printk("a[%d] : %d", i, a1[i]);
    }
}

void printBefore(void) {
    int i = 0;
    printk(KERN_INFO "Before: \n");
    for(i = 0; i < 100; i++) {
        printk("a[%d] : %d", i, a2[i]);
    }
}


static int threadSort(void *args) {
    spin_lock(&lock);
    struct sortParams *params = (struct sortParams*) args;
    int start = params->start;
    int end = params->end;
    int i = params->start;
    int temp_element;
    int j;
    while(i <= end) {
        temp_element = a2[i];
        j = i-1;
        while(j >= start && temp_element < a2[j]){
            a2[j+1] = a2[j];
            j--;
        }
        a2[j+1] = temp_element;
        i++;
    }
    spin_unlock(&lock);
    return 0;
}

static int threadMerge(void *unused) {
    spin_lock(&lock);
    int i = 0;
    int j = MAX_NUM/2 + 1;
    int n = MAX_NUM - 1;
    int k = 0;
    while(i < MAX_NUM/2 + 1 && j <= n) {
        if(a2[i] < a2[j]) {
            a1[k++] = a2[i++];
        }
        else {
            a1[k++] = a2[j++];
        }
    }
    while(i < (MAX_NUM/2 + 1)) {
        a1[k++] = a2[i++];
    }
    while(j <= n) {
        a1[k++] = a2[j++];
    }
    spin_unlock(&lock);
    return 0;
}

int getRandom (void){
    int n;
    get_random_bytes(&n, sizeof(n));
    n = n*n*n*n;
    n = n % 100000;
    n = n < 0 ? n*-1 : n;
    return n;
}
void fillArray(void){
    int i = 0;
    for(i = 0; i < MAX_NUM; i++) {
        a2[i] = getRandom();
    }
}

int init_module(void)
{
    struct sortParams arg1; 
    struct sortParams arg2;
    printk(KERN_INFO "START %s\n", TAG);
    fillArray();
    printBefore();
    arg1.start = 0;
    arg1.end = MAX_NUM/2;
    arg2.start = MAX_NUM/2 + 1;
    arg2.end = MAX_NUM - 1; 
    t1 = kthread_run(threadSort, &arg1, "sortBegin");
    t2 = kthread_run(threadSort, &arg2, "sortEnd");
    t3 = kthread_run(threadMerge, NULL, "merge1");   
    t4 = kthread_run(threadMerge, NULL, "merge2");  
    printAfter();
    kthread_stop(t1);
    kthread_stop(t2);
    kthread_stop(t3);
    kthread_stop(t4);   
    return 0;
}

void cleanup_module(void)
{
    printk(KERN_DEBUG "END %s\n", TAG);
}