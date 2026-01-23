#include "zf_common_headfile.h"


int GPS_Flag_,TASK_1_FLAG,cb_flag;
// 鑿滃崟鍙橀噺澹版槑
MenuItem *mainMenu, *taskMenu, *PathMenu, *TargetMenu, *ParaMenu, *SpeedMenu;
MenuItem *task1_get,*task2_get,*task3_get,*task4_get,*task_clear;
MenuItem *GD_Path,*Key_R1,*Key_R2,*Key_R5,*Key_R6;
MenuItem *Task_1,*Task_2,*Task_3,*Task_4,*Task_STOP,*LAT_LON,*gl_crc,*GPS_TASK;
MenuItem *text;
MenuItem *createMenuItem(const char *name,MenuCallback cb) {
    MenuItem *item = (MenuItem *)malloc(sizeof(MenuItem));
    if (item == NULL) return NULL;
    strncpy(item->name, name, sizeof(item->name) - 1);
    item->name[sizeof(item->name) - 1] = '\0';
    item->callback = cb;
    item->parent = NULL;
    item->child = NULL;
    item->prev = NULL;
    item->next = NULL;
    return item;
}

void addChild(MenuItem *parent, MenuItem *child) {
    if (!parent || !child) return;

    child->parent = parent;
    if (!parent->child) {
        parent->child = child;
    } else {
        MenuItem *last = parent->child;
        while (last->next) last = last->next;
        last->next = child;
        child->prev = last;
    }
}


void MENU_Init(void) {
    // 鍒涘缓鑿滃崟椤�
    mainMenu = createMenuItem("main",NULL);
    taskMenu   = createMenuItem("task",NULL);
    SpeedMenu  = createMenuItem("SPEED",NULL);
    PathMenu   = createMenuItem("Path",NULL);
    TargetMenu = createMenuItem("Target",NULL);
    ParaMenu   = createMenuItem("para",para_disp);

    addChild(mainMenu, taskMenu);
    addChild(mainMenu, SpeedMenu);
    addChild(mainMenu, PathMenu);
    addChild(mainMenu, TargetMenu);
    addChild(mainMenu, ParaMenu);

//韪╃偣
    task1_get  = createMenuItem("TASK1_GET",TASK1_GET);
    task2_get  = createMenuItem("TASK2_GET",TASK2_GET);
    task3_get  = createMenuItem("TASK3_GET",TASK3_GET);
    task4_get  = createMenuItem("TASK4_GET",TASK4_GET);
    task_clear  = createMenuItem("TASK_CLEAR",TASK_CLEAR);
    gl_crc = createMenuItem("GL_CRC",GL_CRC);
    addChild(TargetMenu, task1_get );
    addChild(TargetMenu, task2_get );
    addChild(TargetMenu, task3_get );
    addChild(TargetMenu, task4_get );
    addChild(TargetMenu, task_clear );
    addChild(TargetMenu, gl_crc );

//浠诲姟
    Task_1 = createMenuItem("task1",start_11);
    Task_2 = createMenuItem("task2",start_12);
    Task_3 = createMenuItem("task3",start_13);
    Task_4 = createMenuItem("task4",NULL);
    GPS_TASK = createMenuItem("GPS_TASK",start_1);

    addChild(taskMenu, Task_1 );
    addChild(taskMenu, Task_2 );
    addChild(taskMenu, Task_3 );
    addChild(taskMenu, Task_4 );
    addChild(taskMenu, GPS_TASK );
//璋冨弬
    LAT_LON = createMenuItem("IMG_SHOW", start_img);
//    Key_R1 = createMenuItem("R1", Key_Change_R1);
//    Key_R2 = createMenuItem("R2", Key_Change_R2);
//    Key_R5 = createMenuItem("R5", Key_Change_R5);
//    Key_R6 = createMenuItem("R6", Key_Change_R6);
//
    addChild( SpeedMenu, LAT_LON);
//    addChild( SpeedMenu, Key_R1 );
//    addChild( SpeedMenu, Key_R2 );
//    addChild( SpeedMenu, Key_R5 );
//    addChild( SpeedMenu, Key_R6 );

//    Overall_Motor
//璺緞

}

// 鏄剧ず鑿滃崟
void displayMenu(MenuItem *menu)
{
    if (!menu) return;
    ips200_show_string(110, 16*0, menu->name);

    if (menu->child) {
        MenuItem *child = menu->child;
        int index = 1;
        while (child) {
            ips200_show_string(0, index * 16, child->name);
            child = child->next;
            index++;
        }
    }
}

int getChildCount(MenuItem *menu)
{
    if (!menu || !menu->child) return 0;
    int count = 0;
    MenuItem *child = menu->child;
    while (child) {
        count++;
        child = child->next;
    }
    return count;
}

MenuItem *getChildAtIndex(MenuItem *menu, int index)
{
    if (!menu || !menu->child) return NULL;
    MenuItem *child = menu->child;
    int count = 0;
    while (child && count < index) {
        child = child->next;
        count++;
    }
    return child;
}



// 瀵艰埅鍑芥暟
void navigateMenu(void)
{
    static MenuItem *current_menu = NULL;
    static int current_index = 0;
    if (!current_menu) {
        current_menu = mainMenu;
        current_index = 0;
    }
    else {
        displayMenu(current_menu);
        ips200_show_string(120, (current_index+1)*16, "<-");
    }

    if(current_menu == ParaMenu)
    {
        para_disp();
    }
    // 鏄剧ず褰撳墠鑿滃崟
    if(!gpio_get_level(SWITCH1) && !gpio_get_level(SWITCH2))  //1.2閮藉湪涓婇潰
     {
        int choice = key_flag_get();
        switch(choice) {
            case 1: // 涓嬬Щ
                ips200_clear();
                current_index++;
                if (current_index >= getChildCount(current_menu))
                    current_index = 0;
                key1_flag = 0;
                break;

            case 2: // 涓婄Щ
                ips200_clear();
                if (current_index > 0) current_index--;
                else current_index = getChildCount(current_menu)-1;
                key2_flag = 0;
                break;

            case 3: { // 杩涘叆/鎵ц
                ips200_clear();
               MenuItem *selected = getChildAtIndex(current_menu, current_index);
                if (selected)
                {
                    if (selected->child) {
                      current_menu = selected;
                      current_index = 0;
                    }
                    if (selected->callback !=NULL)
                    {
                      selected->callback();
                    }
                 }
                key3_flag = 0;
                break;

            case 4: //杩斿洖
                ips200_clear();
                if (current_menu->parent) {
                    current_menu = current_menu->parent;
                    current_index = 0;
                }
                key4_flag = 0;
                break;
        }
      }
    }
}
