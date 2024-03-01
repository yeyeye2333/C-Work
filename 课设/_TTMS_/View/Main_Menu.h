//
//  Main_Menu.h
//  _TTMS_
//
//  Created by GA KE on 2024/2/26.
//

#ifndef Main_Menu_h
#define Main_Menu_h

/*
 [S]ystem Manager <-> SM_Menu
 [S]tudio Management.
 [A]ccount Management.
 */
// 系统管理员的主菜单
void SM_Menu(void);

/*
 [P]lay Manager <-> PM_Menu
 Play [M]anagement.
 [A]rrange Perform.
 Query [T]icket.
 [S]ale Statistics.
 [B]usiness Statistics.
 */
// 剧目管理员的主菜单
void PM_Menu(void);

/*
 [T]icket Seller <-> TS_Menu
 Query [P]erform.
 [Q]uery Ticket.
 [S]ale Statistics.
 Sell [T]icket.
 */
// 售票员的主菜单
void TS_Menu(void);

/*
 标识符：SCU_MainMenu
 功能：主菜单函数
 */
void Main_Menu(void);

#endif /* Main_Menu_h */
