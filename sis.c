#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/init_task.h>
#include <linux/rcupdate.h>
#include <linux/fdtable.h>
#include <linux/fs.h> 
#include <linux/fs_struct.h>
#include <linux/dcache.h>
#include <linux/slab.h>
#include <linux/types.h>

void show_open_files (struct files_struct *files){
	struct fdtable *files_table;
	int i=0;
	struct path files_path;
	
	char *cwd;
	char *buf = (char *)kmalloc(GFP_KERNEL,100*sizeof(char));

	files_table = files_fdtable(files);

	while(files_table->fd[i] != NULL) { 
	files_path = files_table->fd[i]->f_path;
	cwd = d_path(&files_path,buf,100*sizeof(char));

	printk(KERN_ALERT "   Open file with fd %d  %s", i,cwd);

	i++;
	}
}
void DFS(struct task_struct *task)
{   
	struct task_struct *child;
	struct list_head *list;
	struct fdtable *files_table;
	files_table = files_fdtable(task->files);
	if (files_table->fd[0] != NULL)
		printk( KERN_INFO "Process %s has open files. Here they are:", 
				   task->comm); 
	show_open_files(task->files);
	list_for_each(list, &task->children) {
	child = list_entry(list, struct task_struct, sibling);
	DFS(child);
	}
}



int init_module(void)
{
    printk(KERN_INFO "Now we will list processes with their open files\n");
    DFS(&init_task);

    return 0;
}

void cleanup_module(void)
{
        printk(KERN_INFO "That's it\n");
}

