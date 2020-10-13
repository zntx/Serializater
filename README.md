c 语言 json对象 struct 描述 格式化 

对 json-c 解析库 进一步 封装

目前只支持json-c

#### 使用说明 #############
需要定义  struct address 描述数据，

#########
struct address 
{ 
    char *city; 
    char *country; 
}; 

struct json_desction address_desc[] = { 
    { "city",         json_type_string,    			0,                             offsetof(struct address, city),                 NULL},  
    { "country",      json_type_string,    			0,                             offsetof(struct address, country),              NULL}, 
    { NULL,           json_type_null,      			0,                             0,                                              NULL}       
};

然后调用 json_struct_prease 方法， 将 json 信息 提取到 struct address 结构式 体 中

具体见 test.c
