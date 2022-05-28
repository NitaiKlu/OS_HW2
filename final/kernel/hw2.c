#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/init_task.h>

asmlinkage long sys_hello(void)
{
    printk("Hello, World!\n");
    return 0;
}

asmlinkage long sys_set_status(int status)
{
    struct important_task *it_important_task;

    if (status != 0 && status != 1)
    {
        return -EINVAL;
    }
    current->f_status = status;

    if (!list_empty(&init_task.important_tasks))
    {
        list_for_each_entry(it_important_task, &init_task.important_tasks, list)
        {
            if (it_important_task->my_pid == current->tgid)
            {
                it_important_task->status = status;
                return 0;
            }
        }
    }

    return 0;
}

asmlinkage long sys_get_status(void)
{
    return current->f_status;
}

asmlinkage long sys_register_process(void)
{
    struct important_task *new_task;
    struct important_task *it_important_task;

    // No need to add a process twice
    if (!list_empty(&init_task.important_tasks))
    {
        list_for_each_entry(it_important_task, &init_task.important_tasks, list)
        {
            if (it_important_task->my_pid == current->tgid)
            {
                return 0;
            }
        }
    }

    // We assume new_task ia allocated
    new_task = kmalloc(sizeof(*new_task), GFP_KERNEL);

    // Updating new_task's fields
    new_task->status = current->f_status;
    new_task->my_pid = current->tgid;
    list_add(&new_task->list, &init_task.important_tasks);

    return 0;
}

asmlinkage long sys_get_all_cs(void)
{
    long sum = 0;
    struct important_task *it_important_task;

    if (list_empty(&init_task.important_tasks))
    {
        return -ENODATA;
    }

    list_for_each_entry(it_important_task, &init_task.important_tasks, list)
    {
        if (it_important_task->status == 1)
        {
            sum += it_important_task->my_pid;
        }
    }

    return sum;
}
