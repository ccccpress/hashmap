# USE

初始化
```
make_kv();
```

新增&更新数据
```
put_kv("k", "v");
```

查询一条数据
```
char *v = get_kv("k");
printf("[%s]\n", v);
```

删除一条数据
```
del_kv("k");
```

遍历打印输出
```
print_kv();
```