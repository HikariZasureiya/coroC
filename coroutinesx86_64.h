#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<unistd.h>
#define COR_NUM 100



typedef struct{
    uint64_t rsp;
    uint64_t rbp;
    uint64_t rip;
    uint64_t stackptr;
} cor_pointers;
  
  
typedef struct cor_struct{          // need this for coroutines
    cor_pointers * corpointers;
}cor_struct;

extern void coroutines_init(void * pointer);
extern void coroutines_register(void * funptr , uintptr_t top , void* pointer);
extern void coroutine_yield(void * basepointer);
extern int  get_curr_cor_idx();
extern void set_curr_cor_idx(uint64_t value);
extern int  get_cor_num();
extern void set_cor_num(uint64_t value);
extern void coroutine_kill(void * pointer);



cor_struct* cor_struct_init(){
    cor_struct * newcor = (cor_struct *)malloc(sizeof(cor_struct));
    cor_pointers * newptr = (cor_pointers *)calloc(COR_NUM , sizeof(cor_pointers));
    if(!newcor || !newptr){
        perror("couldn't allocate memory\n");
        exit(1);
    }
    newcor->corpointers = newptr;
    return newcor;
}


void register_coroutines(void * funptr , cor_struct * corstruct){
    uint64_t *stack = (uint64_t *)malloc(4096+16);
    
    uintptr_t aligned = ((uintptr_t)stack + 15) & ~0xF;
    uintptr_t stack_top = aligned + 4096;
    int number = get_cor_num();
    coroutines_register(funptr , stack_top , &(corstruct->corpointers)[number]);
    corstruct->corpointers[number].stackptr = (uint64_t)(uintptr_t)stack;
}

void cor_kill(cor_struct * corstruct){
    int number = get_cor_num();
    int curidx = get_curr_cor_idx();
    free((void *)(uintptr_t)corstruct->corpointers[curidx].stackptr);
    for(int i=curidx ; i<number-1 ; i++){
        corstruct->corpointers[i].rsp = corstruct->corpointers[i+1].rsp;
        corstruct->corpointers[i].rbp = corstruct->corpointers[i+1].rbp;
        corstruct->corpointers[i].rip = corstruct->corpointers[i+1].rip;
        corstruct->corpointers[i].stackptr = corstruct->corpointers[i+1].stackptr;
    }

    
    corstruct->corpointers[number-1].rsp = 0;
    corstruct->corpointers[number-1].rbp = 0;
    corstruct->corpointers[number-1].rip = 0;
    corstruct->corpointers[number-1].stackptr = 0;




    number -= 1;
    if (curidx >= number)  
        curidx = 0;


    set_cor_num((uint64_t)number);
    set_curr_cor_idx((uint64_t)curidx); 
    coroutine_kill(&(corstruct->corpointers)[curidx]);
    return;
}






