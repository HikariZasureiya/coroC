#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<unistd.h>
#define COR_NUM 100
#define STACK_SIZE 4096

// a structure to store the essentials which switching stack. nothing much to see 
typedef struct{
    uint64_t rsp;
    uint64_t rbp;
    uint64_t rip;
    uint64_t stackptr;
    uint64_t inaddr;
    uint64_t outaddr;
} cor_pointers;
  
// could've just used a pointer to coroutines but nah a struct works too
typedef struct cor_struct{          
    cor_pointers * corpointers;
}cor_struct;

// asm functions
extern void coroutines_init(cor_pointers * pointer);
extern void coroutines_register(void * funptr , uintptr_t top , cor_pointers* pointer , void * inarg , void * outarg);
extern void coroutine_yield(cor_pointers * basepointer);
extern int  get_curr_cor_idx();
extern void set_curr_cor_idx(uint64_t value);
extern int  get_cor_num();
extern void set_cor_num(uint64_t value);
extern void coroutine_kill(cor_pointers * pointer);


// memory allocator for cor_struct
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

// registers function to be used as coroutine , allocates stack , takes in inaddr , outaddr
void register_coroutines(void * funptr , cor_struct * corstruct , void * inarg , void * outarg){    
    uint64_t *stack = (uint64_t *)malloc(STACK_SIZE+16);
    uintptr_t aligned = ((uintptr_t)stack + 15) & ~0xF;
    uintptr_t stack_top = aligned + 4096;
    int number = get_cor_num();
    coroutines_register(funptr , stack_top , &(corstruct->corpointers)[number] , inarg , outarg);
    corstruct->corpointers[number].stackptr = (uint64_t)(uintptr_t)stack;
    corstruct->corpointers[number].inaddr = (uint64_t)(uintptr_t)inarg;
    corstruct->corpointers[number].outaddr = (uint64_t)(uintptr_t)outarg;
}

  // retrieves input argument address
void * retrieve_args(cor_struct * corstruct){                             
    int curidx = get_curr_cor_idx();
    void * ret = (void *)(uintptr_t)(corstruct->corpointers)[curidx].inaddr;
    return ret;
}

//retrieves the output argument address
void * retcor_val(cor_struct * corstruct){             
    int curidx = get_curr_cor_idx();
    void * ret = (void *)(uintptr_t)(corstruct->corpointers)[curidx].outaddr;
    return ret;
}

//frees stack , changes idx and stuff , manages yield array , switches context
void cor_kill(cor_struct * corstruct){                                      
    int number = get_cor_num();
    int curidx = get_curr_cor_idx();

    void* temp  =(void *)(uintptr_t)corstruct->corpointers[curidx].stackptr;
    free(temp);
    
    for(int i=curidx ; i<number-1 ; i++){
        corstruct->corpointers[i].rsp = corstruct->corpointers[i+1].rsp;
        corstruct->corpointers[i].rbp = corstruct->corpointers[i+1].rbp;
        corstruct->corpointers[i].rip = corstruct->corpointers[i+1].rip;
        corstruct->corpointers[i].stackptr = corstruct->corpointers[i+1].stackptr;
        corstruct->corpointers[i].inaddr = corstruct->corpointers[i+1].inaddr;
        corstruct->corpointers[i].outaddr = corstruct->corpointers[i+1].outaddr;
    }

    
    corstruct->corpointers[number-1].rsp = 0;
    corstruct->corpointers[number-1].rbp = 0;
    corstruct->corpointers[number-1].rip = 0;
    corstruct->corpointers[number-1].stackptr = 0;
    corstruct->corpointers[number -1].inaddr = 0;
    corstruct->corpointers[number -1].outaddr = 0;
    number -= 1;
    if (curidx >= number)  
        curidx = 0;

    set_cor_num((uint64_t)number);
    set_curr_cor_idx((uint64_t)curidx); 
    coroutine_kill(&(corstruct->corpointers)[curidx]);
    return;
}

//idk..run it maybe ?
void coroutine_run(cor_struct *corstruct){
    while(1){
        coroutine_yield(corstruct->corpointers);
        int num = get_cor_num();
        if(num <= 1)
          break; 
      }
}





