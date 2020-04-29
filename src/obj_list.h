/**
 * @file obj_list.h
 * @author Letter (nevermindzzt@gmail.cn)
 * @brief object list
 * @version 0.1
 * @date 2020-04-23
 * 
 * @copyright (c) 2020 Letter
 * 
 */
#ifndef __OBJ_LIST_H__
#define __OBJ_LIST_H__

/**
 * @defgroup OBJ_LIST object_list
 * @brief object list
 * @addtogroup OBJ_LIST
 * @{
 */

/**
 * @brief 对象链表
 * 
 */
typedef struct obj_lsit
{
    void *obj;                                  /**< 对象 */
    struct obj_lsit *next;                      /**< 下一个节点指针 */
} ObjList;

/**
 * @brief 对象链表添加节点
 * 
 * @param list 链表
 * @param node 节点
 * @return ObjList* 链表
 */
ObjList *objListAddNode(ObjList *list, ObjList *node);

/**
 * @brief 对象链表添加对象
 * 
 * @param list 链表
 * @param obj 对象
 * @return ObjList* 链表
 */
ObjList *objListAdd(ObjList *list, void *obj);

/**
 * @brief 对象链表删除节点
 * 
 * @param list 链表
 * @param node 待删除的节点
 * @return ObjList* 节点删除后的链表
 */
ObjList *objListDelNode(ObjList *list, ObjList *node);

/**
 * @brief 对象链表删除对象
 * 
 * @param list 链表
 * @param obj 待删除的对象
 * @return ObjList* 对象删除后的链表
 */
ObjList *objListDel(ObjList *list, void *obj);

/**
 * @brief 获取链表大小
 * 
 * @param list 链表
 * @return size_t 链表大小
 */
size_t objListGetSize(ObjList *list);

/**
 * @}
 */

#endif
