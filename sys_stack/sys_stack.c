#include <linux/linkage.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/slab.h>

static LIST_HEAD(stack);

struct stack_value {
        int value;
        struct list_head stack;
};

static int first_value;
static int last_value;
static unsigned long count;
static struct kobject *sys_stack_kobject;

asmlinkage long sys_stack_push(int value)
{
        struct stack_value *new_stack_value;

        new_stack_value = kmalloc(sizeof(*new_stack_value), GFP_KERNEL);
        new_stack_value->value = value;

        list_add(&new_stack_value->stack, &stack);

        count++;
        if (count == 1) {
                first_value = value;
        }
        last_value = value;

        return 0;
}

asmlinkage long sys_stack_pop(void)
{
        int value;
        struct stack_value *top_stack_value;

        if (list_empty(&stack)) {
                pr_debug("EMPTY STACK\n");
                return -1;
        }

        top_stack_value = list_first_entry(&stack, struct stack_value, stack);
        value = top_stack_value->value;
        list_del(&top_stack_value->stack);
        count--;
        if (count == 0) {
                first_value = -1;
                last_value = -1;
        }
        else{
                last_value = list_first_entry(&stack, struct stack_value, stack)->value;
        }

        kfree(top_stack_value);
        return value;
}

static ssize_t count_show(struct kobject *kobj, struct kobj_attribute *attr,
                            char *buf)
{
        return sprintf(buf, "%lu\n", count);
}

static ssize_t first_value_show(struct kobject *kobj, struct kobj_attribute *attr,
                            char *buf)
{
        return sprintf(buf, "%d\n", first_value);
}

static ssize_t last_value_show(struct kobject *kobj, struct kobj_attribute *attr,
                            char *buf)
{
        return sprintf(buf, "%d\n", last_value);
}

static struct kobj_attribute count_attribute = __ATTR_RO(count);

static struct kobj_attribute first_value_attribute = __ATTR_RO(first_value);

static struct kobj_attribute last_value_attribute = __ATTR_RO(last_value);

static struct attribute *attrs_count[] = {
        &count_attribute.attr,
        NULL,
};
static struct attribute *attrs_first[] = {
        &first_value_attribute.attr,
        NULL,
};

static struct attribute *attrs_last[] = {
        &last_value_attribute.attr,
        NULL,
};

static int __init sys_stack_init(void)
{
        int retval_count;
        int retval_first;
        int retval_last;

        sys_stack_kobject = kobject_create_and_add("sys_stack", kernel_kobj);
        if (!sys_stack_kobject)
                pr_debug("stack_dev: can't create kobject");

        retval_count = sysfs_create_file(sys_stack_kobject, *attrs_count);
        if (retval_count) {
                pr_debug("stack_dev count: can't create sysfs group");
                kobject_put(sys_stack_kobject);
                return retval_count;
        }

        retval_first = sysfs_create_file(sys_stack_kobject, *attrs_first);
        if (retval_first) {
                pr_debug("stack_dev first: can't create sysfs group");
                kobject_put(sys_stack_kobject);
                return retval_first;
        }

        retval_last = sysfs_create_file(sys_stack_kobject, *attrs_last);
        if (retval_last) {
                pr_debug("stack_dev last: can't create sysfs group");
                kobject_put(sys_stack_kobject);
                return retval_last;
        }

        return 0;
}

static void __exit sys_stack_exit(void)
{
        kobject_put(sys_stack_kobject);
}

module_init(sys_stack_init);
module_exit(sys_stack_exit);
MODULE_LICENSE("GPL");
