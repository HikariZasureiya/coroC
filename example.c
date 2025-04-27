#include "coroutinesx86_64.h"

cor_struct * corstruct;     //declare global structure for coroutines

void fun(){
  int yeh =  *(int *)(retrieve_args(corstruct));  //retrieve_args to get the input argument pointer
  printf("input argument : %d\n" , yeh);          
  for(int i=0; i<yeh; i++){
    printf("routine1: %d\n", i);
    coroutine_yield(corstruct->corpointers);
  }
  int * ret = (int *)retcor_val(corstruct);     //rector_val to get the output argument pointer
  *ret = 50;                                    //modifying the output
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
  int temp = 20;
  int ret = 0;
  register_coroutines(fun , corstruct ,&temp , &ret);
  register_coroutines(fun1 , corstruct , NULL, NULL);

  coroutine_run(corstruct);    //jus run it
  printf("return from fun is: %d\n", ret);

}


