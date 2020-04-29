/**
 * @file cerializable.h
 * @author Letter (nevermindzzt@gmail.cn)
 * @brief c serializable
 * @version 0.1
 * @date 2020-04-26
 * 
 * @copyright (c) 2020 Letter
 * 
 */

#ifndef __CERIALIZABLE_H__
#define __CERIALIZABLE_H__

#include "reflection.h"

#define CERIALIZABLE_VERSION        "1.0.0-beta1"

/**
 * @defgroup CERIALIZABLE cerializable
 * @brief c serializable
 * @addtogroup CERIALIZABLE
 * @{
 */

/**
 * @brief 序列化
 * 
 * @param obj 对象
 * @param model Reflection 模型
 * @param size 序列化后的数据大小
 * @return void* 序列化得到的数据地址
 */
void *cSerialize(void *obj, Reflection *model, size_t *size);

/**
 * @brief 反序列化
 * 
 * @param mem 序列化数据地址
 * @param model Reflection 模型
 * @return void* 反序列化得到的对象
 */
void *cDeserialize(void *mem, Reflection *model);

/**
 * @}
 */

#endif /* __CERIALIZABLE_H__ */
