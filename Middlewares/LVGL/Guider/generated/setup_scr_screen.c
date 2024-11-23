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

    // ��ȡ������ֵ�������ı�
    int index = lv_slider_get_value(slider);
    lv_label_set_text(objects->label, texts[index]);
}


// ��ʱ���ص���������ʱ�����ı�
static void update_span_text(lv_timer_t * timer) {
    // �Ӷ�ʱ���������л�ȡ ui_objects_t �ṹ��
    ui_objects_t * objects = (ui_objects_t *)timer->user_data;

    if (!objects || !objects->label || (!objects->bar && !objects->slider))   return;

    // ���� label ���ı�
    lv_label_set_text(objects->label, texts[current_text_index]);

    //���½�������ֵ
    int progress = (current_text_index * 100)/(TEXT_COUNT -1);  // ���㵱ǰ����
    // ʹ�ø����Ķ���ʱ��ʹ�������仯��ƽ��
    lv_bar_set_value(objects->bar,progress,LV_ANIM_ON);  // ���½�������ֵ
    lv_obj_set_style_anim_time(objects->bar,500,0);  // ���ö���ʱ��Ϊ500ms��ʹ�ý������仯����ƽ��

    // ����Ƿ����
    if (current_text_index == TEXT_COUNT - 1) {
        // ֹͣ��ʱ��
        lv_timer_del(timer);

        // lv_obj_clear_flag(objects->gif, LV_OBJ_FLAG_HIDDEN);

        // ���ý���ģʽ
        if (!is_interactive_mode) {
            is_interactive_mode = true;

            //��ȫɾ��������
            if (objects->bar) {
                lv_obj_remove_event_cb(objects->bar,NULL);
                lv_obj_del(objects->bar);
                objects->bar = NULL;
            }

            // ����������
            lv_obj_t *slider = lv_slider_create(lv_obj_get_parent(objects->label));
            lv_obj_set_size(slider, 265, 20);
            lv_obj_set_pos(slider, 29, 205);
            lv_slider_set_range(slider, 0, TEXT_COUNT - 1);
            lv_slider_set_value(slider, current_text_index, LV_ANIM_OFF);
            lv_obj_set_style_anim_time(slider, 500, 0);

            // ��ӻ����¼��ص�
            lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, objects);

            // ���¶�������
            objects->slider = slider;
        }
        return; // ���ٸ���
    }

    // ����������׼����ʾ��һ���ı�����
    current_text_index = (current_text_index + 1) % TEXT_COUNT;

    // ���ݵ�ǰ�ı������������ö�ʱ��ʱ��
    int new_interval = calculate_display_time(texts[current_text_index]);
    lv_timer_set_period(timer,new_interval);
}

void Text_span(lv_ui *ui) {
    // ����һ���µ� label����ǩ������
    lv_obj_t * label = lv_label_create(ui->screen);
    lv_obj_set_pos(label, 18, 32);
    lv_obj_set_size(label, 290, 149);

    // ���ñ�ǩ�ı����������з�
    lv_label_set_text(label, "\tOzymandias\n\nPercy Bysshe Shelley\r");

    // �����ı���ʽ��������ʹ���Զ�����ʽ���߱���Ĭ����ʽ
    lv_obj_set_style_text_color(label, lv_color_hex(0xed65b9), 0);
    lv_obj_set_style_text_font(label, &lv_font_PAPYRUS_30, 0);

    // ����������
    lv_obj_t * bar = lv_bar_create(ui->screen);
    lv_obj_set_pos(bar,29,205);
    lv_obj_set_size(bar,265,20);
    lv_bar_set_mode(bar,LV_BAR_MODE_NORMAL);  // �������ķ�Χ�� 0 �� 100
    lv_bar_set_value(bar, 0, LV_ANIM_ON);  // ��ʼֵΪ 0

    // ����һ�� ui_objects_t �ṹ�������� label �� bar
    ui_objects_t * objects = malloc(sizeof(ui_objects_t));
    objects->label = label;
    objects->bar = bar;

    // ����һ����ʱ������ 3000 ���루3 �룩����ûص�����
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
    lv_obj_align(gif, LV_ALIGN_BOTTOM_MID, 0, 0);  // �ײ���ʾ GIF
    lv_obj_move_foreground(gif);  // ȷ�� GIF λ�����ϲ�
    // lv_obj_add_flag(gif,LV_OBJ_FLAG_HIDDEN);

    //The custom code of screen.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen);

    //Init events for screen.
    events_init_screen(ui);
}