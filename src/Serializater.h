#ifndef DEF_SERIAL_H
#define DEF_SERIAL_H

#include "cJSON.h"

#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) & ((TYPE *)0)->MEMBER)
#endif

#define bool int
#define true 1
#define false 0

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
int Seriali_Print(void *address, const StructDesc *des, int deep);


/*
*  生成 json
*/
char* Seriali_Encode(void* address, const StructDesc *des );

/*
* json 字符串解析
*/
bool Seriali_Decode(void* address, const StructDesc *des, const char* json);

#endif