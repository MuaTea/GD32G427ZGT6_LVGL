/*
* Copyright 2024 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/
#include "lvgl/lvgl.h"
#include <stdio.h>
#include <stdlib.h>

#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"

typedef struct {
    lv_obj_t * label;
    lv_obj_t * bar;
    lv_obj_t *slider;
    lv_obj_t *gif;
} ui_objects_t;


#define TEXT_COUNT (sizeof(texts) / sizeof(texts[0]))

static int current_text_index = 0;
static bool is_interactive_mode = false;

static const char * texts[] = {
    "I met a Traveller from\nan antique land ..",
    "Who said-Two vast\nand trunkless legs of\nstone\nStand in the desert....",
    "Near them,on the sand\nHalf sunk a shattered\nvisage lies,",
    "whose frown,\nAnd wrinkled lip, and\nsneer of cold command",
    "Tell that its sculptor\nwell those passions\nread which yet survive,",
    "stamped on these\nlifeless things,\nThe hand that mocked\nthem, \nand the heart that fed;",
    "And on the pedestal,\nthese words appear:",
    "My name is\n          Ozymandias\n        King of Kings!",
    "\nLook on my Works,\nye Mighty,\nand despair!",
    "\nNothing\nBeside\nRemains.",
    "\nRound the decayOf\nthat colossal Wreck,\nboundless and bare",
    "\nThe lone and level\nsands stretch\nfar away.",
};

int calculate_display_time(const char *text) {
    int length = strlen(text);
    int time_per_char = 50;
    return length * time_per_char;
}

static void slider_event_cb(lv_event_t *e) {
    lv_obj_t *slider = lv_event_get_target(e);
    ui_objects_t *objects = (ui_objects_t *)lv_event_get_user_data(e);

    if (!objects || !objects->label) return;

    // 获取滑动条值并更新文本
    int index = lv_slider_get_value(slider);
    lv_label_set_text(objects->label, texts[index]);
}


// 定时器回调函数，定时更改文本
static void update_span_text(lv_timer_t * timer) {
    // 从定时器的数据中获取 ui_objects_t 结构体
    ui_objects_t * objects = (ui_objects_t *)timer->user_data;

    if (!objects || !objects->label || (!objects->bar && !objects->slider))   return;

    // 更新 label 的文本
    lv_label_set_text(objects->label, texts[current_text_index]);

    //更新进度条的值
    int progress = (current_text_index * 100)/(TEXT_COUNT -1);  // 计算当前进度
    // 使用更长的动画时间使进度条变化更平滑
    lv_bar_set_value(objects->bar,progress,LV_ANIM_ON);  // 更新进度条的值
    lv_obj_set_style_anim_time(objects->bar,500,0);  // 设置动画时间为500ms，使得进度条变化更加平滑

    // 检查是否完成
    if (current_text_index == TEXT_COUNT - 1) {
        // 停止定时器
        lv_timer_del(timer);

        // lv_obj_clear_flag(objects->gif, LV_OBJ_FLAG_HIDDEN);

        // 启用交互模式
        if (!is_interactive_mode) {
            is_interactive_mode = true;

            //安全删除进度条
            if (objects->bar) {
                lv_obj_remove_event_cb(objects->bar,NULL);
                lv_obj_del(objects->bar);
                objects->bar = NULL;
            }

            // 创建滑动条
            lv_obj_t *slider = lv_slider_create(lv_obj_get_parent(objects->label));
            lv_obj_set_size(slider, 265, 20);
            lv_obj_set_pos(slider, 29, 205);
            lv_slider_set_range(slider, 0, TEXT_COUNT - 1);
            lv_slider_set_value(slider, current_text_index, LV_ANIM_OFF);
            lv_obj_set_style_anim_time(slider, 500, 0);

            // 添加滑动事件回调
            lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, objects);

            // 更新对象引用
            objects->slider = slider;
        }
        return; // 不再更新
    }

    // 更新索引，准备显示下一个文本段落
    current_text_index = (current_text_index + 1) % TEXT_COUNT;

    // 根据当前文本长度重新设置定时器时间
    int new_interval = calculate_display_time(texts[current_text_index]);
    lv_timer_set_period(timer,new_interval);
}

void Text_span(lv_ui *ui) {
    // 创建一个新的 label（标签）对象
    lv_obj_t * label = lv_label_create(ui->screen);
    lv_obj_set_pos(label, 18, 32);
    lv_obj_set_size(label, 290, 149);

    // 设置标签文本，包括换行符
    lv_label_set_text(label, "\tOzymandias\n\nPercy Bysshe Shelley\r");

    // 设置文本样式，您可以使用自定义样式或者保持默认样式
    lv_obj_set_style_text_color(label, lv_color_hex(0xed65b9), 0);
    lv_obj_set_style_text_font(label, &lv_font_PAPYRUS_30, 0);

    // 创建进度条
    lv_obj_t * bar = lv_bar_create(ui->screen);
    lv_obj_set_pos(bar,29,205);
    lv_obj_set_size(bar,265,20);
    lv_bar_set_mode(bar,LV_BAR_MODE_NORMAL);  // 进度条的范围是 0 到 100
    lv_bar_set_value(bar, 0, LV_ANIM_ON);  // 初始值为 0

    // 创建一个 ui_objects_t 结构体来保存 label 和 bar
    ui_objects_t * objects = malloc(sizeof(ui_objects_t));
    objects->label = label;
    objects->bar = bar;

    // 创建一个定时器，在 3000 毫秒（3 秒）后调用回调函数
    lv_timer_create(update_span_text, 3000, objects);
}

void setup_scr_screen(lv_ui *ui)
{
    //Write codes screen
    ui->screen = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen, 320, 480);
    lv_obj_set_scrollbar_mode(ui->screen, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen, 208, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_spangroup_1
    ui->screen_spangroup_1 = lv_spangroup_create(ui->screen);
    lv_spangroup_set_align(ui->screen_spangroup_1, LV_TEXT_ALIGN_LEFT);
    lv_spangroup_set_overflow(ui->screen_spangroup_1, LV_SPAN_OVERFLOW_CLIP);
    lv_spangroup_set_mode(ui->screen_spangroup_1, LV_SPAN_MODE_FIXED);
    //create span
    Text_span(ui);

    //Write style state: LV_STATE_DEFAULT for &style_screen_spangroup_1_main_main_default
    static lv_style_t style_screen_spangroup_1_main_main_default;
    ui_init_style(&style_screen_spangroup_1_main_main_default);

    lv_style_set_border_width(&style_screen_spangroup_1_main_main_default, 0);
    lv_style_set_radius(&style_screen_spangroup_1_main_main_default, 0);
    lv_style_set_bg_opa(&style_screen_spangroup_1_main_main_default, 0);
    lv_style_set_pad_top(&style_screen_spangroup_1_main_main_default, 0);
    lv_style_set_pad_right(&style_screen_spangroup_1_main_main_default, 0);
    lv_style_set_pad_bottom(&style_screen_spangroup_1_main_main_default, 0);
    lv_style_set_pad_left(&style_screen_spangroup_1_main_main_default, 0);
    lv_style_set_shadow_width(&style_screen_spangroup_1_main_main_default, 0);
    lv_obj_add_style(ui->screen_spangroup_1, &style_screen_spangroup_1_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_spangroup_refr_mode(ui->screen_spangroup_1);

    //Write codes screen_img_1
    ui->screen_img_1 = lv_img_create(ui->screen);
    lv_obj_add_flag(ui->screen_img_1, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_1, &alpha_320x480);
    lv_img_set_pivot(ui->screen_img_1, 50,50);
    lv_img_set_angle(ui->screen_img_1, 0);
    lv_obj_set_pos(ui->screen_img_1, 0, 0);
    lv_obj_set_size(ui->screen_img_1, 320, 480);

    //Write style for screen_img_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_1, 93, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_1, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Display gif
    lv_obj_t *gif = lv_gif_create(ui->screen);
    lv_gif_set_src(gif,&raw_gif);
    lv_obj_align(gif, LV_ALIGN_BOTTOM_MID, 0, 0);  // 底部显示 GIF
    lv_obj_move_foreground(gif);  // 确保 GIF 位于最上层
    // lv_obj_add_flag(gif,LV_OBJ_FLAG_HIDDEN);

    //The custom code of screen.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen);

    //Init events for screen.
    events_init_screen(ui);
}