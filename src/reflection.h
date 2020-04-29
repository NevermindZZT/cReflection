/**
 * @file reflection.h
 * @author Letter (nevermindzzt@gmail.cn)
 * @brief c reflection
 * @version 1.0.0
 * @date 2020-04-23
 * 
 * @copyright (c) 2020 Letter
 * 
 */
#ifndef __REFLECTION_H__
#define __REFLECTION_H__

#include "stddef.h"
#include "reflection_cfg.h"

#define REFLECTION_VERSION              "1.0.0-beta1"

/**
 * @defgroup REFLECTION reflection
 * @brief c reflection
 * @addtogroup REFLECTION
 * @{
 */

#define REFLECT_ASSERT(x, expr) \
        if (!x) { expr; }

#define REFLECT_BASIC_MODEL_CHAR        &reflectBasicTypeModel[0]      /**< char型链表数据模型 */
#define REFLECT_BASIC_MODEL_SHORT       &reflectBasicTypeModel[2]      /**< short型链表数据模型 */
#define REFLECT_BASIC_MODEL_INT         &reflectBasicTypeModel[4]      /**< int型链表数据模型 */
#define REFLECT_BASIC_MODEL_LONG        &reflectBasicTypeModel[6]      /**< long型链表数据模型 */
#define REFLECT_BASIC_MODEL_FLOAT       &reflectBasicTypeModel[8]      /**< float型链表数据模型 */
#define REFLECT_BASIC_MODEL_DOUBLE      &reflectBasicTypeModel[10]     /**< double型链表数据模型 */
#define REFLECT_BASIC_MODEL_STRING      &reflectBasicTypeModel[12]     /**< string型链表数据模型 */

/**
 * @brief Reflection 模型
 * 
 * @param isPointer 是否为指针类型
 * @param type Reflection 数据类型
 * @param size 大小
 * @param name 字段属性名
 * @param offset 字段在结构体中的偏移
 * @param model 子Reflection 模型
 */
#define REFLECT_MODEL(isPointer, type, size, name, offset, model) \
        {isPointer, type, size, name, offset, model}

/**
 * @brief Reflection 对象模型定义
 * 
 * @param type 对象(结构体)类型
 * @note 定义在每个Reflectoin 模型最后，必须要有
 */
#define REFLECT_MODEL_OBJ(type) \
        REFLECT_MODEL(0, REFLECT_TYPE_OBJ, sizeof(type), NULL, 0, NULL)

/**
 * @brief Reflection char 类型数据模型定义
 * 
 * @param type 对象(结构体)类型
 * @param key 字段名(结构体成员名)
 */
#define REFLECT_MODEL_CHAR(type, key) \
        REFLECT_MODEL(0, REFLECT_TYPE_CHAR, sizeof(char), #key, offsetof(type, key), REFLECT_BASIC_MODEL_CHAR)

/**
 * @brief Reflection char 指针类型数据模型定义
 * 
 * @param type 对象(结构体)类型
 * @param key 字段名(结构体成员名)
 */
#define REFLECT_MODEL_CHAR_P(type, key) \
        REFLECT_MODEL(1, REFLECT_TYPE_CHAR, sizeof(char *), #key, offsetof(type, key), REFLECT_BASIC_MODEL_CHAR)

/**
 * @brief Reflection short 类型数据模型定义
 * 
 * @param type 对象(结构体)类型
 * @param key 字段名(结构体成员名)
 */
#define REFLECT_MODEL_SHORT(type, key) \
        REFLECT_MODEL(0, REFLECT_TYPE_SHORT, sizeof(short), #key, offsetof(type, key), REFLECT_BASIC_MODEL_SHORT)

/**
 * @brief Reflection short 指针类型数据模型定义
 * 
 * @param type 对象(结构体)类型
 * @param key 字段名(结构体成员名)
 */
#define REFLECT_MODEL_SHORT_P(type, key) \
        REFLECT_MODEL(1, REFLECT_TYPE_SHORT, sizeof(short *), #key, offsetof(type, key), REFLECT_BASIC_MODEL_SHORT)

/**
 * @brief Reflection int 类型数据模型定义
 * 
 * @param type 对象(结构体)类型
 * @param key 字段名(结构体成员名)
 */
#define REFLECT_MODEL_INT(type, key) \
        REFLECT_MODEL(0, REFLECT_TYPE_INT, sizeof(int), #key, offsetof(type, key), REFLECT_BASIC_MODEL_INT)

/**
 * @brief Reflection int 指针类型数据模型定义
 * 
 * @param type 对象(结构体)类型
 * @param key 字段名(结构体成员名)
 */
#define REFLECT_MODEL_INT_P(type, key) \
        REFLECT_MODEL(1, REFLECT_TYPE_INT, sizeof(int *), #key, offsetof(type, key), REFLECT_BASIC_MODEL_INT)

/**
 * @brief Reflection long 类型数据模型定义
 * 
 * @param type 对象(结构体)类型
 * @param key 字段名(结构体成员名)
 */
#define REFLECT_MODEL_LONG(type, key) \
        REFLECT_MODEL(0, REFLECT_TYPE_LONG, sizeof(long), #key, offsetof(type, key), REFLECT_BASIC_MODEL_LONG)

/**
 * @brief Reflection long 指针类型数据模型定义
 * 
 * @param type 对象(结构体)类型
 * @param key 字段名(结构体成员名)
 */
#define REFLECT_MODEL_LONG_P(type, key) \
        REFLECT_MODEL(1, REFLECT_TYPE_LONG, sizeof(long *), #key, offsetof(type, key), REFLECT_BASIC_MODEL_LONG)

/**
 * @brief Reflection float 类型数据模型定义
 * 
 * @param type 对象(结构体)类型
 * @param key 字段名(结构体成员名)
 */
#define REFLECT_MODEL_FLOAT(type, key) \
        REFLECT_MODEL(0, REFLECT_TYPE_FLOAT, sizeof(float), #key, offsetof(type, key), REFLECT_BASIC_MODEL_FLOAT)

/**
 * @brief Reflection float 指针类型数据模型定义
 * 
 * @param type 对象(结构体)类型
 * @param key 字段名(结构体成员名)
 */
#define REFLECT_MODEL_FLOAT_P(type, key) \
        REFLECT_MODEL(1, REFLECT_TYPE_FLOAT, sizeof(float *), #key, offsetof(type, key), REFLECT_BASIC_MODEL_FLOAT)

/**
 * @brief Reflection double 类型数据模型定义
 * 
 * @param type 对象(结构体)类型
 * @param key 字段名(结构体成员名)
 */
#define REFLECT_MODEL_DOUBLE(type, key) \
        REFLECT_MODEL(0, REFLECT_TYPE_DOUBLE, sizeof(double), #key, offsetof(type, key), REFLECT_BASIC_MODEL_DOUBLE)

/**
 * @brief Reflection double 指针类型数据模型定义
 * 
 * @param type 对象(结构体)类型
 * @param key 字段名(结构体成员名)
 */
#define REFLECT_MODEL_DOUBLE_P(type, key) \
        REFLECT_MODEL(1, REFLECT_TYPE_DOUBLE, sizeof(double *), #key, offsetof(type, key), REFLECT_BASIC_MODEL_DOUBLE)

/**
 * @brief Reflection 字符串类型数据模型定义
 * 
 * @param type 对象(结构体)类型
 * @param key 字段名(结构体成员名)
 */
#define REFLECT_MODEL_STRING(type, key) \
        REFLECT_MODEL(0, REFLECT_TYPE_STRING, sizeof(char *), #key, offsetof(type, key), REFLECT_BASIC_MODEL_STRING)

/**
 * @brief Reflection 子结构体类型数据模型定义
 * 
 * @param type 对象(结构体)类型
 * @param size 子结构体大小
 * @param key 字段名(结构体成员名)
 * @param model Reflection 模型
 */
#define REFLECT_MODEL_STRUCT(type, key, size, model) \
        REFLECT_MODEL(0, REFLECT_TYPE_STRUCT, size, #key, offsetof(type, key), model)

/**
 * @brief Reflection 子结构体指针类型数据模型定义
 * 
 * @param type 对象(结构体)类型
 * @param key 字段名(结构体成员名)
 * @param model Reflection 模型
 */
#define REFLECT_MODEL_STRUCT_P(type, key, model) \
        REFLECT_MODEL(1, REFLECT_TYPE_STRUCT, sizeof(void *), #key, offsetof(type, key), model)

/**
 * @brief Reflection 数组类型数据模型定义
 * 
 * @param type 对象(结构体)类型
 * @param size 数组大小
 * @param key 字段名(结构体成员名)
 * @param model Reflection 模型
 */
#define REFLECT_MODEL_ARRAY(type, key, size, model) \
        REFLECT_MODEL(0, REFLECT_TYPE_ARRAY, size, #key, offsetof(type, key), model)

/**
 * @brief Reflection 链表类型数据模型定义
 * 
 * @param type 对象(结构体)类型
 * @param key 字段名(结构体成员名)
 * @param model Reflection 模型
 */
#define REFLECT_MODEL_LIST(type, key, model) \
        REFLECT_MODEL(0, REFLECT_TYPE_LIST, sizeof(void *), #key, offsetof(type, key), model)


/**
 * @brief Reflection 数据类型
 * 
 */
typedef enum
{
    REFLECT_TYPE_OBJ = 0,

    REFLECT_TYPE_CHAR,
    REFLECT_TYPE_SHORT,
    REFLECT_TYPE_INT,
    REFLECT_TYPE_LONG,
    REFLECT_TYPE_FLOAT,
    REFLECT_TYPE_DOUBLE,
    REFLECT_TYPE_STRING,

    REFLECT_TYPE_STRUCT,
    REFLECT_TYPE_ARRAY,
    REFLECT_TYPE_LIST
} ReflectionType;

/**
 * @brief Reflection 数据模型定义
 * 
 */
typedef struct reflection_def
{
    unsigned char isPointer : 1;                /**< 是否为指针类型数据 */
    ReflectionType type;                        /**< 数据类型 */
    unsigned short size;                        /**< 数据大小 */
    char *name;                                 /**< 字段名 */
    short offset;                               /**< 偏移 */
    struct reflection_def *model;               /**< 子数据模型 */
} Reflection;

extern Reflection reflectBasicTypeModel[];      /**< 基础类型链表数据模型 */

/**
 * @brief 新字符串(字符串复制)
 * 
 * @param str 原字符串
 * @return char* 复制得到的新字符串
 */
char *reflectNewString(char *str);

/**
 * @brief 获取模型对象的大小
 * 
 * @param model Reflection 模型
 * @return size_t 大小
 */
size_t reflectGetObjSize(Reflection *model);

/**
 * @brief 释放对象内存
 * 
 * @param obj 对象
 * @param model Reflection 模型
 * @param isPointer 是否为指针类型
 */
void reflectFreeObjEx(void *obj, Reflection *model, char isPointer);

/**
 * @brief 释放对象内存
 * 
 * @param obj 对象
 * @param model Reflection 模型
 */
#define reflectFreeObj(obj, model) \
        reflectFreeObjEx(obj, model, 1)

/**
 * @brief 释放内存
 * 
 * @param mem 内存
 */
#define reflectFreeMem(mem) \
        REFLECT_FREE(mem)

/**
 * @}
 */

#endif
