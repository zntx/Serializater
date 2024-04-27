#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/Serializater.h"
#include "../src/cJSON.h"

struct address
{
    int cityID;
    char *country;
};

StructDesc address_desc[] = {
    {Seriali_Model_Int(1, false), offsetof(struct address, cityID), "cityID"},
    {Seriali_Model_String(1, false), offsetof(struct address, country), "country"},
    {Seriali_Model_End(), 0, NULL}};

struct domain_list
{
    char *name;
    char *url;
};

StructDesc domain_list_desc[] = {
    {Seriali_Model_String(1, true), offsetof(struct domain_list, name), "name"},
    {Seriali_Model_String(1, true), offsetof(struct domain_list, url), "url"},
    {Seriali_Model_End(), 0, NULL}};

struct json_t001
{
    bool istrue1;
    double max1;
    int count1;
    cstring name1;
    struct address address1;

    bool *istrue2;
    double *max2;
    int *count2;
    cstring *name2;
    struct address *address2;

    bool istrue3[3];
    double max3[3];
    int count3[3];
    cstring name3[3];
    struct address address3[3];

    bool *istrue4[2];
    double *max4[2];
    int *count4[2];
    cstring *name4[2];
    struct address *address4[2];
};

StructDesc json_t001_desc[] = {
    {Seriali_Model_Bool(1, false), offsetof(struct json_t001, istrue1), "istrue1"},
    {Seriali_Model_Double(1, false), offsetof(struct json_t001, max1), "max1"},
    {Seriali_Model_Int(1, false), offsetof(struct json_t001, count1), "count1"},
    {Seriali_Model_String(1, false), offsetof(struct json_t001, name1), "name1"},
    {Seriali_Model_Object(struct address, address_desc, 1,  false), offsetof(struct json_t001, address1), "address1"},

    {Seriali_Model_Bool(1, true), offsetof(struct json_t001, istrue2), "istrue2"},
    {Seriali_Model_Double(1,  true), offsetof(struct json_t001, max2), "max2"},
    {Seriali_Model_Int(1,  true), offsetof(struct json_t001, count2), "count2"},
    {Seriali_Model_String(1, true), offsetof(struct json_t001, name2), "name2"},
    {Seriali_Model_Object(struct address, address_desc, 1, true), offsetof(struct json_t001, address2), "address2"},

    {Seriali_Model_Bool(3, false), offsetof(struct json_t001, istrue3), "istrue3"},
    {Seriali_Model_Double(3, false), offsetof(struct json_t001, max3), "max3"},
    {Seriali_Model_Int(3, false), offsetof(struct json_t001, count3), "count3"},
    {Seriali_Model_String(3, false), offsetof(struct json_t001, name3), "name3"},
    {Seriali_Model_Object(struct address, address_desc, 3, false), offsetof(struct json_t001, address1), "address3"},

    {Seriali_Model_Bool(2, true), offsetof(struct json_t001, istrue4), "istrue4"},
    {Seriali_Model_Double(2, true), offsetof(struct json_t001, max4), "max4"},
    {Seriali_Model_Int(2, true), offsetof(struct json_t001, count4), "count4"},
    {Seriali_Model_String(2, true), offsetof(struct json_t001, name4), "name4"},
    {Seriali_Model_Object(struct address, address_desc, 2, true), offsetof(struct json_t001, address4), "address4"},

    {Seriali_Model_End(), 0, NULL}};

int main()
{
    const char *json = "{\
	\"istrue1\":true,\
	\"max1\": 10,\
	\"count1\":11,\
	\"name1\":\"aaaaa\",\
	\"address1\":{\
			\"cityID\":3333,\
			\"country\":\"bbbbbbbbb\"\
	},\
	\"istrue2\":false,\
	\"max2\": 1.11,\
	\"count2\":3,\
	\"name2\":\"wwwwwww\",\
	\"address2\":{\
			\"cityID\":999,\
			\"country\":\"ssssss\"\
	},\
	\"istrue3\":[true, false, true],\
	\"max3\":[0, 3.72807707261204e-317, 4.18667275760706e-317],\
	\"count3\":[4263968, 0, 268501009],\
	\"name3\":[\"wwwwwww\", \"wwwwwww\", \"wwwwwww\"],\
	\"address3\":[{\
        \"cityID\":       3333,\
        \"country\":      \"bbbbbbbbb\"\
			}, {\"cityID\":       3333,\
					\"country\":      \"bbbbbbbbb\"\
			}, {\"cityID\":       3333,\
					\"country\":      \"bbbbbbbbb\"\
			}],\
	\"istrue4\": [false, false],\
	\"max4\": [1.11, 1.11],\
	\"count4\":  [3, 3],\
	\"name4\": [\"wwwwwww\", \"wwwwwww\"],\
	\"address4\": [{\"cityID\":       999,\
				\"country\":      \"ssssss\"\
			}, {\"cityID\":       999,\
				\"country\":      \"ssssss\"\
			}]\
}";

    struct json_t001 test2 = {0};
    Seriali_Decode( &test2, &json_t001_desc[0], json);
    printf("____________________________________________\n");
    printf("address %p\n", &test2.address1.cityID);
    printf("address %d\n", test2.address1.cityID);

    Seriali_Print(&test2, &json_t001_desc[0], 0);

    printf("____________________________________________\n");
    char* json_string = Seriali_Encode( &test2, &json_t001_desc[0] );
    printf("json_string :%s\n", json_string);

    free(json_string);
}