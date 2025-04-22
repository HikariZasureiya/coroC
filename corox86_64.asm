; ___________             .__         .__                        _________                             __  .__                      
; \__    ___/___________  |  | _____  |  |   ___________  ____   \_   ___ \  ___________  ____  __ ___/  |_|__| ____   ____   ______
;   |    |  \_  __ \__  \ |  | \__  \ |  | _/ __ \_  __ \/  _ \  /    \  \/ /  _ \_  __ \/  _ \|  |  \   __\  |/    \_/ __ \ /  ___/
;   |    |   |  | \// __ \|  |__/ __ \|  |_\  ___/|  | \(  <_> ) \     \___(  <_> )  | \(  <_> )  |  /|  | |  |   |  \  ___/ \___ \ 
;   |____|   |__|  (____  /____(____  /____/\___  >__|   \____/   \______  /\____/|__|   \____/|____/ |__| |__|___|  /\___  >____  >
;                       \/          \/          \/                       \/                                        \/     \/     \/ 
;                                                      %                                                          
;                                                   %@%%                                                          
;                                                 *@%##%                                                          
;                                                #%%####                                                          
;                                              =%%#####*                                             *            
;                                             =%%######*                                           %@             
;                                            *@%%######                                          %@#              
;                                 =------===#%@%%%####*                                        *@@#               
;                         =-::--===+++++*****#%@@@%%%##*                                      %@%#                
;                   .:::--==++*###%%%%%%%%%%%%%%%%%%%###*                                    %%%#                 
;                ..::----=+*%%@@@@@@@@@@%%%%%%%%%%%%%%##*+=                                 @%%%                  
;              --=++-#@@%%%%%%%%@@@@@@@@@@@@@%%%%%%%%%%%%##*+=                            #@%%#                   
;          #%@@@@@%%%%%@@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%####*=                         #@%%#                    
;     #%@@%%%%%####%%%%@@%%%%%%%%%##%%%%%%####%%%%%%%%%%%%%%%###*+                      %@%%%+                    
;  *########%++#####%%%%%%###############%#####%%%%%%%%%%%%%%%####*+                   %@%%%*                     
;   -==--=*==@*=+**+++============+*##=+**+####%%%%%%%%%%%%%########*+                @@%%%#                      
;      -----========+***#**++++===========++++*%@%%%%%%%%%#############*+  +%%#      @@%%%#                       
;           ===+===========================+++#@@@@%%%%%%%%#################%%#    #@@%###                        
;              ---============+++++++++++++++*%%@@@@%%%%%#*##################%%%%%%%%###%*                        
;                   ----=======++++++++++++++++==#@@@%%%*++++++**##################*++*##                         
;                            --====++++++++++++***+%@@%##*+++++++++*#%%%#*++++++++++++*##                         
;                             =+=++===++++++++******#@@%###******++++%%%%%++++++    +**##                         
;                             ++++++++===+++++++****+@@@%###********#@%%%%%# +**      %%%#                        
;                             *++++++        =++++++++@@@%##*++++++**+****#%%%%%+       %%                        
;                            -+++++-                  *@@%%##*    **+    -#***###                                 
;                           :*%%*++                    %@%%%##           =#######*                                
;                         .-**%#+++#:*                 =%%%%%#*        -+*####***#                                
;                       .:-*#####**##**                 #%%%%%#        --=*###*-*=                                
;                     .::*##########+#*=                +#%%###      ####*###**+++                                
;                   ::+**##*+###*=+####*              ###%%%##       @@@#*****%                                   
;                ..:=*#####+---*%%%*++=-           =*#*###%%##%%%+     *                                          
;           .:-+****#######%%*+++++++===           *########%#@@%#+                                               
;           @#***#######*+++********             +###########%@@###*                                              
;             @%*+++++++*#                     =*###################                                              
;                                            *###################*##                                              
;                                          =***################=####*                                             
;                                        ::+**#########=+##*==#%###++                                             
;                                     :::-=****########----=#%#*++++*                                             
;                                 ::---=++***###########*##*++++++++*                                             
;                               .:=+*****###############*++++*****                                                
;                                      %#*******##%@   
;                               @@###################*++++**#                                                     
;                               *@@***#####***++++++*****                                                         

section .data
  COR_NUM           EQU       100 
  current_cor       dq        0
  number_cor        dq        0
  newline           db        10
  struct_byte       dq        24



section .bss


section .text
  global coroutines_init , coroutine_yield , coroutines_register , get_cor_num , set_cor_num , get_curr_cor_idx , set_curr_cor_idx , coroutine_kill


get_cor_num:
  mov rax , [number_cor]
  ret
set_cor_num:
  mov [number_cor] , rdi
  ret
get_curr_cor_idx:
  mov rax , [current_cor]
  ret

set_curr_cor_idx:
  mov [current_cor] , rdi
  ret

;; makes the main thread a coroutine
coroutines_init:
  mov rbx , [number_cor]

  cmp rbx , COR_NUM
  jge exiterr
  
  pop rax       ;pops the return address
  mov [rdi] , rsp
  mov [rdi+8] , rbp
  mov [rdi+16] , rax

  inc rbx
  mov [number_cor] , rbx

  jmp rax


coroutines_register:
  push rax
  push rbx
  mov rbx , [number_cor]
  cmp rbx , COR_NUM
  jge exiterr
  mov rax , rsi

  mov [rdx] , rax
  mov [rdx+8] , rax
  mov [rdx+16] , rdi

  inc rbx
  mov [number_cor] , rbx
  pop rbx
  pop rax
  
  ret



coroutine_kill:
  pop rax
  mov rsp , [rdi]
  mov rbp , [rdi+8]
  mov rax , [rdi+16]
  jmp rax 


coroutine_yield:
  ;; first need to preserve the return address

  mov rbx , [current_cor] 
  lea rsi, [rdi+rbx*8]
  lea rsi, [rsi+rbx*8]
  lea rsi, [rsi+rbx*8]
  lea rsi, [rsi+rbx*8]


  pop rax
  mov [rsi] , rsp
  mov [rsi+8] , rbp
  mov [rsi+16] , rax

  inc rbx
  xor rdx , rdx
  cmp rbx , [number_cor]
  cmovge rbx , rdx
  mov [current_cor] , rbx
  lea rsi, [rdi+rbx*8]
  lea rsi, [rsi+rbx*8]
  lea rsi, [rsi+rbx*8]
  lea rsi, [rsi+rbx*8]
 

  mov rsp , [rsi]
  mov rbp , [rsi+8]
  mov rax , [rsi+16]
  jmp rax   
  
exit0:
  mov rax , 60            ;exit
  mov rdi , 0
  syscall

exiterr:
  mov rax , 60            ;exit
  mov rdi , 1
  syscall


  
  
