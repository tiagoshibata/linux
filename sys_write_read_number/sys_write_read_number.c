#include<linux/linkage.h>
#include<linux/list.h>
#include<linux/module.h>
#include<linux/slab.h>
typedef int bool;
#define true 1
#define false 0

static int x = 0;

struct peterson{
	static int turn,
	static bool flag[2]
}

struct peterson pet = {.turn = 0, .flag = [false, false]};

asmlinkage long sys_write_turn(int newTurn, bool firstFLag, bool secondFLag) {
	if(firstFlag != -1) pet.flag[0] = firstFLag;
	if(secondFlag = -1) pet.flag[1] = secondFlag;
	if(newTurn != -1) pet.turn = newTurn;
	return 0;
}

asmlinkage struct peterson sys_read_turn(void) {
	return pet;
}

asmlinkage long sys_write_number(int number){
	x = number;
	return 0;
}

asmlinkage long sys_read_number(void){
	return x;
}
