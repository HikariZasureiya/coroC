# 🧵 coroC - A lightweight Coroutine System in C (x86_64)

A simple coroutine system implemented in C using x86_64 NASM assembly.

![image](https://github.com/user-attachments/assets/464d53ce-33f6-4424-896f-5a66e4c5acbb)

## Features

- Lightweight cooperative multitasking
- Manual context switching
- Supports multiple coroutines
- Designed for x86_64 Linux

## Requirements
```
GCC
NASM
Linux (x86_64)
```
## Build
```
make all
```
## Structures
```C
typedef struct{
    uint64_t rsp;
    uint64_t rbp;
    uint64_t rip;
    uint64_t stackptr;
    uint64_t inaddr;
    uint64_t outaddr;
} cor_pointers;


typedef struct cor_struct{          
    cor_pointers * corpointers;
}cor_struct;
```
## API Reference

```C
cor_struct* cor_struct_init() // initialize coroutine structure

void coroutines_init(cor_pointers * coroutinepointerarray) // call in the beginning of a subroutine before registering coroutines

void register_coroutines(void * functionptr , cor_struct * corstruct , void * inarg , void * outarg) // register coroutines 

void * retrieve_args(cor_struct * corstruct) //retrieves input arguments of a coroutine

void * retcor_val(cor_struct * corstruct)  //returns a pointer to output arguments of a coroutine

void cor_kill(cor_struct * corstruct)  // kills a coroutine before returning

int get_cor_num();  // gets number of coroutines in scheduler

void set_cor_num(uint64_t value); // sets number of coroutines in scheduler

int get_curr_cor_idx() // gets index of current context

void set_curr_cor_idx(uint64_t value) // set index of current context

void coroutine_yield(cor_pointers * basepointer) //changes context of current coroutine

void coroutine_run(cor_struct *corstruct) //execute the coroutines
```

## Usage
Check out example.c

## Limitations

- **Limited Stack Size per Coroutine**  
  Each coroutine has a fixed-size stack. Using too much stack space may cause crashes.

- **Manual Context Switching**  
  Coroutines must manually yield control. No automatic preemption.

- **Single-threaded Design**  
  Only supports single-threaded execution.

- **Platform Specific (x86_64 Linux)**  
  Requires x86_64 architecture and Linux OS.

