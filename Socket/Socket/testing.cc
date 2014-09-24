#include <stdio.h>

int main()
{
    FILE* fp = fopen("index.html", "r");

    if(fp == NULL)
    {
        printf("FUCK");
        return -1;
    }
    else
    {
        printf("YES");
        return 0;
    }



}