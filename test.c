#include <stdio.h>



#include "json_struct.h"


const char *json_string = "{\
    \"istrue\":true,\
    \"max\":65535,\
    \"count\":1234,\
	\"name\": \"SO JSON在线\",\
	\"url\":\"https://www.sojson.com\",\
	\"address\": {\
		\"city\": \"北京\",\
		\"country\": \"中国\"\
	},\
	\"domain_list\": {\
        \"name\": \"ICP备案查询\",\
        \"url\": \"https://icp.sojson.com\"\
    },\
	\"testid\": [123, 234],\
    \"xingming\":[\"niha\", \"niha1\",\"niha2\", \"niha3\"],\
    \"testadd\": [{\
			\"city\": \"ICP备案查询\",\
			\"country\": \"https://icp.sojson.com\"\
		},\
		{\
			\"city\": \"JSON在线解析\",\
			\"country\": \"https://www.sojson.com\"\
		},\
		{\
			\"city\": \"房贷计算器\",\
			\"country\": \"https://fang.sojson.com\"\
		}\
	]\
}";



struct address
{
    char *city;
    char *country;
};
struct domain_list
{
    char *name;
    char *url;
};
struct json_t001
{
    json_bool istrue;
    double max;
    int count;
    char *name;
    char *url;
    struct address *_address;
    struct domain_list *_domain_list;
    int testid_len;
    int *testid;
    int testadd_len;
    struct address *testadd;
    int xingming_len;
    char** xingming;
    
};


struct json_desction address_desc[] = {
    { "city",         json_type_string,    			0,                             offsetof(struct address, city),                 NULL}, 
    { "country",      json_type_string,    			0,                             offsetof(struct address, country),              NULL},
    { NULL,           json_type_null,      			0,                             0,                                              NULL}      
};

struct json_desction domain_list_desc[] = {
    { "name",         json_type_string,    			0,                             offsetof(struct domain_list, name),             NULL}, 
    { "url",          json_type_string,    			0,                             offsetof(struct domain_list, url),              NULL},
    { NULL,           json_type_null,      			0,                             0,                                              NULL}      
};

struct json_desction Tjson_dec[] = {
    { "istrue",       json_type_boolean,            0,                             offsetof(struct json_t001, istrue),              NULL }, 
    { "max",          json_type_double,             0,                             offsetof(struct json_t001, max),              NULL }, 
    { "count",        json_type_int,                0,                             offsetof(struct json_t001, count),              NULL }, 
    { "name",         json_type_string,    			0,                             offsetof(struct json_t001, name),               NULL }, 
    { "url",          json_type_string,    			0,                             offsetof(struct json_t001, url),                NULL }, 
    { "address",      json_type_object,    			sizeof(struct address),        offsetof(struct json_t001, _address),           address_desc },
    { "domain_list",  json_type_object,    			sizeof(struct domain_list),    offsetof(struct json_t001, _domain_list),       domain_list_desc },
    { "testid",       json_type_array,              0,                             offsetof(struct json_t001, testid_len),         NULL }, 
    { "testid",       json_type_int,                0,                             offsetof(struct json_t001, testid),             NULL },
    { "testadd",      json_type_array,              0,                             offsetof(struct json_t001, testadd_len),        NULL }, 
    { "testadd",      json_type_object,             sizeof(struct address),        offsetof(struct json_t001, testadd),            address_desc }, 
    { "xingming",     json_type_array,              0,                             offsetof(struct json_t001, xingming_len),       NULL }, 
    { "xingming",     json_type_string,             sizeof(char*),                 offsetof(struct json_t001, xingming),           address_desc },
    
    { NULL,           json_type_null,    			0,                             0,                                              NULL }        
};

int main(int argc, char *agrv[])
{
    struct json_object *josn = json_tokener_parse(json_string);
    struct json_t001 *test_001 = malloc(sizeof(struct json_t001));


    json_struct_prease(&Tjson_dec[0], josn, test_001);
    json_object_put(josn);
    printf("____________________________________________________\n" );
    printf("istrue %d\n", test_001->istrue);
    printf("max %f\n", test_001->max);
    printf("count %d\n", test_001->count);
    printf("name %s\n", test_001->name);
    printf("url %s\n", test_001->url);
    printf("address\n{\tcity %s\n", test_001->_address->city);
    printf("\tcountry %s\n}\n", test_001->_address->country);
    printf("domain_list\n{\tname %s\n", test_001->_domain_list->name);
    printf("\turl %s\n}\n", test_001->_domain_list->url);
    printf("testid :[" );
    for(int i = 0; i< test_001->testid_len; i++ )
    {
        printf("  %d,", test_001->testid[i]);
    }
    printf("]\n");
    printf("testadd:[\n" );
    for(int i = 0; i< test_001->testadd_len; i++ )
    {
        printf("{\n\tcity %s \n", test_001->testadd[i].city);
        printf("\tcountry %s \n}\n", test_001->testadd[i].country);
    }
    printf("]\n");

    printf("xingming[" );
    for(int i = 0; i< test_001->xingming_len; i++ )
    {
        printf("  %s,", test_001->xingming[i]);
    }
    printf("]\n");
    printf("count %d\n", test_001->count);



    printf("____________________________________________________\n" );

    struct json_object *josn2 = json_struct_assemble(&Tjson_dec[0],  test_001);
    printf("json %s \n", json_object_to_json_string(josn2) );
    json_object_put(josn2);


    printf("____________________________________________________\n" );

    json_struct_free(&Tjson_dec[0],  test_001);
    free(test_001);

    return 1;
}