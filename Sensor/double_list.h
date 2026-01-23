#ifndef _DOUBLE_LIST_H
#define _DOUBLE_LIST_H
typedef void (*MenuCallback)(void);
typedef struct MenuItem {
    char name[50];
    MenuCallback callback;
    struct MenuItem* parent;  
    struct MenuItem* child;   
    struct MenuItem* prev;    
    struct MenuItem* next;    
} MenuItem;
extern int GPS_Flag_,TASK_1_FLAG,cb_flag;
void navigateMenu(void);

MenuItem *createMenuItem(const char *name,MenuCallback cb);
void addChild(MenuItem *parent, MenuItem *child);
void displayMenu(MenuItem *menu);
void MENU_Init(void);
#endif // _DOUBLE_LIST_H

