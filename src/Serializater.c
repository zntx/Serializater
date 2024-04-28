#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Serializater.h"
#include "cJSON.h"

bool Seriali_Default(void *address, const StructDesc *first)
{
    int i = 0;
    int max = 0;
    if( address == NULL || first == NULL )
    {
        return false;
    }

    for (const StructDesc* des = first ; des != NULL && des->model.type != Seriali_Null; des++)
    {
        switch (des->model.type)
        {
        case Seriali_Char:

            for (int i = 0; i < des->model.width; i++)
            {
                if (des->model.is_ptr)
                {
                    (*(char **)((address + des->offset) + i * sizeof(char *))) = malloc(sizeof(char));
                    *(*(char **)((address + des->offset) + i * sizeof(char *))) = '0';
                }
                else
                {
                    *((char *)(address + des->offset) + i) = '0';
                }
            }
            break;

        case Seriali_Bool:
            for (int i = 0; i < des->model.width; i++)
            {
                if (des->model.is_ptr)
                {
                    (*(int **)((address + des->offset) + i * sizeof(char *))) = malloc(sizeof(bool));
                    *(*(int **)((address + des->offset) + i * sizeof(char *))) = false;
                }
                else
                {
                    *((bool *)(address + des->offset) + i) = false;
                }
            }
            break;

        case Seriali_Int:
            for (int i = 0; i < des->model.width; i++)
            {
                if (des->model.is_ptr)
                {
                    *(int **)((address + des->offset) + i * sizeof(char *)) = malloc(sizeof(int));
                    *(*(int **)((address + des->offset) + i * sizeof(char *))) = 0;
                }
                else
                    *((int *)(address + des->offset) + i) = 0;
            }
            break;

        case Seriali_Double:
            for (int i = 0; i < des->model.width; i++)
            {
                if (des->model.is_ptr)
                {
                    *(double **)((address + des->offset) + i * sizeof(char *)) = malloc(sizeof(double));
                    *(*(double **)((address + des->offset) + i * sizeof(char *))) = 0;
                }
                else
                    *((double *)(address + des->offset) + i) = 0;
            }
            break;

        case Seriali_Float:
            for (int i = 0; i < des->model.width; i++)
            {
                if (des->model.is_ptr)
                {
                    *(float **)((address + des->offset) + i * sizeof(char *)) = malloc(sizeof(float));
                    *(*(float **)((address + des->offset) + i * sizeof(char *))) = 0.000;
                }
                else
                    *((float *)(address + des->offset) + i) = 0.000;
            }
            break;

        case Seriali_String:
            for (int i = 0; i < des->model.width; i++)
            {
                if (des->model.is_ptr)
                {
                    char **tmp_str = malloc(sizeof(int *));
                    *tmp_str = strdup("");
                    (*(cstring **)((address + des->offset) + i * sizeof(char *))) = tmp_str;

                }
                else
                {
                    *((cstring *)(address + des->offset) + i) = strdup("");
                }
            }
            break;

        case Seriali_Object:
            for (int i = 0; i < des->model.width; i++)
            {
                if (des->model.is_ptr)
                {
                    *(void **)((address + des->offset) + i * sizeof(char *)) = malloc(des->model.size);
                    void *tmp_addr = *(void **)((address + des->offset) + i * sizeof(char *));
                    Seriali_Default(tmp_addr, des->model.type_desc);
                }
                else
                {
                    Seriali_Default(address + des->offset, des->model.type_desc);
                }
            }
            break;

        default:
            return false;
            break;
        }
    }

    return true;
}

SERIALIZATER_PUBLIC(int) Seriali_Print(void *address, const StructDesc *des, int deep)
{
    void *data_child = NULL;
    int i = 0;
    for (; des != NULL && des->model.type != Seriali_Null; des++)
    {
        for (int i = 0; i < deep; i++)
            printf("\t");
        printf("name %s \t", des->name);
        switch (des->model.type)
        {
        case Seriali_Char:
            printf(" [ ");
            for (int i = 0; i < des->model.width; i++)
            {
                if (des->model.is_ptr)
                    printf(" %c, ", *(*(char **)((address + des->offset) + i * sizeof(char *))));
                else
                    printf(" %c, ", *((char *)(address + des->offset) + i) );
            }
            printf("]\n");

            break;
        case Seriali_Bool:
            printf(" [ ");
            for (int i = 0; i < des->model.width; i++)
            {
                if (des->model.is_ptr)
                    printf(" %s, ", *(*(bool **)((address + des->offset) + i * sizeof(char *))) > 0 ? "true" : "false");
                else
                    printf(" %s, ", *((bool *)(address + des->offset) + i) > 0 ? "true" : "false");
            }
            printf("]\n");
            break;
        case Seriali_Int:
            printf(" in int [ ");
            for (int i = 0; i < des->model.width; i++)
            {
                if (des->model.is_ptr)
                    printf(" %d, ", *(*(int **)((address + des->offset) + i * sizeof(char *))));
                else
                    printf(" %d, ", *((int *)(address + des->offset) + i));
            }
            printf("]\n");
            break;
        case Seriali_Double:
            printf("[ ");
            for (int i = 0; i < des->model.width; i++)
            {
                if (des->model.is_ptr)
                {
                    printf(" %f, ", *(*(double **)((address + des->offset) + i * sizeof(char *))));
                }
                else
                    printf(" %f, ", *((double *)(address + des->offset) + i));
            }
            printf("]\n");
            break;

        case Seriali_Float:
            printf("[ ");
            for (int i = 0; i < des->model.width; i++)
            {
                if (des->model.is_ptr)
                {
                    printf(" %f, ", *(*(float **)((address + des->offset) + i * sizeof(char *))));
                }
                else
                    printf(" %f, ", *((float *)(address + des->offset) + i));
            }
            printf("]\n");
            break;

        case Seriali_String:
            printf("[ ");
            for (int i = 0; i < des->model.width; i++)
            {
                if (des->model.is_ptr)
                    printf(" %s, ", *(*(cstring **)((address + des->offset) + i * sizeof(char *))));
                else
                    printf(" %s, ", *((cstring *)(address + des->offset) + i));
            }
            printf("]\n");
            break;

        case Seriali_Object:
            printf(" {\n");
            for (int i = 0; i < des->model.width; i++)
            {
                if (des->model.is_ptr)
                {
                    void *tmp_addr = *(void **)((address + des->offset) + i * sizeof(char *));
                    Seriali_Print(tmp_addr, des->model.type_desc, deep + 1);
                }
                else
                {
                    Seriali_Print(address + des->offset, des->model.type_desc, deep + 1);
                }
            }
            printf(" }\n");
            break;

        default:
            printf(" unknow type %d \t", des->model.type);
            break;
        }
    }
}

cJSON *Seriali_to_json(void *address, const StructDesc *des)
{
    void *data_child = NULL;
    int i = 0;

    cJSON *root = cJSON_CreateObject();
    cJSON *array = NULL;
    for (; des != NULL && des->model.type != Seriali_Null; des++)
    {

        switch (des->model.type)
        {
        case Seriali_Char:
            printf(" %c \n", *(char *)(address + des->offset));
            break;
        case Seriali_Bool:
            if (des->model.width > 1)
            {
                array = cJSON_CreateArray();
                cJSON_AddItemToObject(root, des->name, array);

                for (int i = 0; i < des->model.width; i++)
                {
                    if (des->model.is_ptr)
                        cJSON_AddItemToArray(array, cJSON_CreateBool(*(*(bool **)((address + des->offset) + i * sizeof(char *)))));
                    else
                        cJSON_AddItemToArray(array, cJSON_CreateBool(*((bool *)(address + des->offset) + i)));
                }
            }
            else
            {
                if (des->model.is_ptr)
                    cJSON_AddItemToObject(root, des->name, cJSON_CreateBool(*(*(bool **)((address + des->offset) + 0 * sizeof(char *)))));
                else
                    cJSON_AddItemToObject(root, des->name, cJSON_CreateBool(*((bool *)(address + des->offset) + 0)));
            }

            break;
        case Seriali_Int:
            if (des->model.width > 1)
            {
                array = cJSON_CreateArray();
                cJSON_AddItemToObject(root, des->name, array);

                for (int i = 0; i < des->model.width; i++)
                {
                    if (des->model.is_ptr)
                        cJSON_AddItemToArray(array, cJSON_CreateNumber(*(*(int **)((address + des->offset) + i * sizeof(char *)))));
                    else
                        cJSON_AddItemToArray(array, cJSON_CreateNumber(*((int *)(address + des->offset) + i)));
                }
            }
            else
            {
                if (des->model.is_ptr)
                    cJSON_AddItemToObject(root, des->name, cJSON_CreateNumber(*(*(int **)((address + des->offset) + 0 * sizeof(char *)))));
                else
                    cJSON_AddItemToObject(root, des->name, cJSON_CreateNumber(*((int *)(address + des->offset) + 0)));
            }
            break;
        case Seriali_Double:
            if (des->model.width > 1)
            {
                array = cJSON_CreateArray();
                cJSON_AddItemToObject(root, des->name, array);

                for (int i = 0; i < des->model.width; i++)
                {
                    if (des->model.is_ptr)
                        cJSON_AddItemToArray(array, cJSON_CreateNumber(*(*(double **)((address + des->offset) + i * sizeof(char *)))));
                    else
                        cJSON_AddItemToArray(array, cJSON_CreateNumber(*((double *)(address + des->offset) + i)));
                }
            }
            else
            {
                if (des->model.is_ptr)
                    cJSON_AddItemToObject(root, des->name, cJSON_CreateNumber(*(*(double **)((address + des->offset) + 0 * sizeof(char *)))));
                else
                    cJSON_AddItemToObject(root, des->name, cJSON_CreateNumber(*((double *)(address + des->offset) + 0)));
            }
            break;

        case Seriali_Float:
            if (des->model.width > 1)
            {
                array = cJSON_CreateArray();
                cJSON_AddItemToObject(root, des->name, array);

                for (int i = 0; i < des->model.width; i++)
                {
                    if (des->model.is_ptr)
                        cJSON_AddItemToArray(array, cJSON_CreateNumber(*(*(float **)((address + des->offset) + i * sizeof(char *)))));
                    else
                        cJSON_AddItemToArray(array, cJSON_CreateNumber(*((float *)(address + des->offset) + i)));
                }
            }
            else
            {
                if (des->model.is_ptr)
                    cJSON_AddItemToObject(root, des->name, cJSON_CreateNumber(*(*(float **)((address + des->offset) + 0 * sizeof(char *)))));
                else
                    cJSON_AddItemToObject(root, des->name, cJSON_CreateNumber(*((float *)(address + des->offset) + 0)));
            }
            break;

        case Seriali_String:
            if (des->model.width > 1)
            {
                array = cJSON_CreateArray();
                cJSON_AddItemToObject(root, des->name, array);

                for (int i = 0; i < des->model.width; i++)
                {
                    if (des->model.is_ptr)
                        cJSON_AddItemToArray(array, cJSON_CreateString(*(*(cstring **)((address + des->offset) + i * sizeof(char *)))));
                    else
                        cJSON_AddItemToArray(array, cJSON_CreateString(*((cstring *)(address + des->offset) + i)));
                }
            }
            else
            {
                if (des->model.is_ptr)
                    cJSON_AddItemToObject(root, des->name, cJSON_CreateString(*(*(cstring **)((address + des->offset) + 0 * sizeof(char *)))));
                else
                    cJSON_AddItemToObject(root, des->name, cJSON_CreateString(*((cstring *)(address + des->offset) + 0)));
            }
            break;

        case Seriali_Object:
            if (des->model.width > 1)
            {
                array = cJSON_CreateArray();
                cJSON_AddItemToObject(root, des->name, array);

                for (int i = 0; i < des->model.width; i++)
                {
                    if (des->model.is_ptr)
                    {
                        void *tmp_addr = *(void **)((address + des->offset) + i * sizeof(char *));
                        cJSON *item = Seriali_to_json(tmp_addr, des->model.type_desc);
                        cJSON_AddItemToArray(array, item);
                    }
                    else
                    {
                        cJSON *item = Seriali_to_json(address + des->offset, des->model.type_desc);
                        cJSON_AddItemToArray(array, item);
                    }
                }
            }
            else
            {
                if (des->model.is_ptr)
                {
                    void *tmp_addr = *(void **)((address + des->offset) + 0 * sizeof(char *));
                    cJSON *item = Seriali_to_json(tmp_addr, des->model.type_desc);
                    cJSON_AddItemToObject(root, des->name, item);
                }
                else
                {
                    cJSON *item = Seriali_to_json(address + des->offset, des->model.type_desc);
                    cJSON_AddItemToObject(root, des->name, item);
                }
            }
            break;

        default:
            break;
        }
    }

    return root;
}

bool Seriali_from_json(void *address, const StructDesc *first, const cJSON *root)
{
    int i = 0;
    int max = 0;
    cJSON *item = NULL;
    cJSON *array_item = NULL;

    for (const StructDesc* des = first; des != NULL && des->model.type != Seriali_Null; des++)
    {
        item = cJSON_GetObjectItem(root, des->name);
        if (item == NULL)
        {
            printf(" not find %s \n", des->name);
            Seriali_Default(address, des);
            continue;
        }

        if (cJSON_IsArray(item) && des->model.width == 1)
        {   
            printf("name %s is not array \n", des->name);
            return false;
        }
        if (!cJSON_IsArray(item) && des->model.width > 1)
        {
            printf("name %s is not array \n", des->name);
            return false;
        }

        switch (des->model.type)
        {
        case Seriali_Char:

            if (des->model.width > 1)
            {
                for (int i = 0; i < des->model.width; i++)
                {
                    array_item = cJSON_GetArrayItem(item, i);
                    if (item == NULL)
                    {
                        return false;
                    }

                    if (des->model.is_ptr)
                    {
                        (*(char **)((address + des->offset) + i * sizeof(char *))) = malloc(sizeof(char));
                        *(*(char **)((address + des->offset) + i * sizeof(char *))) = array_item->valueint;
                    }
                    else
                    {
                        *((char *)(address + des->offset) + i) = array_item->valueint;
                    }
                }
            }
            else
            {
                if (des->model.is_ptr)
                {
                    (*(char **)((address + des->offset) + 0 * sizeof(char *))) = malloc(sizeof(char));
                    *(*(char **)((address + des->offset) + 0 * sizeof(char *))) = item->valueint;
                }
                else
                {
                    *((char *)(address + des->offset) + 0) = item->valueint;
                }
            }


            break;
        case Seriali_Bool:
            if (des->model.width > 1)
            {
                for (int i = 0; i < des->model.width; i++)
                {
                    array_item = cJSON_GetArrayItem(item, i);
                    if (item == NULL)
                    {
                        return false;
                    }

                    if (des->model.is_ptr)
                    {
                        (*(int **)((address + des->offset) + i * sizeof(char *))) = malloc(sizeof(bool));
                        *(*(int **)((address + des->offset) + i * sizeof(char *))) = array_item->valueint;
                    }
                    else
                    {
                        *((bool *)(address + des->offset) + i) = array_item->valueint;
                    }
                }
            }
            else
            {
                if (des->model.is_ptr)
                {
                    (*(int **)((address + des->offset) + 0 * sizeof(char *))) = malloc(sizeof(bool));
                    *(*(int **)((address + des->offset) + 0 * sizeof(char *))) = item->valueint;
                }
                else
                {
                    *((bool *)(address + des->offset) + 0) = item->valueint;
                }
            }

            break;

        case Seriali_Int:
            if (des->model.width > 1)
            {
                for (int i = 0; i < des->model.width; i++)
                {
                    array_item = cJSON_GetArrayItem(item, i);
                    if (item == NULL)
                    {
                        return false;
                    }

                    if (des->model.is_ptr)
                    {
                        *(int **)((address + des->offset) + i * sizeof(char *)) = malloc(sizeof(int));
                        *(*(int **)((address + des->offset) + i * sizeof(char *))) = array_item->valueint;
                    }
                    else
                        *((int *)(address + des->offset) + i) = array_item->valueint;
                }
            }
            else
            {
                if (des->model.is_ptr)
                {
                    *(int **)((address + des->offset) + 0 * sizeof(char *)) = malloc(sizeof(int));
                    *(*(int **)((address + des->offset) + 0 * sizeof(char *))) = item->valueint;
                }
                else
                    *((int *)(address + des->offset) + 0) = item->valueint;
            }
            break;

        case Seriali_Double:
            if (des->model.width > 1)
            {
                for (int i = 0; i < des->model.width; i++)
                {
                    array_item = cJSON_GetArrayItem(item, i);
                    if (item == NULL)
                    {
                        return false;
                    }
                    if (des->model.is_ptr)
                    {
                        *(double **)((address + des->offset) + i * sizeof(char *)) = malloc(sizeof(double));
                        *(*(double **)((address + des->offset) + i * sizeof(char *))) = array_item->valuedouble;
                    }
                    else
                        *((double *)(address + des->offset) + i) = array_item->valuedouble;
                }
            }
            else
            {
                if (des->model.is_ptr)
                {
                    *(double **)((address + des->offset) + 0 * sizeof(char *)) = malloc(sizeof(double));
                    *(*(double **)((address + des->offset) + 0 * sizeof(char *))) = item->valuedouble;
                }
                else
                    *((double *)(address + des->offset) + 0) = item->valuedouble;
            }
            break;

        case Seriali_Float:
            if (des->model.width > 1)
            {
                for (int i = 0; i < des->model.width; i++)
                {
                    array_item = cJSON_GetArrayItem(item, i);
                    if (item == NULL)
                    {
                        return false;
                    }
                    if (des->model.is_ptr)
                    {
                        *(float **)((address + des->offset) + i * sizeof(char *)) = malloc(sizeof(float));
                        *(*(float **)((address + des->offset) + i * sizeof(char *))) = array_item->valuedouble;
                    }
                    else
                        *((float *)(address + des->offset) + i) = array_item->valuedouble;
                }
            }
            else
            {
                if (des->model.is_ptr)
                {
                    *(float **)((address + des->offset) + 0 * sizeof(char *)) = malloc(sizeof(float));
                    *(*(float **)((address + des->offset) + 0 * sizeof(char *))) = item->valuedouble;
                }
                else
                    *((float *)(address + des->offset) + 0) = item->valuedouble;
            }
            break;

        case Seriali_String:
            if( des->model.width > 1 )
            {
                for (int i = 0; i < des->model.width; i++)
                {
                    array_item = cJSON_GetArrayItem(item, i);
                    if (item == NULL)
                    {
                        printf(" not find %s \n", des->name);
                        return false;
                    }
                    if (des->model.is_ptr)
                    {
                        char **tmp_str = malloc(sizeof(int *));
                        *tmp_str = strdup( array_item->valuestring);
                        (*(cstring **)((address + des->offset) + i * sizeof(char *))) = tmp_str;
                    }
                    else
                    {
                        *((cstring *)(address + des->offset) + i) = strdup(array_item->valuestring);
                    }
                }
            }
            else
            {
                if (des->model.is_ptr)
                {
                    char **tmp_str = malloc(sizeof(int *) );
                    *tmp_str = strdup( item->valuestring);
                    (*(cstring **)((address + des->offset) + 0 * sizeof(char *))) = tmp_str;
                }
                else
                {
                    *((cstring *)(address + des->offset) + 0) = strdup(item->valuestring);
                }
            }
            break;

        case Seriali_Object:
            if( des->model.width > 1)
            {
                for (int i = 0; i < des->model.width; i++)
                {
                    array_item = cJSON_GetArrayItem(item, i);
                    if (item == NULL)
                    {
                        printf(" not find %s \n", des->name);
                        return false;
                    }
                    if (des->model.is_ptr)
                    {
                        *(void **)((address + des->offset) + i * sizeof(char *)) = malloc(des->model.size);
                        void *tmp_addr = *(void **)((address + des->offset) + i * sizeof(char *));

                        bool ret = Seriali_from_json(tmp_addr, des->model.type_desc, array_item);
                        if( ret == false)
                        {
                            return false;
                        }
                    }
                    else
                    {
                        bool ret =  Seriali_from_json(address + des->offset, des->model.type_desc, array_item);
                        if( ret == false)
                        {
                            return false;
                        }
                    }
                }
            }
            else
            {
                if (des->model.is_ptr)
                {
                    *(void **)((address + des->offset) + i * sizeof(char *)) = malloc(des->model.size);
                    void *tmp_addr = *(void **)((address + des->offset) + i * sizeof(char *));
                    bool ret = Seriali_from_json(tmp_addr, des->model.type_desc, item);
                    if( ret == false)
                    {
                        return false;
                    }
                }
                else
                {
                    bool ret = Seriali_from_json(address + des->offset, des->model.type_desc, item);
                    if( ret == false)
                    {
                        return false;
                    }
                }
            }
            break;

        default:
            break;
        }
    }

    return true;
}


/*
*/
SERIALIZATER_PUBLIC(bool) Seriali_Decode(void* address, const StructDesc *des, const char* json)
{
    if( json == NULL)
    {
        return false;
    }

    cJSON *root = cJSON_Parse(json);
    if( root == NULL)
    {
        return false;
    }

    return Seriali_from_json(address, des, root);
}


SERIALIZATER_PUBLIC(char*) Seriali_Encode(void* address, const StructDesc *des )
{
    cJSON *root = Seriali_to_json(address , des);
    if( root == NULL)
    {
        return NULL;
    }

    char* json_str = cJSON_Print(root);

    cJSON_Delete(root);

    return json_str;
}
