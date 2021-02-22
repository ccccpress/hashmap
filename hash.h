unsigned int hash(char *str, int max)
{
    unsigned long hash = 5381;
    int c;
    unsigned char *u_str = (unsigned char *)str;

    while ((c = *u_str++)) // 可能'\0' == false
    {
        hash = ((hash << 5) + hash) + c;
    }

    return hash % max;
}
