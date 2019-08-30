#pragma once

#ifdef DEBUG
#define KDEBUG_PRINT(s) (k_println("[DEBUG][__FILE__][__FUNCTION__] s"));
#else
#define KDEBUG_PRINT(s)
#endif
