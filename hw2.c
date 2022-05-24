#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/sched.h>
#include <linux/slab.h>


asmlinkage long sys_hello(void)
{
    printk("Hello, World!\n");
    return 0;
}

asmlinkage long sys_set_status(enum faculty_status status)
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
    struct list_head *list_ptr;
    struct task_struct *init_struct;
    struct task_struct *current_task = current;
    while (current_task->pid != 1)
    {
        current_task = current_task->real_parent;
    }
    init_struct = current_task;
    // list_ptr = kmalloc(sizeof(struct list_head), GFP_KERNEL);
    list_ptr = &current->important_tasks;
    if (list_ptr)
    {
        list_add(list_ptr, &(init_struct->important_tasks));
    }
    return 0;
}

asmlinkage long sys_get_all_cs(void)
{
    long sum = 0;
    struct list_head *it;    
    struct task_struct *init_struct;
    struct task_struct *it_pcb;
    struct task_struct *current_task = current;
    while (current_task->pid != 1)
    {
        current_task = current_task->real_parent;
    }
    init_struct = current_task;
    if(list_empty(&init_struct->important_tasks)) {
        return -ENODATA;
    }
    list_for_each(it, &init_struct->important_tasks)
    {
        it_pcb = list_entry(it, struct task_struct, important_tasks);
        if (it_pcb->f_status == CS_faculty)
        {
            sum += it_pcb->tgid;
        }
    }
    if (sum == 0) // error
    {
        return -ENODATA;
    }
    return sum;
}