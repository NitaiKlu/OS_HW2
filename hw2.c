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
    struct important_task *it_important_task;
    struct task_struct *init_struct;
    struct task_struct *current_task = current;
    // We assume new_task ia allocated
    struct important_task *new_task = kmalloc(sizeof(new_task), GFP_KERNEL);

    while (current_task->pid != 1)
    {
        current_task = current_task->real_parent;
    }
    init_struct = current_task;
    // Now "init_struct" is a pointer to Init's task_struct

    // Updating new_task's fields
    new_task->status = current->f_status;
    new_task->my_pid = current->tgid;
    INIT_LIST_HEAD(&new_task->list);

    if (!list_empty(&init_struct->important_tasks))
    {
        list_for_each_entry(it_important_task, &init_struct->important_tasks, list)
        {
            // No need to add a process twice
            if (it_important_task->my_pid == current->tgid)
            {
                return 0;
            }
        }
    }

    // Adding the list_head ptr to Init's list
    list_add(&new_task->list, &init_struct->important_tasks);
    return 0;
}

asmlinkage long sys_get_all_cs(void)
{
    long sum = 0;
    struct list_head *it;
    struct task_struct *init_struct;
    struct important_task *it_important_task;
    struct task_struct *current_task = current;
    while (current_task->pid != 1)
    {
        current_task = current_task->real_parent;
    }
    init_struct = current_task;
    // Now "init_struct" is a pointer to Init's task_struct

    if (list_empty(&init_struct->important_tasks))
    {
        return -ENODATA;
    }

    list_for_each_entry(it_important_task, &init_struct->important_tasks, list)
    {
        if (it_important_task->status == 1)
        {
            sum += it_important_task->my_pid;
        }
    }
    if (sum == 0) // error
    {
        return -ENODATA;
    }
    return sum;
}
