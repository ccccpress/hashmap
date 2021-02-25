#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "hash.h"
// kv_ 是保存数据的数组
// kv0 是保存数据的链表元素
// max 是数组的长度，也被当作NULL的一个枚举
// now 是当前链表元素总数

int max = 8;
int now = 0;
char *null = "null";

typedef struct kv
{
    char *key;
    char *value;
    struct kv *next;
} kv;

kv *empty = (kv *)&max; // 随便搞个变量当枚举类型了

kv *kv_;

// 初始化这个字典
void make_kv()
{
    kv_ = (kv *)malloc(max * sizeof(kv));
    for (int i = 0; i < max; i++)
    {
        kv_[i].next = empty;
    }
    return;
}
void add_temp(kv *kv_2, char *key, char *value, unsigned int c) // 如果之前计算过hash就传过来 qaq
{
    int len1 = strlen(key) + 1;
    int len2 = strlen(value) + 1;

    kv *kv0 = (kv *)malloc(sizeof(kv));
    char *a = (char *)malloc(len1 * sizeof(char));
    char *b = (char *)malloc(len2 * sizeof(char));

    kv0->key = memcpy(a, key, len1);
    kv0->value = memcpy(b, value, len2);
    kv0->next = NULL;

    kv *temp = &kv_2[c];

    if (temp->next == empty) // 数组上是空的
    {
        temp->key = kv0->key;
        temp->value = kv0->value;
        temp->next = NULL; // 如果从len_kv来，就必须是NULL
    }
    else // 数组上有东西
    {
        while (temp->next != NULL) // 这里是个死循环 bug已修复
        {
            temp = temp->next;
        }
        temp->next = kv0;
    }
}
// 扩容 & 缩小
void len_kv(double what)
{
    kv *kv_2 = (kv *)malloc(what * max * sizeof(kv)); // 新的数组
    for (int i = 0; i < what * max; i++)
    {
        kv_2[i].next = empty;
    }
    for (int i = 0; i < max; i++)
    {

        kv *temp = &kv_[i];
        if (temp->next == empty) // 但我现在不敢删
        {
            continue;
        }
        unsigned int c = hash(temp->key, what * max);
        add_temp(kv_2, temp->key, temp->value, c);
        while (temp->next != NULL)
        {
            temp = temp->next;
            unsigned int c = hash(temp->key, what * max);
            add_temp(kv_2, temp->key, temp->value, c);
        }
    }
    free(kv_); // 我猜这里没有内存泄漏 Orz
    kv_ = kv_2;
    max = what * max;
}

// 删除一个元素
void del_kv(char *key)
{
    unsigned int c = hash(key, max);
    if (*kv_[c].key == *key) // 在数组
    {
        if (kv_[c].next == NULL) // 后面没有了
        {
            kv_[c].next = empty;
            free(kv_[c].key);
            free(kv_[c].value);
        }
        else // 后面还有的，就接过来
        {
            free(kv_[c].key);
            free(kv_[c].value);
            kv_[c].key = kv_[c].next->key;
            kv_[c].value = kv_[c].next->value;
            kv *temp = kv_[c].next;
            kv_[c].next = kv_[c].next->next;
            free(temp); // 这句话可能不对，先注释了
        }
        now--;
        return;
    }

    kv *temp1 = &kv_[c]; // 在链表
    kv *temp2 = kv_[c].next;
    while (temp1->next != NULL) // 这个地方改过
    {
        if (*temp2->key == *key)
        {
            if (temp2->next == NULL)
            {
                temp1->next = NULL;
            }
            else
            {
                temp1->next = temp2->next;
            }
            free(temp2->key);
            free(temp2->value);
            free(temp2);
            now--;
            return;
        }
        temp1 = temp1->next;
        temp2 = temp2->next;
    }
    printf("dont hava this key: %s\n", key);
}

// 遍历打印输出
void print_kv()
{
    for (int i = 0; i < max; i++)
    {
        kv *temp = &kv_[i];
        if (temp->next == empty)
        {
            continue;
        }
        printf("[%d] [%s] [%s]\n", i, temp->key, temp->value);
        while (temp->next != NULL)
        {
            temp = temp->next;
            printf("[%d] [%s] [%s]\n", i, temp->key, temp->value);
        }
    }
}

// 获取一个元素的值
char *get_kv(char *key)
{
    unsigned int c = hash(key, max);
    if (kv_[c].next == empty) // 数组本身就是空的！没想到吧
    {
        return null;
    }
    if (*kv_[c].key == *key) // 在数组
    {
        return kv_[c].value;
    }

    kv *temp1 = &kv_[c];
    while (temp1->next != NULL) // 在链表
    {
        temp1 = temp1->next;
        if (*temp1->key == *key)
        {
            return temp1->value;
        }
    }
    return null; // 这个地方可能有问题，好像没问题
}

// 改变一个元素的值，如果没有这个元素，就新增一个
void put_kv(char *key, char *value)
{
    // 测试阶段，先不扩容
    if (now == 2.0 * max)
    {
        len_kv(2);
    }
    if ((now == 0.5 * max) && (now > 8))
    {
        len_kv(0.5);
    }
    int len2 = strlen(value) + 1;
    unsigned int c = hash(key, max);

    if (kv_[c].next == empty)
    {
        add_temp(kv_, key, value, c); // 什么都没有还要改，只能增加了Orz
        now++;
        return;
    }

    if (*kv_[c].key == *key) // 在数组
    {
        free(kv_[c].value);
        char *b = (char *)malloc(len2 * sizeof(char));
        kv_[c].value = memcpy(b, value, len2);
        return;
    }

    kv *temp1 = &kv_[c];
    while (temp1->next != NULL) // 在链表
    {
        temp1 = temp1->next;
        if (*temp1->key == *key)
        {
            free(temp1->value);
            char *b = (char *)malloc(len2 * sizeof(char));
            temp1->value = memcpy(b, value, len2);
            return;
        }
    }
    add_temp(kv_, key, value, c); // 什么都没有还要改，只能增加了Orz
    now++;
    return;
}

int main()
{
    make_kv();
    put_kv("k", "v");
    print_kv();
}