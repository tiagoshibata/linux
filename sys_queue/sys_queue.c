#include <linux/linkage.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/slab.h>

static LIST_HEAD(queue);

struct queue_value {
	int value;
	struct list_head queue;
};

static unsigned long count;
static struct kobject *sys_queue_kobject;

asmlinkage long sys_queue_push(int value)
{
	struct queue_value *new_queue_value;

	new_queue_value = kmalloc(sizeof(*new_queue_value), GFP_KERNEL);
	new_queue_value->value = value;

	list_add_tail(&new_queue_value->queue, &queue);
	count++;

	return 0;
}

asmlinkage long sys_queue_pop(void)
{
	int value;
	struct queue_value *top_queue_value;

	if (list_empty(&queue)) {
		pr_debug("EMPTY QUEUE\n");
		return -1;
	}

	top_queue_value = list_first_entry(&queue, struct queue_value, queue);
	value = top_queue_value->value;
	list_del(&top_queue_value->queue);
	kfree(top_queue_value);
	count--;

	return value;
}

static ssize_t count_show(struct kobject *kobj, struct kobj_attribute *attr,
			    char *buf)
{
	return sprintf(buf, "%lu\n", count);
}

static struct kobj_attribute count_attribute = __ATTR_RO(count);

static struct attribute *attrs[] = {
	&count_attribute.attr,
	NULL,
};

static int __init sys_queue_init(void)
{
	int retval;

	sys_queue_kobject = kobject_create_and_add("sys_queue", kernel_kobj);
	if (!sys_queue_kobject)
		pr_debug("queue_dev: can't create kobject");

	retval = sysfs_create_file(sys_queue_kobject, *attrs);
	if (retval) {
		pr_debug("queue_dev: can't create sysfs group");
		kobject_put(sys_queue_kobject);
		return retval;
	}

	return 0;
}

static void __exit sys_queue_exit(void)
{
	kobject_put(sys_queue_kobject);
}

module_init(sys_queue_init);
module_exit(sys_queue_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple queue module");
