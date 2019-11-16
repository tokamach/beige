#pragma once

#include "types.h"
#include "env.h"

lobj_t* apply(env_t* env, lobj_t* fun, lobj_t* args);
lobj_t* eval(env_t* env, lobj_t* exp); 
