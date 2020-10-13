#ifndef JSON_STRUCT_H
#define JSON_STRUCT_H



#include <json-c/json.h>
#include <json-c/json_object.h>
#include <json-c/bits.h>

struct json_desction
{
    char *name;
    int type;
    int size;
    size_t ptr;
    struct json_desction *des;
};



#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) & ((TYPE *)0)->MEMBER)
#endif


int json_struct_prease(struct json_desction *index, struct json_object *josn, void *data);
int json_struct_free(struct json_desction *index, void *data);
struct json_object * json_struct_assemble(struct json_desction *index, void *data);


#endif //JSON_STRUCT_H
