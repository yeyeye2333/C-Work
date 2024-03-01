//
//  Common.c
//  _TTMS_
//
//  Created by GA KE on 2024/2/28.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Common.h"

void readInt(int *p) {
    // 1. 读取int
    scanf("%d", p);
    // 2. 读取\n
    getchar();
}

void readChar(char *pc) {
    // 1. 读取字符
    scanf("%c", pc);
    // 2. 读取\n
    getchar();
}

void readStr(char str[], int len) {
    // 1. 读取输入
    fgets(str, len, stdin);
    // 2. 删除换行符
    str[strlen(str) - 1] = '\0';
}

