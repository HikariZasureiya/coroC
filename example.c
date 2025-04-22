#include "coroutinesx86_64.h"

cor_struct * corstruct;     //declare global structure for coroutines

void fun(){
  for(int i=0; i<10; i++){
    printf("routine1: %d\n", i);
    coroutine_yield(corstruct->corpointers);
  }
  cor_kill(corstruct);  
  return;
}

void fun1(){
  for(int i=10; i<20; i++){
    printf("routine2: %d\n", i);
    coroutine_yield(corstruct->corpointers);
  }
  cor_kill(corstruct);
  return;
}
 

int main(){
  // sleep(20);
  corstruct = cor_struct_init();
  coroutines_init(&(corstruct->corpointers)[get_cor_num()]);
  register_coroutines(fun , corstruct);
  register_coroutines(fun1 , corstruct);
  while(1){
    coroutine_yield(corstruct->corpointers);
    int num = get_cor_num();
    if(num <= 1)
      break; 
  }

}


