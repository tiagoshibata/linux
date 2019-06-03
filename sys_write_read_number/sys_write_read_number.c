#include<linux/linkage.h>
#include<linux/list.h>
#include<linux/module.h>
#include<linux/slab.h>

#define true 1
#define false 0

static int x = 0;

struct peterson{
 	int turn;
	int flag[2];
};

static struct peterson pet = { .turn=0, .flag[0]= false, .flag[1]=false};

asmlinkage long	sys_write_flag(int index, int value){
        pet.flag[index] = value;
        return 0;
}

asmlinkage long sys_read_flag(int index) {
        return pet.flag[index];
}

asmlinkage long sys_write_turn(int turn){
	pet.turn = turn;
	return 0;
}

asmlinkage long sys_read_turn(void) {
	return pet.turn;
}

asmlinkage long sys_write_number(int number){
	x = number;
	return 0;
}

asmlinkage long sys_read_number(void){
	return x;
}
