#ifndef DEF_SERIAL_H
#define DEF_SERIAL_H


#ifdef __cplusplus
extern "C"
{
#endif

#if !defined(__WINDOWS__) && (defined(WIN32) || defined(WIN64) || defined(_MSC_VER) || defined(_WIN32))
    #define __WINDOWS__
#endif

#ifdef __WINDOWS__

/* When compiling for windows, we specify a specific calling convention to avoid issues where we are being called from a project with a different default calling convention.  For windows you have 3 define options:

CJSON_HIDE_SYMBOLS - Define this in the case where you don't want to ever dllexport symbols
CJSON_EXPORT_SYMBOLS - Define this on library build when you want to dllexport symbols (default)
CJSON_IMPORT_SYMBOLS - Define this if you want to dllimport symbol

For *nix builds that support visibility attribute, you can define similar behavior by

setting default visibility to hidden by adding
-fvisibility=hidden (for gcc)
or
-xldscope=hidden (for sun cc)
to CFLAGS

then using the CJSON_API_VISIBILITY flag to "export" the same symbols the way CJSON_EXPORT_SYMBOLS does

*/

    #define SERIALIZATER_CDECL __cdecl
    #define SERIALIZATER_STDCALL __stdcall

    /* export symbols by default, this is necessary for copy pasting the C and header file */
    #if !defined(SERIALIZATER_HIDE_SYMBOLS) && !defined(SERIALIZATER_IMPORT_SYMBOLS) && !defined(SERIALIZATER_SYMBOLS)
    #define SERIALIZATER_SYMBOLS
    #endif

    #if defined(SERIALIZATER_HIDE_SYMBOLS)
        #define SERIALIZATER_PUBLIC(type)   type SERIALIZATER_STDCALL
    #elif defined(SERIALIZATER_SYMBOLS)
        #define SERIALIZATER_PUBLIC(type)   __declspec(dllexport) type SERIALIZATER_STDCALL
    #elif defined(SERIALIZATER_IMPORT_SYMBOLS)
        #define SERIALIZATER_PUBLIC(type)   __declspec(dllimport) type SERIALIZATER_STDCALL
    #endif
#else /* !__WINDOWS__ */
    #define SERIALIZATER_CDECL
    #define SERIALIZATER_STDCALL

    #if (defined(__GNUC__) || defined(__SUNPRO_CC) || defined (__SUNPRO_C)) && defined(SERIALIZATER_API_VISIBILITY)
        #define SERIALIZATER_PUBLIC(type)   __attribute__((visibility("default"))) type
    #else
        #define SERIALIZATER_PUBLIC(type) type
    #endif
#endif


#include "cJSON.h"

#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) & ((TYPE *)0)->MEMBER)
#endif

#ifndef __cplusplus
#define bool char
#define true 1
#define false 0
#endif

#define cstring char*

typedef enum  {
    Seriali_Null = 0,
    Seriali_Char,
    Seriali_Bool,
    Seriali_Int,
    Seriali_Double,
    Seriali_Float,
    Seriali_String,
    Seriali_Object,
}Seriali_Type;

typedef struct {
    Seriali_Type type;
    size_t size;
    int width;
    bool is_ptr;
    void *type_desc;
}Seriali_Model;

typedef struct {
    Seriali_Model model;
    size_t offset;
    const char* name;
}StructDesc;



#define Seriali_Model_End( ) \
    { Seriali_Null, 0,  0, 0, NULL}

#define Seriali_Model_Bool(w, b ) \
    { Seriali_Bool, sizeof(bool), w, b, NULL}

#define Seriali_Model_Char( w, b ) \
    { Seriali_Char, sizeof(char),   w, b, NULL }

#define Seriali_Model_Int(w, b ) \
    { Seriali_Int, sizeof(int),    w, b , NULL}

#define Seriali_Model_Double(w, b ) \
    { Seriali_Double, sizeof(double), w, b, NULL }

#define Seriali_Model_Float(w, b ) \
    { Seriali_Float, sizeof(float),  w, b, NULL }

#define Seriali_Model_String(w, b ) \
   { Seriali_String, sizeof(cstring), w, b, NULL}

#define Seriali_Model_Object(obj, des, w, b ) \
   { Seriali_Object, sizeof(obj), w, b, des}

/*
* 结构体打印
*/
SERIALIZATER_PUBLIC(int) Seriali_Print(void *address, const StructDesc *des, int deep);


/*
*  生成 json
*/
SERIALIZATER_PUBLIC(char*) Seriali_Encode(void* address, const StructDesc *des );

/*
* json 字符串解析
*/
SERIALIZATER_PUBLIC(bool) Seriali_Decode(void* address, const StructDesc *des, const char* json);


#ifdef __cplusplus
}
#endif

#endif