/**
 * @file obj_list.c
 * @author Letter (nevermindzzt@gmail.cn)
 * @brief object list
 * @version 0.1
 * @date 2020-04-23
 * 
 * @copyright (c) 2020 Letter
 * 
 */
#include "reflection.h"
#include "obj_list.h"
#include "stddef.h"


/**
 * @brief 对象链表添加节点
 * 
 * @param list 链表
 * @param node 节点
 * @return ObjList* 链表
 */
ObjList *objListAddNode(ObjList *list, ObjList *node)
{
    if (!list)
    {
        node->next = NULL;
        return node;
    }

    ObjList *p = list;
    while (p->next)
    {
        p = p->next;
    }

    p->next = node;
    node->next = NULL;
    return list;
}

/**
 * @brief 对象链表添加对象
 * 
 * @param list 链表
 * @param obj 对象
 * @return ObjList* 链表
 */
ObjList *objListAdd(ObjList *list, void *obj)
{
    if (!list)
    {
        list = REFLECT_MALLOC(sizeof(ObjList));
        REFLECT_ASSERT(list, return NULL);
        list->obj = obj;
        list->next = NULL;
        return list;
    }

    ObjList *node = REFLECT_MALLOC(sizeof(ObjList));
    REFLECT_ASSERT(node, return list);
    node->obj = obj;
    return objListAddNode(list, node);
}

/**
 * @brief 对象链表删除节点
 * 
 * @param list 链表
 * @param node 待删除的节点
 * @return ObjList* 节点删除后的链表
 */
ObjList *objListDelNode(ObjList *list, ObjList *node)
{
    REFLECT_ASSERT(list, return NULL);

    ObjList head = {0};
    head.next = list;
    ObjList *p = &head;

    while (p->next)
    {
        if (p->next == node)
        {
            p->next = p->next->next ? p->next->next : NULL;
        }
        p = p->next;
    }
    return head.next;
}


/**
 * @brief 对象链表删除对象
 * 
 * @param list 链表
 * @param obj 待删除的对象
 * @return ObjList* 对象删除后的链表
 */
ObjList *objListDel(ObjList *list, void *obj)
{
    REFLECT_ASSERT(list, return NULL);

    ObjList head = {0};
    head.next = list;
    ObjList *p = &head;

    while (p->next)
    {
        if (p->next->obj && p->next->obj == obj)
        {
            p->next = p->next->next ? p->next->next : NULL;
        }
        p = p->next;
    }
    return head.next;
}


/**
 * @brief 获取链表大小
 * 
 * @param list 链表
 * @return size_t 链表大小
 */
size_t objListGetSize(ObjList *list)
{
    REFLECT_ASSERT(list, return 0);

    ObjList *p = list;
    size_t size = 0;
    while (p)
    {
        p = p->next;
        size ++;
    }
    return size;
}
