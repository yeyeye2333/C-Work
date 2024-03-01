//
//  Account.h
//  _TTMS_
//
//  Created by GA KE on 2024/3/1.
//

#ifndef Account_h
#define Account_h

typedef enum {
    USR_ANOMY = 0,  // 匿名账户，初始化账号时使用
    USR_CLERK = 1,  // 销售员类型
    USR_MANG = 2,   // 经理类型
    USR_ADMIN = 9   
} account_type_t;

typedef struct {
    int id;                 // 用户ID
    account_type_t type;    // 用户类型
    char *userName;         // 用户名
    char *password;         // 用户密码
} account_t;

typedef struct account_node {
    account_t data;             // 实体数据
    struct account_node *prev;  // 前向指针
    struct account_node *next;  // 后向指针
} account_node_t, *account_list_t; // 账户节点类型、账户链表类型

int Account_Srv_Add(account_t *data);

int Account_Src_DeleteByID(int id);

int Account_Srv_Modify(account_t *data);

int Account_Srv_FetchByID(int id, account_t *buf);

int Account_Srv_FetchAll(account_list_t list);

#endif /* Account_h */
