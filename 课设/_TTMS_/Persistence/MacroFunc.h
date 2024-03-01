//
//  Persist_Macro.h
//  _TTMS_
//
//  Created by GA KE on 2024/2/28.
//

#ifndef Persist_Macro_h
#define Persist_Macro_h

// 为type_t类型的data使用KEY_NAME分配一个主键ID，并将其写入到DATA_FILE文件
// 返回是否添加成功
#define INSERT(data, KEY_NAME, DATA_FILE, type_t) {             \
            assert(NULL != data);                               \
            long key = EntKey_Perst_GetNewKeys(KEY_NAME, 1);    \
            if (key <= 0) {                                     \
                return 0;                                       \
            }                                                   \
            data->id = (int) key;                               \
            FILE *fp = fopen(DATA_FILE, "ab");                  \
            int rtn = 0;                                        \
            if (NULL == fp) {                                   \
                printf("Cannot open file %s!\n", DATA_FILE);    \
                return 0;                                       \
            }                                                   \
            rtn = (int) fwrite(data, sizeof(type_t), 1, fp);    \
            fclose(fp);                                         \
            return rtn;                                         \
        }

// 将主键ID为id且类型为type_t的对象从DATA_FILE文件中删除，过程中需要使用DATA_TEMP_FILE临时文件
// 返回是否找到待删除对象
#define DELETE_BY_ID(id, DATA_FILE, DATA_TEMP_FILE, type_t) {       \
            if (rename(DATA_FILE, DATA_TEMP_FILE) < 0) {            \
                printf("Cannot open file %s!\n", DATA_FILE);        \
                return 0;                                           \
            }                                                       \
            FILE *fpSour, *fpTarg;                                  \
            fpSour = fopen(DATA_TEMP_FILE, "rb");                   \
            if (NULL == fpSour) {                                   \
                printf("Cannot open file %s!\n", DATA_FILE);        \
                return 0;                                           \
            }                                                       \
            fpTarg = fopen(DATA_FILE, "wb");                        \
            if (NULL == fpTarg) {                                   \
                printf("Cannot open file %s!\n", DATA_TEMP_FILE);   \
                return 0;                                           \
            }                                                       \
            type_t buf;                                             \
            int found = 0;                                          \
            while (!feof(fpSour)) {                                 \
                if (fread(&buf, sizeof(type_t), 1, fpSour)) {       \
                    if (id == buf.id) {                             \
                        found = 1;                                  \
                        continue;                                   \
                    }                                               \
                    fwrite(&buf, sizeof(type_t), 1, fpTarg);        \
                }                                                   \
            }                                                       \
            fclose(fpTarg);                                         \
            fclose(fpSour);                                         \
            remove(DATA_TEMP_FILE);                                 \
            return found;                                           \
        }

// 更新type_t类型的data对象在DATA_FILE文件中的数据
// 返回是否找到待更新对象
#define UPDATE(data, DATA_FILE, type_t) {                               \
            assert(NULL != data);                                       \
            FILE *fp = fopen(DATA_FILE, "rb+");                         \
            if (NULL == fp) {                                           \
                printf("Cannot open file %s!\n", DATA_FILE);            \
                return 0;                                               \
            }                                                           \
            type_t buf;                                                 \
            int found = 0;                                              \
            while (!feof(fp)) {                                         \
                if (fread(&buf, sizeof(type_t), 1, fp)) {               \
                    if (buf.id == data->id) {                           \
                        fseek(fp, -((int)sizeof(type_t)), SEEK_CUR);    \
                        fwrite(data, sizeof(type_t), 1, fp);            \
                        found = 1;                                      \
                        break;                                          \
                    }                                                   \
                }                                                       \
            }                                                           \
            fclose(fp);                                                 \
            return found;                                               \
        }

// 在DATA_FILE文件中查找主键ID为id，类型为type_t的对象，并将其放入buf中
// 返回是否找到待查找对象
#define SELECT_BY_ID(id, buf, DATA_FILE, type_t) {          \
            assert(NULL != buf);                            \
            FILE *fp = fopen(DATA_FILE, "rb");              \
            if (NULL == fp) {                               \
                return 0;                                   \
            }                                               \
            type_t data;                                    \
            int found = 0;                                  \
            while (!feof(fp)) {                             \
                if (fread(&data, sizeof(type_t), 1, fp)) {  \
                    if (id == data.id) {                    \
                        *buf = data;                        \
                        found = 1;                          \
                        break;                              \
                    };                                      \
                }                                           \
            }                                               \
            fclose(fp);                                     \
            return found;                                   \
        }

// 在DATA_FILE文件中查找类型为type_t的对象，并将其加入list中，其中list的节点类型为node_t
// 返回是查找到的对象数量
#define SELECT_ALL(list, DATA_FILE, node_t, type_t) {                       \
            assert(NULL != list);                                           \
            node_t *newNode;                                                \
            type_t data;                                                    \
            List_Free(list, node_t);                                        \
            FILE *fp = fopen(DATA_FILE, "rb");                              \
            if (NULL == fp) {                                               \
                return 0;                                                   \
            }                                                               \
            int recCount = 0;                                               \
            while (!feof(fp)) {                                             \
                if (fread(&data, sizeof(type_t), 1, fp)) {                  \
                    newNode = (node_t*) malloc(sizeof(node_t));        \
                    if (!newNode) {                                         \
                        printf("Warning, Memory OverFlow!!!\n");            \
                        printf("Cannot Load more Data into memory!!!\n");   \
                        break;                                              \
                    }                                                       \
                    newNode->data = data;                                   \
                    List_AddTail(list, newNode);                            \
                    recCount++;                                             \
                }                                                           \
            }                                                               \
            fclose(fp);                                                     \
            return recCount;                                                \
        }

#endif /* Persist_Macro_h */
