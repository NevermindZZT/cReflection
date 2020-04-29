/**
 * @file reflection.c
 * @author Letter (nevermindzzt@gmail.cn)
 * @brief c reflection
 * @version 1.0.0
 * @date 2020-04-23
 * 
 * @copyright (c) 2020 Letter
 * 
 */
#include "reflection.h"
#include "string.h"
#include "obj_list.h"
#include "log.h"

/**
 * @brief 基本类型 Reflection 模型
 * 
 */
Reflection reflectBasicTypeModel[] =
{
    [0] = {0, REFLECT_TYPE_CHAR, sizeof(char), NULL, 0, NULL},
    [1] = {0, REFLECT_TYPE_OBJ, sizeof(char), NULL, 0, NULL},

    [2] = {0, REFLECT_TYPE_SHORT, sizeof(short), NULL, 0, NULL},
    [3] = {0, REFLECT_TYPE_OBJ, sizeof(short), NULL, 0, NULL},

    [4] = {0, REFLECT_TYPE_INT, sizeof(int), NULL, 0, NULL},
    [5] = {0, REFLECT_TYPE_OBJ, sizeof(int), NULL, 0, NULL},

    [6] = {0, REFLECT_TYPE_LONG, sizeof(long), NULL, 0, NULL},
    [7] = {0, REFLECT_TYPE_OBJ, sizeof(long), NULL, 0, NULL},

    [8] = {0, REFLECT_TYPE_FLOAT, sizeof(float), NULL, 0, NULL},
    [9] = {0, REFLECT_TYPE_OBJ, sizeof(float), NULL, 0, NULL},

    [10] = {0, REFLECT_TYPE_DOUBLE, sizeof(double), NULL, 0, NULL},
    [11] = {0, REFLECT_TYPE_OBJ, sizeof(double), NULL, 0, NULL},

    [12] = {0, REFLECT_TYPE_STRING, sizeof(char *), NULL, 0, NULL},
    [13] = {0, REFLECT_TYPE_OBJ, sizeof(char *), NULL, 0, NULL},
};

/**
 * @brief 新字符串(字符串复制)
 * 
 * @param str 原字符串
 * @return char* 复制得到的新字符串
 */
char *reflectNewString(char *str)
{
    int len = strlen(str);
    char *dest = REFLECT_MALLOC(len + 1);
    strcpy(dest, str);
    return dest;
}

/**
 * @brief 获取模型对象的大小
 * 
 * @param model Reflection 模型
 * @return size_t 大小
 */
size_t reflectGetObjSize(Reflection *model)
{
    Reflection *p = model;
    while (p->type != REFLECT_TYPE_OBJ)
    {
        p++;
    }
    return p->size;
}


/**
 * @brief 释放对象内存
 * 
 * @param obj 对象
 * @param model Reflection 模型
 * @param isPointer 是否为指针类型
 */
void reflectFreeObjEx(void *obj, Reflection *model, char isPointer)
{
    Reflection *p = model;
    while (p->type != REFLECT_TYPE_OBJ)
    {
        if (p->isPointer)
        {
            reflectFreeObjEx((void *)(*(size_t *)((size_t)obj + p->offset)), p->model, 1);
        }
        else if (p->type == REFLECT_TYPE_STRING)
        {
            REFLECT_FREE((char *)(*(size_t *)((size_t)obj + p->offset)));
        }
        else if (p->type == REFLECT_TYPE_ARRAY)
        {
            size_t itemSize = reflectGetObjSize(p->model);
            for (size_t i = 0; i < p->size; i++)
            {
                reflectFreeObjEx((void *)((size_t)obj + p->offset + itemSize * i), p->model, 0);
            }
        }
        else if (p->type == REFLECT_TYPE_STRUCT)
        {
            reflectFreeObjEx((void *)((size_t)obj + p->offset), p->model, 0);
        }
        else if (p->type == REFLECT_TYPE_LIST)
        {
            ObjList *list = (ObjList *)*(size_t *)((size_t)obj + p->offset);
            ObjList *item;
            while (list)
            {
                item = list;
                list = list->next;
                if (item->obj)
                {
                    reflectFreeObjEx(item->obj, p->model, 1);
                }
                REFLECT_FREE(item);
            }
        }
        p++;
    }
    if (isPointer)
    {
        REFLECT_FREE(obj);
    }
}
