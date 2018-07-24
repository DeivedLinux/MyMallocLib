#ifndef EXCEPTION_H_
#define EXCEPTION_H_ 

#include <setjmp.h>

#define ACCESS_CONTROL_BOUND 

#define try jmp_buf jmp; switch(setjmp(jmp)){  case 0:
#define catch(e) break; case e:
#define throw(e) longjmp(jmp,e)



#endif