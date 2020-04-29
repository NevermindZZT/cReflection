# C Reflection

![c reflection](https://img.shields.io/badge/c_reflection-1.0.0_beta1-brightgreen.svg)
![cerializable](https://img.shields.io/badge/c_reflection-1.0.0_beta1-brightgreen.svg)
![standard](https://img.shields.io/badge/standard-c99-brightgreen.svg)
![build](https://img.shields.io/badge/build-2020.04.28-brightgreen.svg)
![license](https://img.shields.io/badge/license-MIT-brightgreen.svg)

C语言类反射机制实现

- [C Reflection](#c-reflection)
  - [简介](#简介)
  - [模块](#模块)
  - [配置](#配置)
  - [模型定义](#模型定义)
  - [对象链表](#对象链表)
  - [Api](#api)
    - [C Reflection Api](#c-reflection-api)
    - [对象链表Api](#对象链表api)

## 简介

[C Reflection](https://github.com/NevermindZZT/cReflection)是一个针对C语言的，类似于高级语言反射机制的实现，`C Reflection`通过使用数据结构体(Reflection 模型)对C语言结构体进行描述，通过内存直接操作数据

`C Reflection`本身并没有提供实用的功能，只是对于类反射机制的实现，并提供了几个通用的接口，比如结构体内存释放，基于`C Reflection`，可以实现一些需要借助反射机制才可实现的实用工具

## 模块

`C Reflection`本身不提供实用工具，基于`C Reflection`实现的工具称之为模块，目前已经实现的模块如下：

| 模块                                | 简介                                                             |
| ----------------------------------- | ---------------------------------------------------------------- |
| [cerializable](doc/cerializable.md) | C语言序列化，反序列化工具，可以直接将C语言结构体和数据块进行转换 |

## 配置

`C Reflecion`需要使用内存管理的函数`malloc`和`free`，再使用前，需要在`reflection_cfg.h`文件中配置这两个函数

```C
/**
 * @brief 内存分配函数
 */
#define REFLECT_MALLOC          malloc

/**
 * @brief 内存释放函数
 */
#define REFLECT_FREE            free
```

如果不使用标准库的内存管理，只需要对应地修改这两个宏即可

## 模型定义

`C Reflection`使用`Reflection 模型`对数据进行描述，`Reflection 模型`是一个结构体数组，成员类型为`Reflection`，`C Reflection`定义了一系列宏，可以简化模型的定义

示例如下：

```C
typedef struct
{
    int id;
    char *name;
} Repo;

Reflection repoReflection[] =
{
    REFLECT_MODEL_INT(Repo, id),
    REFLECT_MODEL_STRING(Repo, name),
    REFLECT_MODEL_OBJ(Repo),
}
```

结构体`Repo`共有两个成员，整型数据`id`和字符串`name`，使用`Reflection 模型`对这个结构体中的每个成员进行描述，在模型最后，需要加上`REFLECT_MODEL_OBJ(Repo)`，表示模型的结束

`C Reflection`模型定义相关的宏说明如下：

| 宏                                                        | 对应数据类型 | 备注                                                 |
| --------------------------------------------------------- | ------------ | ---------------------------------------------------- |
| REFLECT_MODEL(isPointer, type, size, name, offset, model) | ALL          | 通用的模型定义宏，可直接定义任何参数，一般不需要用到 |
| REFLECT_MODEL_OBJ(type)                                   | struct       | 定义一个对象，必须在每个模型最后定义                 |
| REFLECT_MODEL_CHAR(type, key)                             | char         | 定义一个char类型数据                                 |
| REFLECT_MODEL_CHAR_P(type, key)                           | char *       | 定义一个指向char类型数据的指针                       |
| REFLECT_MODEL_SHORT(type, key)                            | short        | 定义一个short类型数据                                |
| REFLECT_MODEL_SHORT_P(type, key)                          | short *      | 定义一个指向short类型数据的指针                      |
| REFLECT_MODEL_INT(type, key)                              | int          | 定义一个int类型数据                                  |
| REFLECT_MODEL_INT_P(type, key)                            | int *        | 定义一个指向int类型数据的指针                        |
| REFLECT_MODEL_LONG(type, key)                             | long         | 定义一个long类型数据                                 |
| REFLECT_MODEL_LONG_P(type, key)                           | long *       | 定义一个指向long类型数据的指针                       |
| REFLECT_MODEL_FLOAT(type, key)                            | float        | 定义一个float类型数据                                |
| REFLECT_MODEL_FLOAT_P(type, key)                          | float *      | 定义一个指向float类型数据的指针                      |
| REFLECT_MODEL_DOUBLE(type, key)                           | double       | 定义一个double类型数据                               |
| REFLECT_MODEL_DOUBLE_P(type, key)                         | double *     | 定义一个指向double类型数据的指针                     |
| REFLECT_MODEL_STRING(type, key)                           | char *       | 定义一个字符串类型数据                               |
| REFLECT_MODEL_STRUCT(type, key, size, model)              | struct       | 定义一个子结构体(非指针形式)                         |
| REFLECT_MODEL_STRUCT_P(type, key, model)                  | struct *     | 定义一个子结构体(指针形式)                           |
| REFLECT_MODEL_ARRAY(type, key, size, model)               | array        | 定义一个数组(位于结构体中)                           |
| REFLECT_MODEL_LIST(type, key, model)                      | list         | 定义一个链表(ObjList *)                              |

## 对象链表

为了方便操作，`C Reflection`实现了一个链表，`C Reflecion`以及使用`C Reflection`实现的模块都使用这个链表进行操作

```C
typedef struct obj_lsit
{
    void *obj;                                  /**< 对象 */
    struct obj_lsit *next;                      /**< 下一个节点指针 */
} ObjList;
```

## Api

`C Reflection`源文件有完整的注释，可以通过Doxygen等工具导出完整的API文档，以下是几个关键API的说明

### C Reflection Api

`C Reflection`的Api主要提供了几个基础的操作接口，包括字符串复制，对象内存释放等

- 新字符串(字符串复制)
  
  从原字符串复制新的字符串，方便使用`C Reflection`内存操作

  ```C
  /**
   * @brief 新字符串(字符串复制)
   *
   * @param str 原字符串
   * @return char* 复制得到的新字符串
   */
  char *reflectNewString(char *str);
  ```

  - 参数
    - `str` 原字符串
  
  - 返回
    - `char *` 复制得到的新字符串

- 释放对象内存

  使用`Reflection 模型`一次性释放结构体(包含子结构体)所有关联内存

  ```C
  /**
   * @brief 释放对象内存
   *
   * @param obj 对象
   * @param model Reflection 模型
   */
  #define reflectFreeObj(obj, model) \
          reflectFreeObjEx(obj, model, 1)
  ```

  - 参数
    - `obj` 对象
    - `model` Reflection 模型

- 释放数据内存

  释放数据内存

  ```C
  /**
   * @brief 释放内存
   *
   * @param mem 内存
   */
  #define reflectFreeMem(mem) \
          REFLECT_FREE(mem)
  ```

  - 参数
    - `mem` 内存

### 对象链表Api

对象链表Api用于操作对象链表，包括对象的添加，删除等

- 对象添加

  向对象链表中添加新的对象

  ```C
  /**
   * @brief 对象链表添加对象
   *
   * @param list 链表
   * @param obj 对象
   * @return ObjList* 链表
   */
  ObjList *objListAdd(ObjList *list, void *obj);
  ```

  - 参数
    - `list` 链表
    - `obj` 待添加的对象

  - 返回
    - `Objlist *` 添加对象后的链表

- 对象删除

  从对象链表中删除对象

  ```C
  /**
   * @brief 对象链表删除对象
   *
   * @param list 链表
   * @param obj 待删除的对象
   * @return ObjList* 对象删除后的链表
   */
  ObjList *objListDel(ObjList *list, void *obj);
  ```

  - 参数
    - `list` 链表
    - `obj` 待删除的对象

  - 返回
    - `Objlist *` 删除对象后的链表
