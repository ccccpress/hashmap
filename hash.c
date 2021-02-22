#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
// kv_ 是保存数据的数组
// kv0 是保存数据的链表
// max 是数组的长度，也被当作NULL的一个枚举

int max = 8;

typedef struct kv
{
    char *key;
    char *value;
    struct kv *next;
} kv;

kv *empty = (kv *)&max; // 随便搞个变量当枚举类型了

kv *make_kv()
{
    kv *kv_ = (kv *)malloc(max * sizeof(kv));
    for (int i = 0; i < max; i++)
    {
        kv_[i].next = empty;
    }
    return kv_;
}

void add_kv(kv *kv_, char *key, char *value)
{
    int len1 = strlen(key) + 1;
    int len2 = strlen(value) + 1;
    unsigned int c = hash(key, max);

    kv *kv0 = (kv *)malloc(sizeof(kv));
    char *a = (char *)malloc(len1 * sizeof(char));
    char *b = (char *)malloc(len2 * sizeof(char));

    kv0->key = memcpy(a, key, len1);
    kv0->value = memcpy(b, value, len2);
    kv0->next = NULL;

    if (kv_[c].next == NULL)
    {
        kv_[c].next = kv0;
    }
    else if (kv_[c].next == empty)
    {
        kv_[c].key = kv0->key;
        kv_[c].value = kv0->value;
        kv_[c].next = NULL;
    }
    else
    {
        kv *temp = kv_[c].next;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = kv0;
    }
}
void del_kv(kv *kv_, char *key)
{
    unsigned int c = hash(key, max);
    if (*kv_[c].key == *key) // 在数组
    {
        if (kv_[c].next == NULL) // 后面没有了
        {
            kv_[c].next = empty;
            free(kv_[c].key);
            free(kv_[c].value);
            return;
        }
        else
        {
            free(kv_[c].key);
            free(kv_[c].value);
            kv_[c].key = kv_[c].next->key;
            kv_[c].value = kv_[c].next->value;
            kv_[c].next = kv_[c].next->next;
            free(kv_[c].next);
            return;
        }
    }

    kv *temp1 = kv_[c].next;
    if (*temp1->key == *key) // 在链表第一个
    {
        if (temp1->next == NULL)
        {
            kv_[c].next = NULL;
        }
        else
        {
            kv_[c].next = temp1->next;
        }
        free(temp1->key);
        free(temp1->value);
        free(temp1);
        return;
    }
    kv *temp2 = kv_[c].next->next; // 在链表第二个及以后
    do
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
        }
        temp1 = temp1->next;
        temp2 = temp2->next;
    } while (temp2->next != NULL);
}
void print_kv(kv *kv_)
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
char *get_kv(kv *kv_, char *key)
{
    unsigned int c = hash(key, max);
    if (*kv_[c].key == *key) // 在数组
    {
        return kv_[c].value;
    }

    kv *temp1 = kv_[c].next;
    if (*temp1->key == *key) // 在链表第一个
    {
        return temp1->value;
    }
    while (temp1 != NULL) // 在链表第二个及以后
    {
        if (*temp1->key == *key)
        {
            return temp1->value;
        }
        temp1 = temp1->next;
    }
    return NULL; // 这个地方可能有问题
}
void *put_kv(kv *kv_, char *key, char *value)
{
    int len2 = strlen(value) + 1;
    unsigned int c = hash(key, max);

    char *b = (char *)malloc(len2 * sizeof(char));

    if (*kv_[c].key == *key) // 在数组
    {
        free(kv_[c].value);
        kv_[c].value = memcpy(b, value, len2);
        return;
    }

    kv *temp1 = kv_[c].next;
    if (*temp1->key == *key) // 在链表第一个
    {
        free(temp1->value);
        temp1->value = memcpy(b, value, len2);
        return;
    }
    while (temp1 != NULL) // 在链表第二个及以后
    {
        if (*temp1->key == *key)
        {
            free(temp1->value);
            temp1->value = memcpy(b, value, len2);
            return;
        }
        temp1 = temp1->next;
    }
    add_kv(kv_, key, value); // 什么都没有还要改，只能增加了Orz
    return;
}
int main()
{
    kv *a = make_kv();
    add_kv(a, "key1", "5678");
    add_kv(a, "2", "56789");
    print_kv(a);
    char *b = get_kv(a, "2");
    printf("%s\n", b);
    char *c = get_kv(a, "key1");
    printf("%s\n", c);
    printf("---\n");
    del_kv(a, "2");
    print_kv(a);
}