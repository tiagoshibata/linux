#include<linux/linkage.h>
#include<linux/module.h>

asmlinkage long sys_hello_world(void)
{
  return printk("Hello World From The Kernel!\n");
}
