#include<linux/linkage.h>
#include<linux/list.h>
#include<linux/module.h>
#include<linux/slab.h>

static int x = 0;

asmlinkage long sys_write_number(int number){
	x = number;
	return 0;
}

asmlinkage long sys_read_number(void){
	return x;
}
