# Serializater

![version](https://img.shields.io/badge/version-1.0.4-brightgreen.svg)
![build](https://img.shields.io/badge/build-2020.09.13-brightgreen.svg)
![license](https://img.shields.io/badge/license-MIT-brightgreen.svg)

提供C语言struct序列化库
并基于[cJSON](https://github.com/kbranigan/cJSON),运行于C语言平台的json-struct模型解析工具

- [Serializater](#serializater)
  - [简介](#简介)
  - [使用](#使用)
    - [声明结构体](#声明结构体)
    - [定义数据模型](#定义数据模型)
    - [使用Serializater解析](#使用serializater解析)
  - [结构体数据类型](#结构体数据类型)
  - [数据模型映射](#数据模型映射)
  - [API](#api)
    - [初始化](#初始化)
    - [反序列化](#反序列化)
    - [序列化](#序列化)

## 简介

[Serializater](https://github.com/NevermindZZT/cson)是一个C语言struct序列化库， 并提供对json的序列化和反序列化（基于cJSON）。相比于使用原生cJSON一层一层解析的方式，Serializater采用模型映射的方式，使用模型将结构体的特征进行描述，然后根据模型，将json数据直接解析成结构体，免去使用原生cJSON需要多次调用API的复杂性，可以很大程度减少代码冗余，增加代码逻辑性。

Serializater的模型映射借鉴了高级语言(比如说Java)的反射机制，通过建立数据模型，记录结构体的元素，类型，偏移，然后直接在内存层面进行操作，对C语言提供类似于[gson](https://github.com/google/gson)这样的高效json解析工具

## 使用

Serializater通过数据模型将结构体和json建立映射关系，因此，你需要做的就是在声明结构体的时候同时，使用数据模型对结构体进行描述，之后，只需要直接调用Serializater的api即可

### 声明结构体

```C
/** 测试结构体 */
struct address
{
    int cityID;
    char *country;
};

struct domain_list
{
    char *name;
    char *url;
};
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
```

### 定义数据模型

对每一个需要使用cson的结构体，都需要定义相对应的数据模型

```C
StructDesc address_desc[] = {
    {Seriali_Model_Int(1, false), offsetof(struct address, cityID), "cityID"},
    {Seriali_Model_String(1, false), offsetof(struct address, country), "country"},
    {Seriali_Model_End(), 0, NULL}};

StructDesc domain_list_desc[] = {
    {Seriali_Model_String(1, true), offsetof(struct domain_list, name), "name"},
    {Seriali_Model_String(1, true), offsetof(struct domain_list, url), "url"},
    {Seriali_Model_End(), 0, NULL}};

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
```

### 使用Serializater解析

只需要定义好数据模型，就可以使用CSON读json进行序列化和反序列化

```C
void csonDemo(void)
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

    //Seriali_Print(&test2, &json_t001_desc[0], 0);

    printf("____________________________________________\n");
    char* json_string = Seriali_Encode( &test2, &json_t001_desc[0] );
    printf("json_string :%s\n", json_string);

    free(json_string);
}
```

运行结果：

```json_string :{
        "istrue1":      true,
        "max1": 10,
        "count1":       11,
        "name1":        "aaaaa",
        "address1":     {
                "cityID":       3333,
                "country":      "bbbbbbbbb"
        },
        "istrue2":      false,
        "max2": 1.11,
        "count2":       3,
        "name2":        "wwwwwww",
        "address2":     {
                "cityID":       999,
                "country":      "ssssss"
        },
        "istrue3":      [true, false, true],
        "max3": [0, 3.72807707261204e-317, 4.18667275760706e-317],
        "count3":       [4263968, 0, 268501009],
        "name3":        ["wwwwwww", "wwwwwww", "wwwwwww"],
        "address3":     [{
                        "cityID":       3333,
                        "country":      "bbbbbbbbb"
                }, {
                        "cityID":       3333,
                        "country":      "bbbbbbbbb"
                }, {
                        "cityID":       3333,
                        "country":      "bbbbbbbbb"
                }],
        "istrue4":      [false, false],
        "max4": [1.11, 1.11],
        "count4":       [3, 3],
        "name4":        ["wwwwwww", "wwwwwww"],
        "address4":     [{
                        "cityID":       999,
                        "country":      "ssssss"
                }, {
                        "cityID":       999,
                        "country":      "ssssss"
                }]
}
```

可以看到，无论是解析json，还是序列化结构体到json，在使用Serializater的情况下，都只需要一行代码就可以解决，同样的操作，在使用原生cJSON的情况下，你可能需要多次判断，解析元素。

## 结构体数据类型

Serializater采用数据模型对结构体进行解析，在方便json操作的同时，也给结构体的定义带来了一些限制，目前，Serializater所支持在结构体中定义的数据类型包括：

1. 整数(char, bool, int)
2. 浮点数(float, double)
3. 字符串(char *)
4. 基本类型数组
5. 子结构体 
6. 指针(一级指针)



Serializater支持的数据类型基本包括绝大多数使用场景。为了方便使用，后续将推出: 结构体转换工具。

## 数据模型映射

Serializater采用数据模型建立结构体同json之间的映射，数据模型通过结构体数组进行定义，数据模型定义如下：

```C
/**
 * @brief CSON数据模型定义
 *
 */
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
```

对于每一个需要使用Serializater的结构体，都需要定义一个数据模型，每一个数据模型都需要包含一条结构体描述`Seriali_Model_**`以及若干个数据描述，取决于结构体的成员数量

一般情况下，你只需要使用Serializater提供的宏进行数据模型条目的定义，数据模型宏与对应的数据类型对应如下：

| 数据模型宏                                          | 数据类型 | 备注                                                   |
| --------------------------------------------------- | -------- | ------------------------------------------------------ |
| Seriali_Model_Object(type)                                | 结构体结束   | 放在结构体描述的最后     |
| Seriali_Model_End(array_size, is_ptr )                          | char     |                                                        |                                       
| Seriali_Model_Int(array_size, is_ptr)                           | int      |                                                        |                                           
| Seriali_Model_Double(array_size, is_ptr)                         | float    |                                                        |
| Seriali_Model_Float(array_size, is_ptr)                        | double   |                                                        |
| CSON_MODEL_BOOL(array_size, is_ptr)                          | bool     | C没有bool,对对应int                                   |
| Seriali_Model_String(array_size, is_ptr)                        | char *   |                                                        |
| Seriali_Model_Object(type, type_des, array_size, is_ptr)     | 子结构体 | 子结构体                        |



## API

Serializater源文件 极其简单。

### 初始化

初始化Serializater，提供内存分配和内存释放函数，对于标准C库可以使用`malloc`和`free`


### 反序列化

解析json，将json字符串反序列化成结构体对象

```C
bool Seriali_Decode(void* address, const StructDesc *des, const char* json)
```

- 参数
  - `json` json字符串
  - `des` 描述目标结构体的数据模型
  - `address` 结构体地址
- 返回
  - `bool` 是否成功

### 序列化

编码结构体，将结构体对象序列化成json字符串

```C
char* Seriali_Encode(void* address, const StructDesc *des )
```

- 参数
  - `address` 源结构体对象
  - `des` 描述源结构体的数据模型
- 返回
  - `char *` 序列化得到的json字符串

