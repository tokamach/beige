#pragma once

#include "types.h"
#include "env.h"

cons_t* apply(env_t* env, cons_t* fun, cons_t* args);
cons_t* eval(env_t* env, cons_t* exp); 
