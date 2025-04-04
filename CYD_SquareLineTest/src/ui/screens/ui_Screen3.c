// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.2
// LVGL version: 9.1.0
// Project name: SquareLine_Project_Test2

#include "../ui.h"

void ui_Screen3_screen_init(void)
{
    ui_Screen3 = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_Screen3, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Dropdown1 = lv_dropdown_create(ui_Screen3);
    lv_dropdown_set_options(ui_Dropdown1, "Option 1\nOption 2\nOption 3");
    lv_obj_set_width(ui_Dropdown1, 150);
    lv_obj_set_height(ui_Dropdown1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Dropdown1, 74);
    lv_obj_set_y(ui_Dropdown1, 86);
    lv_obj_set_align(ui_Dropdown1, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Dropdown1, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags



    ui_TabView1 = lv_tabview_create(ui_Screen3);
    lv_tabview_set_tab_bar_size(ui_TabView1, 50);
    lv_obj_set_width(ui_TabView1, 304);
    lv_obj_set_height(ui_TabView1, 160);
    lv_obj_set_x(ui_TabView1, 1);
    lv_obj_set_y(ui_TabView1, -16);
    lv_obj_set_align(ui_TabView1, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_TabView1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_border_color(ui_TabView1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_TabView1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_color(ui_TabView1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(ui_TabView1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_border_color(lv_tabview_get_tab_bar(ui_TabView1), lv_color_hex(0x000000),
                                  LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(lv_tabview_get_tab_bar(ui_TabView1), 255,  LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_color(lv_tabview_get_tab_bar(ui_TabView1), lv_color_hex(0x000000),
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(lv_tabview_get_tab_bar(ui_TabView1), 255,  LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_border_color(lv_tabview_get_tab_bar(ui_TabView1), lv_color_hex(0x000000),
                                  LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(lv_tabview_get_tab_bar(ui_TabView1), 255,  LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_color(lv_tabview_get_tab_bar(ui_TabView1), lv_color_hex(0x000000),
                                   LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(lv_tabview_get_tab_bar(ui_TabView1), 255,  LV_PART_ITEMS | LV_STATE_DEFAULT);

    ui_TabPage1 = lv_tabview_add_tab(ui_TabView1, "Title 1a");

    ui_Button3 = lv_button_create(ui_TabPage1);
    lv_obj_set_width(ui_Button3, 100);
    lv_obj_set_height(ui_Button3, 50);
    lv_obj_set_x(ui_Button3, -5);
    lv_obj_set_y(ui_Button3, 106);
    lv_obj_set_align(ui_Button3, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Button3, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_remove_flag(ui_Button3, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_TabPage2 = lv_tabview_add_tab(ui_TabView1, "Title 2a");

    ui_Label7 = lv_label_create(ui_TabPage2);
    lv_obj_set_width(ui_Label7, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label7, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label7, -23);
    lv_obj_set_y(ui_Label7, -14);
    lv_obj_set_align(ui_Label7, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label7, "text");

    ui_TabPage3 = lv_tabview_add_tab(ui_TabView1, "Title 3");

    ui_TabPage4 = lv_tabview_add_tab(ui_TabView1, "Title 4");

    ui_TabPage5 = lv_tabview_add_tab(ui_TabView1, "Title 5");

    ui_TabPage6 = lv_tabview_add_tab(ui_TabView1, "Title 6");

    lv_obj_add_event_cb(ui_Screen3, ui_event_Screen3, LV_EVENT_ALL, NULL);

}
