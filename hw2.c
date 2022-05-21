#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/sched.h>
#include <linux/slab.h>

asmlinkage long sys_hello(void) {
 printk("Hello, World!\n");
 return 0;
}

asmlinkage int sys_set_status(enum faculty_status status)
{
    if(status != 0 && status != 1) {
        return -1 * EINVAL;
    }
    current->f_status = status;
    return 0;
}

asmlinkage int sys_get_status(void)
{
    return current->f_status;
}

asmlinkage int sys_register_process(void)
{
    struct list_head *new_data;
    new_data = kmalloc(sizeof(struct list_head), GFP_KERNEL);
        if(new_data) {
                list_add(new_data, &(current->important_tasks));
        }
    return 0;
}

asmlinkage long sys_get_all_cs(void)
{
    long sum = 0;
    struct list_head* it;
    list_for_each(it, &(current->important_tasks)) {
        struct task_struct* it_pcb = list_entry(it, struct task_struct, important_tasks);
        if(it_pcb->f_status == 1) {
            sum += it_pcb->tgid;
        }
    }
    if(sum == 0) //error
    {
        return -1 * ENODATA;
    }
    return sum;
}