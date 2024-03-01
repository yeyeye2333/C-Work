//
//  EntityKey_Persist.h
//  _TTMS_
//
//  Created by GA KE on 2024/2/27.
//

#ifndef EntityKey_Persist_h
#define EntityKey_Persist_h

/*
 标识符：SCU_EntKey_Perst_GetNewKeys
 功能：根据传入的实体名entName及实体个数count，为这个count个新实体分配一个长度为count的主键值区间，
     使得每个新实体在该区间内都可以分配到 唯一的 主键。返回值为该主键区间的最小值
 */
long EntKey_Perst_GetNewKeys(const char entName[], int count);

#endif /* EntityKey_Persist_h */
