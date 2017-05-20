#include <linux/linkage.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/slab.h>

static LIST_HEAD(stack);

struct stack_value {
    int value;
    struct list_head stack;
};

asmlinkage long sys_stack_push(int value)
{
    struct stack_value* new_stack_value;
    
    new_stack_value = kmalloc(sizeof(*new_stack_value), GFP_KERNEL);
    new_stack_value->value = value;
    INIT_LIST_HEAD(&new_stack_value->stack);

    list_add(&new_stack_value->stack, &stack);
    return 0;
}

asmlinkage long sys_stack_pop(void)
{
    int value;
    struct stack_value* top_stack_value;

    if (list_empty(&stack)) {
        printk("EMPTY STACK\n");
        return -1;
    }

    top_stack_value = list_first_entry(&stack, struct stack_value, stack); 
    value = top_stack_value->value;
    list_del(&top_stack_value->stack);
    kfree(top_stack_value);

    return value;
}
