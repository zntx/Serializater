#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>

#include "json_struct.h"

int json_struct_prease(struct json_desction *index, struct json_object *josn, void *data)
{
    //deep++;
    for (int i = 0; index->name; index++)
    {
        //printf("index->name %s\n", index->name);
        struct json_object *tmp = NULL;
        if (json_object_object_get_ex(josn, index->name, &tmp))
        {
            if (index->type == json_type_array)
            {
                int count = json_object_array_length(tmp);
                *(int *)(data + index->ptr)  = count;

                //printf(" array len %d %d \n",  count , *(int *)(data + index->ptr) );
                index++;
                
                if (index->type == json_type_boolean)
                {
                    *(json_bool**)(data + index->ptr) = (json_bool *)malloc(sizeof(json_bool) * count);

                    for (int i = 0; i < count; i++)
                    {
                        struct json_object *josn_iteam = json_object_array_get_idx(tmp, i);
                        *(*(json_bool **)(data + index->ptr) + i) = json_object_get_boolean(josn_iteam);
                    }
                }
                else if (index->type == json_type_double)
                {
                    *(double **)(data + index->ptr) = (double *)malloc(sizeof(double) * count);
                    for (int i = 0; i < count; i++)
                    {
                        struct json_object *josn_iteam = json_object_array_get_idx(tmp, i);
                        *(*(double **)(data + index->ptr) + i) = json_object_get_double(josn_iteam);
                    }
                }
                else if (index->type == json_type_int)
                {

                    *(int **)(data + index->ptr) = (int *)malloc(sizeof(int) * count);
                    for (int i = 0; i < count; i++)
                    {
                        struct json_object *josn_iteam = json_object_array_get_idx(tmp, i);
                        *(*(int **)(data + index->ptr) + i) = json_object_get_int(josn_iteam);
                        //printf(" array[%d] %d\n", i,  *(*(int **)(data + index->ptr) + i));
                    }
                }
                else if (index->type == json_type_string)
                {
                    *(char ***)(data + index->ptr) = (char **)malloc(sizeof(char *) * count);
                    //printf("string %p \n",  *(char ***)(data + index->ptr));
                    for (int i = 0; i < count; i++)
                    {
                        struct json_object *josn_iteam = json_object_array_get_idx(tmp, i);
                        *(*(char ***)(data + index->ptr) + i) = strdup(json_object_get_string(josn_iteam));
                        //printf("string %s \n", *(*(char ***)(data + index->ptr) + i));
                    }
                }
                else if (index->type == json_type_object)
                {
                    *(void **)(data + index->ptr) = (void *)malloc(index->size * count);
                    for (int i = 0; i < count; i++)
                    {
                        //void *data_child = malloc(index->size);
                        void *data_child = *(void **)(data + index->ptr) + (i * index->size);
                        //*(*(char ***)(data + index->ptr) + i) = data_child;

                        struct json_object *json_iteam = json_object_array_get_idx(tmp, i);
                        json_struct_prease(index->des, json_iteam, data_child);
                    }
                }
            }
            else
            {
                if (index->type == json_type_boolean)
                {
                    *(json_bool *)(data + index->ptr) = json_object_get_boolean(tmp);
                }
                else if (index->type == json_type_double)
                {
                    *(double *)(data + index->ptr) = json_object_get_double(tmp);
                }
                else if (index->type == json_type_int)
                {
                    *(int *)(data + index->ptr) = json_object_get_int(tmp);
                }
                else if (index->type == json_type_string)
                {
                    //printf("string   %s \n", index->name);
                    *(char **)(data + index->ptr) = strdup( json_object_get_string(tmp) );
                    //if( deep == 2)
                    //{
                    //    printf("2 address  %s\n", *(char **)(data + index->ptr));
                    //}
                }
                else if (index->type == json_type_object)
                {
                    //*(char **)(data + index->ptr) = json_object_get_string(tmp);
                    void *data_child = malloc(index->size);
                    *(void **)(data + index->ptr) = data_child;
                    json_struct_prease(index->des, tmp, data_child);
                    //if(strcmp(index->name, "address") ==0 )
                    //{
                    //    printf("address des %p\n", index->des);
                    //    struct address* tessssss =  data_child;
                    //   printf("address city %s\n", tessssss->city);
                    //    printf("address country %s \n", tessssss->country);
                    //}
                }
            }
        }
        else
        {
            if (index->type == json_type_array)
            {
                //*(void **)(data + index->ptr) = NULL;
                *(int *)(data + index->ptr)  = 0;
            }
            else
            {
                if (index->type == json_type_boolean)
                {
                    *(bool *)(data + index->ptr) = 0;
                }
                else if (index->type == json_type_double)
                {
                    *(double *)(data + index->ptr) = 0;
                }
                else if (index->type == json_type_int)
                {
                    *(int *)(data + index->ptr) = 0;
                }
                else if (index->type == json_type_string)
                {
                    *(char **)(data + index->ptr) = NULL;
                }
                else if (index->type == json_type_object)
                {
                    *(void **)(data + index->ptr) = NULL;
                }
            }
        }
    }
    //deep--;
    return 1;
}

int json_struct_free(struct json_desction *index, void *data)
{
    if( !data )
        return 0;
    for (int i = 0; index->name; index++)
    {
        if (index->type == json_type_array)
        {
            int count = *(int*)(data + index->ptr);
            index++;

            if(count == 0) //数组为空
                continue; 
            
            if (index->type == json_type_string)
            {
                for (int i = 0; i < count; i++)
                {
                    free( *(*(char ***)(data + index->ptr) + i)  );
                }
                free( *(char ***)(data + index->ptr));
            }
            else if (index->type == json_type_object)
            {
                for (int i = 0; i < count; i++)
                {
                    void *data_child = *(void **)(data + index->ptr) + (i * index->size);         
                    json_struct_free(index->des,  data_child);
                }
                free(*(void **)(data + index->ptr));
            }else 
            {
                free(*(void **)(data + index->ptr));
            }
        }
        else
        {
            
            if (index->type == json_type_string)
            {
                char* tmp = *(char **)(data + index->ptr);
                if(tmp)
                    free( tmp);
                
            }
            else if (index->type == json_type_object)
            {
                void* data_child = *(void **)(data + index->ptr);
                json_struct_free(index->des,  data_child);
                if (data_child)
                    free(data_child);
            }
        }
    }
    return 1;
}

struct json_object * json_struct_assemble(struct json_desction *index, void *data)
{
    struct json_object *josn =json_object_new_object();
    for (int i = 0; index->name; index++)
    {
        if (index->type == json_type_array)
        {
            int count = *(int*)(data + index->ptr);
            char* __name = index->name;
            index++;

            if(count == 0) //数组为空
                continue; 

            struct json_object *array =json_object_new_array();
            if (index->type == json_type_boolean)
            {
                for (int i = 0; i < count; i++)
                {
                    json_object_array_add(array, json_object_new_boolean( *(*(json_bool **)(data + index->ptr) + i)) );
                }
            }
            else if (index->type == json_type_double)
            {
                for (int i = 0; i < count; i++)
                {
                    json_object_array_add(array, json_object_new_double(  *(*(double **)(data + index->ptr) + i) ));
                }
            }
            else if (index->type == json_type_int)
            {
                for (int i = 0; i < count; i++)
                {
                    json_object_array_add(array, json_object_new_int(   *(*(int **)(data + index->ptr) + i) ));
                }
            }
            else if (index->type == json_type_string)
            {
                for (int i = 0; i < count; i++)
                {
                    json_object_array_add(array, json_object_new_string(    *(*(char ***)(data + index->ptr) + i) ));
                }
            }
            else if (index->type == json_type_object)
            {
                for (int i = 0; i < count; i++)
                {
                    void *data_child = *(void **)(data + index->ptr) + (i * index->size);
                    struct json_object *tmp = json_struct_assemble(index->des,   data_child);
                    json_object_array_add(array,  tmp);
                }
            }

            json_object_object_add( josn, __name, array );
        }
        else
        {
            if (index->type == json_type_boolean)
            {
                json_object_object_add( josn, index->name, json_object_new_boolean( *(json_bool *)(data + index->ptr) ) );
            }
            else if (index->type == json_type_double)
            {
                json_object_object_add( josn, index->name, json_object_new_double( *(double *)(data + index->ptr) ) );
            }
            else if (index->type == json_type_int)
            {
                json_object_object_add( josn, index->name, json_object_new_int( *(int *)(data + index->ptr) ) );
            }
            else if (index->type == json_type_string)
            {
                json_object_object_add( josn, index->name, json_object_new_string(  *(char **)(data + index->ptr) ) );
            }
            else if (index->type == json_type_object)
            {
                void *data_child = *(void **)(data + index->ptr);
                struct json_object *tmp = json_struct_assemble(index->des, data_child);

                json_object_object_add( josn, index->name, tmp );
            }
        }
    }
    return josn;
}


