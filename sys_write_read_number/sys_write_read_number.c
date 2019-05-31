#include<linux/linkage.h>
#include<linux/list.h>
#include<linux/module.h>
#include<linux/slab.h>

static int x;
static struct kobject *sys_stack_kobject;


asmlinkage long sys_write_number(int number){
	x = number;
	return 0;
}

asmlinkage long sys_read_number(void){
	return x;
}

static ssize_t show(struct kobject *kobj, struct kobj_attribute *attr,
                            char *buf)
{
        return sprintf(buf, "%lu\n", x);
}

static struct kobj_attribute x_attribute = __ATTR_RO(x);


static struct attribute *attrs_x[] = {
        &x_attribute.attr,
        NULL,
};

static int __init sys_stack_init(void){
        int retval_x;

        sys_write_read_number_kobject= kobject_create_and_add("sys_write_read_number", kernel_kobj);
        if (!sys_write_read_number_kobject)
                pr_debug("stack_dev: can't create kobject");

        retval_x = sysfs_create_file(sys_write_read_number_kobject, *attrs_count);
        if (retval_count) {
                pr_debug("stack_dev count: can't create sysfs group");
                kobject_put(sys_write_read_number_kobject);
                return retval_x;
        }

        return 0;
}

static void __exit sys_write_read_number_exit(void)
{
        kobject_put(sys_write_read_number_kobject);
}

module_init(sys_stack_init);
module_exit(sys_write_read_number_exit);
MODULE_LICENSE("GPL");
