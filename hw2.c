#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/sched.h>
#include <linux/slab.h>


asmlinkage long sys_hello(void)
{
    printk("Hello, World!\n");
    return 0;
}

asmlinkage long sys_set_status(int status)
{
    if (status != 0 && status != 1)
    {
        return -EINVAL;
    }
    current->f_status = status;
    return 0;
}

asmlinkage long sys_get_status(void)
{
    return current->f_status;
}

asmlinkage long sys_register_process(void)
{
	struct important_task* it_pcb;
	struct list_head *it;
    struct list_head *list_ptr;
    struct task_struct *init_struct;
    struct task_struct *current_task = current;
    struct important_task *new_task = kmalloc(sizeof(important_task*), GFP_KERNEL);

    while (current_task->pid != 1)
    {
        current_task = current_task->real_parent;
    }
    init_struct = current_task;
    
    
	new_task->status = current->f_status;
	new_task->my_pid = current->tgid;
	LIST_HEAD_INIT(&new_task->list);
	
	if(list_empty(&(init_struct->important_tasks)) {
		list_add(&(new_task->list), &(init_struct->important_tasks));
		return 0;
	}
	
	list_for_each_entry(it_pcb, &init_struct->important_tasks,list)
    {
        if (it_pcb->my_pid == current->pid)
        {
            return 0;
        }
    }

	list_add(&(new_task->list), &(init_struct->important_tasks));
    return 0;
}

asmlinkage long sys_get_all_cs(void)
{
    long sum = 0;
    struct list_head *it;    
    struct task_struct *init_struct;
    struct important_task *it_pcb;
    struct task_struct *current_task = current;
    while (current_task->pid != 1)
    {
        current_task = current_task->real_parent;
    }
    init_struct = current_task;
    if(list_empty(&init_struct->important_tasks)) {
        return -ENODATA;
    }

    list_for_each_entry(it_pcb, &init_struct->important_tasks, list)
    {
        if (it_pcb->status == 1)
        {
            sum += it_pcb->my_pid;
        }
    }
    if (sum == 0) // error
    {
        return -ENODATA;
    }
    return sum;
}
