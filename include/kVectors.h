//----------------------------------------------------------------------------------------------------
/*
Copyright (C) 31.08.2021 Aleksei Likhachev
 
This program is distributed in the hope that it will be useful, 
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the KI General Public License version for more details.
*/
//----------------------------------------------------------------------------------------------------


#ifndef __VEC_TEMPLATES_H_
#define __VEC_TEMPLATES_H_

#define T double //Defolt type
#ifdef T 
	#include "Vectors.h"
	#include "../libc/Vectors.c"
	#undef T
#endif

#define T uint8_t
#ifdef T 
    #include "Vectors.h"
    #include "../libc/Vectors.c"
	#undef T
#endif

#define T int8_t
#ifdef T
    #include "Vectors.h"
    #include "../libc/Vectors.c"
	#undef T
#endif

#define T uint32_t
#ifdef T 
    #include "Vectors.h"
    #include "../libc/Vectors.c"
	#undef T 
#endif

#define T int32_t
#ifdef T
    #include "Vectors.h"
    #include "../libc/Vectors.c"
	#undef T
#endif

#define T uint64_t
#ifdef T
    #include "Vectors.h"
    #include "../libc/Vectors.c"
	#undef T
#endif

#define T int64_t
#ifdef T
    #include "Vectors.h"
    #include "../libc/Vectors.c"
	#undef T
#endif

#define T float
#ifdef T
#define T2 xln
    #include "Vectors.h"
    #include "../libc/Vectors.c"
	#undef T
#undef T2
#endif



#endif 
