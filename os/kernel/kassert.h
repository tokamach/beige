#pragma once

#define S1(x) #x
#define S2(x) S1(x)
#define STRLINE #__LINE__
//TODO: correctly error handle
#define assert(x) if(!(x)) {k_println("[Error] Assert Failed: "S2(__FILE__)":"S2(__LINE__)); return NULL;}

