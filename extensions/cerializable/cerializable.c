/**
 * @file cerializable.c
 * @author Letter (nevermindzzt@gmail.cn)
 * @brief c serializable
 * @version 0.1
 * @date 2020-04-26
 * 
 * @copyright (c) 2020 Letter
 * 
 */
#include "cerializable.h"
#include "string.h"
#include "obj_list.h"
#include "log.h"


/**
 * @brief 获取对象所占用的内存大小
 * 
 * @param obj 对象
 * @param model Reflection 模型
 * @return size_t 占用内存大小
 */
size_t cSerialGetObjSize(void *obj, Reflection *model)
{
    Reflection *p = model;
    size_t size = 0;
    while (p->type != REFLECT_TYPE_OBJ)
    {
        if (p->isPointer)
        {
            size += cSerialGetObjSize((void *)(*(size_t *)((size_t)obj + p->offset)), p->model);
        }
        else if (p->type == REFLECT_TYPE_STRING)
        {
            size += (strlen((char *)(*(size_t *)((size_t)obj + p->offset))) + sizeof(size_t)) & (~(sizeof(size_t) - 1));
        }
        else if (p->type == REFLECT_TYPE_ARRAY)
        {
            size_t itemSize = reflectGetObjSize(p->model);
            for (size_t i = 0; i < p->size; i++)
            {
                size += cSerialGetObjSize(
                    (void *)((size_t)obj + p->offset + itemSize * i),
                    p->model) - itemSize;
            }
        }
        else if (p->type == REFLECT_TYPE_STRUCT)
        {
            size += cSerialGetObjSize(
                (void *)((size_t)obj + p->offset),
                p->model) - reflectGetObjSize(p->model);
        }
        else if (p->type == REFLECT_TYPE_LIST)
        {
            size_t listSize = objListGetSize((ObjList *)(*(size_t *)((size_t)obj + p->offset)));
            size += sizeof(ObjList) * listSize;
            size += cSerialGetObjSize((void *)(*(size_t *)((size_t)obj + p->offset)), p->model) * listSize;
        }
        p++;
    }

    size += (p->size + sizeof(size_t) - 1) & (~(sizeof(size_t) - 1));
    return size;
}


/**
 * @brief 序列化对象
 * 
 * @param obj 对象
 * @param objAddr 序列化对象地址
 * @param memAddr 当前可写数据的内存地址
 * @param model Reflection 模型
 * @param isPointer 对象是否为指针形式
 * @return size_t 占用空间大小
 */
static size_t cSerialObj(void *obj, size_t objAddr, size_t memAddr,
                        Reflection *model, char isPointer)
{
    void *startAddr = (void *)memAddr;
    size_t size = reflectGetObjSize(model);
    memcpy((void *)objAddr, obj, size);
    if (isPointer)
    {
        memAddr += (size + sizeof(size_t) - 1) & (~(sizeof(size_t) - 1));
    }

    Reflection *p = model;
    while (p->type != REFLECT_TYPE_OBJ)
    {
        if (p->isPointer)
        {
            *(size_t *)((size_t)objAddr + p->offset) = memAddr - ((size_t)objAddr + p->offset);
            memAddr += cSerialObj(
                (void *)(*(size_t *)((size_t)obj + p->offset)),
                memAddr,
                memAddr,
                p->model,
                1);
        }
        else if (p->type == REFLECT_TYPE_STRING)
        {
            *(size_t *)((size_t)objAddr + p->offset) = memAddr - ((size_t)objAddr + p->offset);
            strcpy((char *)memAddr, (char *)(*(size_t *)((size_t)obj + p->offset)));
            memAddr += (strlen((char *)(*(size_t *)((size_t)obj + p->offset))) + sizeof(size_t))
                & (~(sizeof(size_t) - 1));
        }
        else if (p->type == REFLECT_TYPE_ARRAY)
        {
            size_t itemSize = reflectGetObjSize(p->model);
            for (size_t i = 0; i < p->size; i++)
            {
                memAddr += cSerialObj(
                    (void *)((size_t)obj + p->offset + itemSize * i),
                    (size_t)objAddr + p->offset + itemSize * i,
                    memAddr,
                    p->model,
                    0);
            }
        }
        else if (p->type == REFLECT_TYPE_STRUCT)
        {
            memAddr += cSerialObj(
                (void *)((size_t)obj + p->offset),
                (size_t)objAddr + p->offset,
                memAddr,
                p->model,
                0);
        }
        else if (p->type == REFLECT_TYPE_LIST)
        {
            ObjList *list = (ObjList *)((size_t)obj + p->offset);
            size_t listSize = objListGetSize(list);
            if (listSize == 0)
            {
                *(size_t *)((size_t)objAddr + p->offset) = 0;
            }
            else
            {
                *(size_t *)((size_t)objAddr + p->offset) 
                    = memAddr - ((size_t)objAddr + p->offset);
                ObjList *listObj = (ObjList *)memAddr;
                memAddr += sizeof(ObjList) * listSize;
                while (list)
                {
                    listObj->obj = (void *)(memAddr - (size_t)(listObj->obj));
                    listObj->next = list->next ? (ObjList *)sizeof(ObjList) : 0;
                    memAddr += cSerialObj(
                        listObj->obj,
                        memAddr,
                        memAddr,
                        p->model,
                        1);
                    listObj ++;
                    list = list->next;
                }
            }
        }
        p++;
    }
    return memAddr - (size_t)startAddr;
}


/**
 * @brief 序列化
 * 
 * @param obj 对象
 * @param model Reflection 模型
 * @param size 序列化后的数据大小
 * @return void* 序列化得到的数据地址
 */
void *cSerialize(void *obj, Reflection *model, size_t *size)
{
    *size = cSerialGetObjSize(obj, model);
    void *mem = REFLECT_MALLOC(*size);
    REFLECT_ASSERT(mem, return NULL);
    cSerialObj(obj, (size_t)mem, (size_t)mem, model, 1);
    return mem;
}


/**
 * @brief 反序列化对象
 * 
 * @param mem 序列化数据地址
 * @param model Reflection 模型
 * @param obj 对象
 * @return void* 反序列化得到的对象
 */
static void *cDeserialObj(void *mem, Reflection *model, void *obj)
{
    size_t size = reflectGetObjSize(model);
    if (obj == NULL)
    {
        obj = REFLECT_MALLOC(size);
    }
    REFLECT_ASSERT(obj, return NULL);
    memcpy(obj, mem, size);

    Reflection *p = model;
    while (p->type != REFLECT_TYPE_OBJ)
    {
        if (p->isPointer)
        {
            *(size_t *)((size_t)obj + p->offset) = 
                (size_t)cDeserialObj(
                    (void *)(*(size_t *)((size_t)mem + p->offset) + ((size_t)mem + p->offset)),
                    p->model,
                    NULL);
        }
        else if (p->type == REFLECT_TYPE_STRING)
        {   
            *(size_t *)((size_t)obj + p->offset) = 
                (size_t)reflectNewString(
                    (char *)((*(size_t *)((size_t)mem + p->offset)) + ((size_t)mem + p->offset)));
        }
        else if (p->type == REFLECT_TYPE_ARRAY)
        {
            size_t itemSize = reflectGetObjSize(p->model);
            for (size_t i = 0; i < p->size; i++)
            {
                cDeserialObj(
                   (void *)((size_t)mem + p->offset + itemSize * i),
                   p->model,
                   (void *)((size_t)obj + p->offset + itemSize * i));
            }
        }
        else if (p->type == REFLECT_TYPE_STRUCT)
        {
            cDeserialObj(
                (void *)((size_t)mem + p->offset),
                p->model,
                (void *)((size_t)obj + p->offset));
        }
        else if (p->type == REFLECT_TYPE_LIST)
        {
            if (*(size_t *)((size_t)mem + p->offset) != 0)
            {
                size_t index = 0;
                ObjList *list;
                ObjList *item = NULL;
                do {
                    if (index == 0)
                    {
                        *(size_t *)((size_t)obj + p->offset) = 
                            (size_t)REFLECT_MALLOC(sizeof(ObjList));
                        item = (ObjList *)(*(size_t *)((size_t)obj + p->offset));
                    }
                    else
                    {
                        item->next = REFLECT_MALLOC(sizeof(ObjList));
                        item = item->next;
                    }
                    list = (ObjList *)(*(size_t *)((size_t)mem + p->offset) 
                        + ((size_t)mem + p->offset) + sizeof(ObjList) * index);
                    item->obj = cDeserialObj(
                        (void *)((size_t)list->obj + *(size_t *)list->obj),
                        p->model,
                        NULL);
                } while (list->next);
            }
        }
        p++;
    }
    return obj;
}


/**
 * @brief 反序列化
 * 
 * @param mem 序列化数据地址
 * @param model Reflection 模型
 * @return void* 反序列化得到的对象
 */
void *cDeserialize(void *mem, Reflection *model)
{
    REFLECT_ASSERT(mem, return NULL);
    return cDeserialObj(mem, model, NULL);
}
