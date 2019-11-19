#pragma once

#define S1(x) #x
#define S2(x) S1(x)
#define STRLINE #__LINE__
//TODO: add string error message, correctly error handle
#define assert(x, err) if(!(x)) {k_println("[ASSERT]:"#err S2(__FILE__)":"S2(__LINE__)); return NULL;}

