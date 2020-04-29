# Cerializable (C Serializable)

C语言序列化，反序列化

- [Cerializable (C Serializable)](#cerializable-c-serializable)
  - [简介](#简介)
  - [使用](#使用)
  - [Api](#api)

## 简介

`Cerializable`是一个C语言编写的，基于[C Reflection](https://github.com/NevermindZZT/cReflection)，用于实现C语言数据序列化和反序列化的模块，可以用于C语言程序结构体数据传输，交换，保存等

## 使用

`Cerializable`是基于[C Reflection](https://github.com/NevermindZZT/cReflection)的模块，使用`Cerializable`需要先对结构体定义好`Reflection 模型`，参考[C Reflection](../readme.md)

下面以如下的结构体为例：

```C
/**
 * @brief 工程结构体
 */
typedef struct
{
    int id;
    char *name;
} Project;

/**
 * @brief 仓库结构体
 */
typedef struct
{
    int id;
    char *user;
    Project *project;
} Hub;
```

1. 定义Reflection模型

    定义两个Reflection模型，分别描述`Project`和`Hub`

    ```C
    /**
    * @brief 工程结构体 Reflection 模型
     */
    Reflection projectReflection[] =
    {
        REFLECT_MODEL_INT(Project, id),
        REFLECT_MODEL_STRING(Project, name),
        REFLECT_MODEL_OBJ(Project)
    };

    /**
     * @brief 仓库结构体 Reflection 模型
     */
    Reflection hubReflection[] =
    {
        REFLECT_MODEL_INT(Hub, id),
        REFLECT_MODEL_STRING(Hub, user),
        REFLECT_MODEL_STRUCT_P(Hub, project, projectReflection),
        REFLECT_MODEL_OBJ(Hub)
    };
    ```

2. 使用cerializable接口进行序列化和反序列化

    这里以一个测试函数为例

    ```C
    void cerialDemo(void)
    {
        Project project = {255, "c serializable"};
        Hub hub = {65535, "Letter", &project};
        size_t size;

        /* 序列化 */
        void *mem = cSerialize(&hub, hubReflection, &size);
        logHexDump(LOG_ALL_OBJ, LOG_NONE, mem, size);

        /* 反序列化 */
        Hub *obj = (Hub *)cDeserialize(mem, hubReflection);
        logDebug("hub id: %d, user: %s, projedt id: %d, name: %s",
            obj->id, obj->user, obj->project->id, obj->project->name);

        /* 释放内存 */
        reflectFreeMem(mem);
        reflectFreeObj(obj, hubReflection);
    }
    ```

    执行得到如下结果：

    ```sh
    memory of 0x20001368, size: 44:
    Offset: 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F
    0x20001360:                         ff ff 00 00 08 00 00 00 |         ........ |
    0x20001370: 0c 00 00 00 4c 65 74 74 65 72 00 00 ff 00 00 00 | ....Letter...... |
    0x20001380: 04 00 00 00 63 20 73 65 72 69 61 6c 69 7a 61 62 | ....c serializab |
    0x20001390: 6c 65 00 00                                     | le..             |

    D(13962) cerialDemo: hub id: 65535, user: Letter, projedt id: 255, name: c serializable
    ````

    可以看到，执行序列化操作后，数据被储存在一块连续的内存中，对于这种连续的数据，就可以进行传递，存储等操作，再执行反序列化操作后，连续的数据被转化成了一个结构体数据，可以直接进行结构体操作

    无论是序列化或者时反序列化，使用`Cerializable`，都只需要调用一个Api即可实现

## Api

`Cerializable`源文件有完整的注释，可以通过Doxygen等工具导出完整的API文档，以下是几个关键API的说明

- 序列化

  序列化结构体，将结构体转换为一块连续的内存数据

  ```C
  /**
   * @brief 序列化
   *
   * @param obj 对象
   * @param model Reflection 模型
   * @param size 序列化后的数据大小
   * @return void* 序列化得到的数据地址
   */
  void *cSerialize(void *obj, Reflection *model, size_t *size);
  ```

  - 参数
    - `obj` 对象
    - `model` Reflection 模型
    - `size` 序列化后的数据大小(输出参数)

  - 返回
    - `void*` 序列化得到的数据地址

- 反序列化

  反序列化数据，将数据转化为结构体

  ```C
  /**
   * @brief 反序列化
   *
   * @param mem 序列化数据地址
   * @param model Reflection 模型
   * @return void* 反序列化得到的对象
   */
  void *cDeserialize(void *mem, Reflection *model);
  ```

  - 参数
    - `mem` 序列化数据地址
    - `model` Reflection 模型

  - 返回
    - `void*` 反序列化得到的对象
