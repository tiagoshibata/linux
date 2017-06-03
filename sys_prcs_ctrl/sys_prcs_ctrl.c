#include <linux/linkage.h>
#include <linux/module.h>

asmlinkage long sys_prcs_ctrl(void)
{
	return pr_info("Hello World From The Kernel!\n");
}
