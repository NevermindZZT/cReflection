/**
 * @file reflection_test.c
 * @author Letter (nevermindzzt@gmail.cn)
 * @brief 
 * @version 0.1
 * @date 2020-04-23
 * 
 * @copyright (c) 2020 Letter
 * 
 */
#include "reflection.h"
#include "cerializable.h"
#include "log.h"
#include "shell.h"

typedef struct
{
    char *a;
    float b;
} SubSub;

Reflection subSubReflect[] =
{
    REFLECT_MODEL_STRING(SubSub, a),
    REFLECT_MODEL_FLOAT(SubSub, b),
    REFLECT_MODEL_OBJ(SubSub),
};

typedef struct
{
    char *a;
    int b;
    SubSub sub[2];
} Sub;

Reflection subReflect[] =
{
    REFLECT_MODEL_STRING(Sub, a),
    REFLECT_MODEL_INT(Sub, b),
    REFLECT_MODEL_ARRAY(Sub, sub, 2, subSubReflect),
    REFLECT_MODEL_OBJ(Sub),
};

typedef struct
{
    char *name;
    char *alias;
    int id;
    Sub *sub;
} Repo;

Reflection repoReflect[] =
{
    REFLECT_MODEL_STRING(Repo, name),
    REFLECT_MODEL_STRING(Repo, alias),
    REFLECT_MODEL_INT(Repo, id),
    REFLECT_MODEL_STRUCT_P(Repo, sub, subReflect),
    REFLECT_MODEL_OBJ(Repo),
};

void cerialTest(void)
{
    SubSub subSub = {"clh", 123.546};
    Sub sub = {"zkq", 0x100};
    sub.sub[0].a = "dsgahfdahfhda";
    sub.sub[0].b = 123.456;
    sub.sub[1].a = "45448465480";
    sub.sub[1].b = 94.456;
    Repo repo = {"hello world", "zjx", 0xFF, &sub};
    int size;
    void *mem = cSerialize(&repo, repoReflect, &size);
    logHexDump(LOG_ALL_OBJ, LOG_NONE, mem, size);
    Repo *r = (Repo *)cDeserialize(mem, repoReflect);
    logDebug("addr: %08x, name: %s, alias: %s id: %d", r, r->name, r->alias, r->id);
    logDebug("addr: %08x, sub, a: %s, b: %d", r->sub, r->sub->a, r->sub->b);
    logDebug("sub.sub, a: %s, b: %f", r->sub->sub[0].a, r->sub->sub[0].b);
    logDebug("sub.sub, a: %s, b: %f", r->sub->sub[1].a, r->sub->sub[1].b);
    reflectFreeObj(r, repoReflect);
    reflectFreeMem(mem);
}
SHELL_EXPORT_CMD(SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC),
cerialTest, cerialTest, c serialize test);


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
SHELL_EXPORT_CMD(SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC),
cerialDemo, cerialDemo, c serialize demo);
