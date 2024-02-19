#include "utility.h"

#include "building/building.h"
#include "building/roadblock.h"
#include "city/finance.h"
#include "core/image.h"
#include "graphics/generic_button.h"
#include "graphics/image.h"
#include "graphics/lang_text.h"
#include "graphics/panel.h"
#include "graphics/text.h"
#include "graphics/window.h"
#include "map/water_supply.h"
#include "translation/translation.h"
#include "window/building_info.h"
#include "window/building/figures.h"

static void go_to_orders(int param1, int param2);
static void toggle_figure_state(int index, int param2);
static void roadblock_orders(int index, int param2);



static struct {
    int focus_button_id;
    int orders_focus_button_id;
    int figure_focus_button_id;
    int building_id;
    int tooltip_id;
} data = { 0, 0, 0, 0, 0 };



static generic_button go_to_orders_button[] = {
    {0, 0, 304, 20, go_to_orders, button_none, 0, 0},
};


static generic_button orders_permission_buttons[] = {
    {0, 4, 210, 22, toggle_figure_state, button_none, PERMISSION_MAINTENANCE, 0},
    {0, 36, 210, 22, toggle_figure_state, button_none, PERMISSION_PRIEST, 0},
    {0, 68, 210, 22, toggle_figure_state, button_none, PERMISSION_MARKET, 0},
    {0, 100, 210, 22, toggle_figure_state, button_none, PERMISSION_ENTERTAINER, 0},
    {0, 132, 210, 22, toggle_figure_state, button_none, PERMISSION_EDUCATION, 0},
    {0, 164, 210, 22, toggle_figure_state, button_none, PERMISSION_MEDICINE, 0},
    {0, 192, 210, 22, toggle_figure_state, button_none, PERMISSION_TAX_COLLECTOR, 0},
    {0, 224, 210, 22, toggle_figure_state, button_none, PERMISSION_LABOR_SEEKER, 0},
    {0, 256, 210, 22, toggle_figure_state, button_none, PERMISSION_MISSIONARY, 0},
    {0, 288, 210, 22, toggle_figure_state, button_none, PERMISSION_WATCHMAN, 0},
};

static int permission_tooltip_translations[] = { 0,
TR_TOOLTIP_BUTTON_ROADBLOCK_PERMISSION_MAINTENANCE,TR_TOOLTIP_BUTTON_ROADBLOCK_PERMISSION_PRIEST,
TR_TOOLTIP_BUTTON_ROADBLOCK_PERMISSION_MARKET, TR_TOOLTIP_BUTTON_ROADBLOCK_PERMISSION_ENTERTAINER,
TR_TOOLTIP_BUTTON_ROADBLOCK_PERMISSION_EDUCATION, TR_TOOLTIP_BUTTON_ROADBLOCK_PERMISSION_MEDICINE,
TR_TOOLTIP_BUTTON_ROADBLOCK_PERMISSION_TAX_COLLECTOR, TR_TOOLTIP_BUTTON_ROADBLOCK_PERMISSION_LABOR_SEEKER,
TR_TOOLTIP_BUTTON_ROADBLOCK_PERMISSION_MISSIONARY, TR_TOOLTIP_BUTTON_ROADBLOCK_PERMISSION_WATCHMAN,
};

static int size_of_orders_permission_buttons = sizeof(orders_permission_buttons) / sizeof(*orders_permission_buttons);

static int permission_orders_tooltip_translations[] = { 0,
TR_TOOLTIP_BUTTON_ROADBLOCK_ORDER_ACCEPT_ALL,TR_TOOLTIP_BUTTON_ROADBLOCK_ORDER_REJECT_ALL };

static generic_button roadblock_orders_buttons[] = {
    {286, 0, 20, 20, roadblock_orders, button_none, 0, 0 },
    {309, 0, 20, 20, roadblock_orders, button_none, 1, 0 },
};



void window_building_draw_engineers_post(building_info_context *c)
{
    c->help_id = 81;
    window_building_play_sound(c, "wavs/eng_post.wav");
    outer_panel_draw(c->x_offset, c->y_offset, c->width_blocks, c->height_blocks);
    lang_text_draw_centered(104, 0, c->x_offset, c->y_offset + 10, BLOCK_SIZE * c->width_blocks, FONT_LARGE_BLACK);

    building *b = building_get(c->building_id);

    if (!c->has_road_access) {
        window_building_draw_description(c, 69, 25);
    } else if (!b->num_workers) {
        window_building_draw_description(c, 104, 9);
    } else {
        if (b->figure_id) {
            window_building_draw_description(c, 104, 2);
        } else {
            window_building_draw_description(c, 104, 3);
        }
        if (c->worker_percentage >= 100) {
            window_building_draw_description_at(c, 72, 104, 4);
        } else if (c->worker_percentage >= 75) {
            window_building_draw_description_at(c, 72, 104, 5);
        } else if (c->worker_percentage >= 50) {
            window_building_draw_description_at(c, 72, 104, 6);
        } else if (c->worker_percentage >= 25) {
            window_building_draw_description_at(c, 72, 104, 7);
        } else {
            window_building_draw_description_at(c, 72, 104, 8);
        }
    }

    inner_panel_draw(c->x_offset + 16, c->y_offset + 136, c->width_blocks - 2, 4);
    window_building_draw_employment(c, 142);
    window_building_draw_risks(c, c->x_offset + c->width_blocks * BLOCK_SIZE - 76, c->y_offset + 144);
}

void window_building_draw_prefect(building_info_context *c)
{
    c->help_id = 86;
    window_building_play_sound(c, "wavs/prefecture.wav");
    outer_panel_draw(c->x_offset, c->y_offset, c->width_blocks, c->height_blocks);
    lang_text_draw_centered(88, 0, c->x_offset, c->y_offset + 10, BLOCK_SIZE * c->width_blocks, FONT_LARGE_BLACK);

    building *b = building_get(c->building_id);
    if (!c->has_road_access) {
        window_building_draw_description(c, 69, 25);
    } else if (b->num_workers <= 0) {
        window_building_draw_description(c, 88, 9);
    } else {
        if (b->figure_id) {
            window_building_draw_description(c, 88, 2);
        } else {
            window_building_draw_description(c, 88, 3);
        }
        if (c->worker_percentage >= 100) {
            window_building_draw_description_at(c, 72, 88, 4);
        } else if (c->worker_percentage >= 75) {
            window_building_draw_description_at(c, 72, 88, 5);
        } else if (c->worker_percentage >= 50) {
            window_building_draw_description_at(c, 72, 88, 6);
        } else if (c->worker_percentage >= 25) {
            window_building_draw_description_at(c, 72, 88, 7);
        } else {
            window_building_draw_description_at(c, 72, 88, 8);
        }
    }

    inner_panel_draw(c->x_offset + 16, c->y_offset + 136, c->width_blocks - 2, 4);
    window_building_draw_employment(c, 142);
    window_building_draw_risks(c, c->x_offset + c->width_blocks * BLOCK_SIZE - 76, c->y_offset + 144);
}

static void draw_roadblock_orders_buttons(int x, int y, int focused)
{
    uint8_t refuse_all_button_text[] = { 'x', 0 };
    uint8_t accept_all_button_text[] = { 'v', 0 };
    button_border_draw(x, y, 20, 20, data.orders_focus_button_id == 1);
    text_draw_centered(accept_all_button_text, x + 1, y + 4, 20, FONT_NORMAL_BLACK, 0);
    button_border_draw(x+25, y, 20, 20, data.orders_focus_button_id == 2);
    text_draw_centered(refuse_all_button_text, x + 26, y + 4, 20, FONT_NORMAL_BLACK, 0);

}


void window_building_draw_roadblock(building_info_context *c)
{
    c->help_id = 0;
    window_building_play_sound(c, "wavs/prefecture.wav");
    outer_panel_draw(c->x_offset, c->y_offset, c->width_blocks, c->height_blocks);
    lang_text_draw_centered(28, 115, c->x_offset, c->y_offset + 10, 16 * c->width_blocks, FONT_LARGE_BLACK);
    window_building_draw_description_at(c, 96, CUSTOM_TRANSLATION, TR_BUILDING_ROADBLOCK_DESC);
}

void window_building_draw_roadblock_button(building_info_context *c)
{
    button_border_draw(c->x_offset + 80, c->y_offset + BLOCK_SIZE * c->height_blocks - 34,
        BLOCK_SIZE * (c->width_blocks - 10), 20, data.focus_button_id == 1 ? 1 : 0);
    lang_text_draw_centered(98, 5, c->x_offset + 80, c->y_offset + BLOCK_SIZE * c->height_blocks - 30,
        BLOCK_SIZE * (c->width_blocks - 10), FONT_NORMAL_BLACK);
}

void window_building_draw_roadblock_orders(building_info_context *c)
{
    c->help_id = 3;
    int y_offset = window_building_get_vertical_offset(c, 28);
    outer_panel_draw(c->x_offset, y_offset, 29, 28);
    text_draw_centered(translation_for(TR_BUILDING_ROADBLOCK), c->x_offset, y_offset + 10, 16 * c->width_blocks, FONT_LARGE_BLACK, 0);
    inner_panel_draw(c->x_offset + 16, y_offset + 42, c->width_blocks - 2, 21);
}

void window_building_draw_roadblock_orders_foreground(building_info_context *c)
{
    int y_offset = window_building_get_vertical_offset(c, 28);
    int ids[] = { GROUP_FIGURE_ENGINEER,GROUP_FIGURE_PREFECT,GROUP_FIGURE_PRIEST,GROUP_FIGURE_PRIEST,
        GROUP_FIGURE_MARKET_LADY,GROUP_FIGURE_MARKET_LADY,GROUP_FIGURE_ACTOR,GROUP_FIGURE_LION_TAMER,
        GROUP_FIGURE_TEACHER_LIBRARIAN, GROUP_FIGURE_SCHOOL_CHILD, GROUP_FIGURE_DOCTOR_SURGEON, GROUP_FIGURE_BATHHOUSE_WORKER,
        GROUP_FIGURE_TAX_COLLECTOR, GROUP_FIGURE_TAX_COLLECTOR, GROUP_FIGURE_LABOR_SEEKER, GROUP_FIGURE_LABOR_SEEKER,
        GROUP_FIGURE_MISSIONARY, GROUP_FIGURE_MISSIONARY, GROUP_FIGURE_TOWER_SENTRY, GROUP_FIGURE_TOWER_SENTRY,
    };
    building *b = building_get(c->building_id);
    data.building_id = b->id;
    draw_roadblock_orders_buttons(c->x_offset + 365, y_offset + 404, data.orders_focus_button_id == 1);

    for (int i = 0; i < size_of_orders_permission_buttons; i++) {
        image_draw(image_group(ids[i * 2]) + 4, c->x_offset + 32, y_offset + 46 + 32 * i, COLOR_MASK_NONE, SCALE_NONE);
        image_draw(image_group(ids[i * 2 + 1]) + 4, c->x_offset + 64, y_offset + 46 + 32 * i,
            COLOR_MASK_NONE, SCALE_NONE);
        button_border_draw(c->x_offset + 180, y_offset + 50 + 32 * i, 210, 22, data.figure_focus_button_id == i + 1);
        int state = building_roadblock_get_permission(i + 1, b);
        if (state) {
            lang_text_draw_centered(99, 7, c->x_offset + 180, y_offset + 55 + 32 * i, 210, FONT_NORMAL_WHITE);
        } else {
            lang_text_draw_centered(99, 8, c->x_offset + 180, y_offset + 55 + 32 * i, 210, FONT_NORMAL_RED);
        }
    }
}

void window_building_roadblock_get_tooltip_walker_permissions(int *translation)
{
    if (data.figure_focus_button_id) {
        *translation = permission_tooltip_translations[data.figure_focus_button_id];
    } else if (data.orders_focus_button_id) {
        *translation = permission_orders_tooltip_translations[data.orders_focus_button_id];
    } else {
        *translation = 0;
    }
}

void window_building_draw_garden_gate(building_info_context *c)
{
    c->help_id = 0;
    window_building_play_sound(c, "wavs/garden.wav");
    outer_panel_draw(c->x_offset, c->y_offset, c->width_blocks, c->height_blocks);
    text_draw_centered(translation_for(TR_BUILDING_GARDEN_WALL_GATE), c->x_offset, c->y_offset + 10, 16 * c->width_blocks, FONT_LARGE_BLACK, 0);
    window_building_draw_description_at(c, 96, CUSTOM_TRANSLATION, TR_BUILDING_GARDEN_WALL_GATE_DESC);
}

void window_building_draw_palisade_gate(building_info_context *c)
{
    c->help_id = 0;
    window_building_play_sound(c, "wavs/gatehouse.wav");
    outer_panel_draw(c->x_offset, c->y_offset, c->width_blocks, c->height_blocks);
    text_draw_centered(translation_for(TR_BUILDING_PALISADE_GATE), c->x_offset, c->y_offset + 10, 16 * c->width_blocks, FONT_LARGE_BLACK, 0);
    window_building_draw_description_at(c, 96, CUSTOM_TRANSLATION, TR_BUILDING_PALISADE_GATE_DESC);
}

void window_building_draw_burning_ruin(building_info_context *c)
{
    c->help_id = 0;
    window_building_play_sound(c, "wavs/ruin.wav");
    outer_panel_draw(c->x_offset, c->y_offset, c->width_blocks, c->height_blocks);
    lang_text_draw_centered(111, 0, c->x_offset, c->y_offset + 10, BLOCK_SIZE * c->width_blocks, FONT_LARGE_BLACK);

    lang_text_draw(41, c->rubble_building_type,
        c->x_offset + 32, c->y_offset + BLOCK_SIZE * c->height_blocks - 173, FONT_NORMAL_BLACK);
    lang_text_draw_multiline(111, 1, c->x_offset + 32, c->y_offset + BLOCK_SIZE * c->height_blocks - 143,
        BLOCK_SIZE * (c->width_blocks - 4), FONT_NORMAL_BLACK);
}

void window_building_draw_rubble(building_info_context *c)
{
    c->help_id = 0;
    window_building_play_sound(c, "wavs/ruin.wav");
    outer_panel_draw(c->x_offset, c->y_offset, c->width_blocks, c->height_blocks);
    lang_text_draw_centered(140, 0, c->x_offset, c->y_offset + 10, BLOCK_SIZE * c->width_blocks, FONT_LARGE_BLACK);

    lang_text_draw(41, c->rubble_building_type,
        c->x_offset + 32, c->y_offset + BLOCK_SIZE * c->height_blocks - 173, FONT_NORMAL_BLACK);
    lang_text_draw_multiline(140, 1, c->x_offset + 32, c->y_offset + BLOCK_SIZE * c->height_blocks - 143,
        BLOCK_SIZE * (c->width_blocks - 4), FONT_NORMAL_BLACK);
}

void window_building_draw_reservoir(building_info_context *c)
{
    c->help_id = 59;
    window_building_play_sound(c, "wavs/resevoir.wav");
    outer_panel_draw(c->x_offset, c->y_offset, c->width_blocks, c->height_blocks);
    lang_text_draw_centered(107, 0, c->x_offset, c->y_offset + 10, BLOCK_SIZE * c->width_blocks, FONT_LARGE_BLACK);
    int text_id = building_get(c->building_id)->has_water_access ? 1 : 3;
    window_building_draw_description_at(c, BLOCK_SIZE * c->height_blocks - 173, 107, text_id);
}

void window_building_draw_aqueduct(building_info_context *c)
{
    c->help_id = 60;
    window_building_play_sound(c, "wavs/aquaduct.wav");
    outer_panel_draw(c->x_offset, c->y_offset, c->width_blocks, c->height_blocks);
    lang_text_draw_centered(141, 0, c->x_offset, c->y_offset + 10, BLOCK_SIZE * c->width_blocks, FONT_LARGE_BLACK);
    window_building_draw_description_at(c, BLOCK_SIZE * c->height_blocks - 144, 141, c->aqueduct_has_water ? 1 : 2);
}

void window_building_draw_fountain(building_info_context *c)
{
    c->help_id = 61;
    window_building_play_sound(c, "wavs/fountain.wav");
    outer_panel_draw(c->x_offset, c->y_offset, c->width_blocks, c->height_blocks);
    lang_text_draw_centered(108, 0, c->x_offset, c->y_offset + 10, BLOCK_SIZE * c->width_blocks, FONT_LARGE_BLACK);
    int text_id;
    building *b = building_get(c->building_id);
    if (b->has_water_access) {
        if (b->num_workers > 0) {
            text_id = 1;
        } else {
            text_id = 2;
        }
    } else if (c->has_reservoir_pipes) {
        text_id = 2;
    } else {
        text_id = 3;
    }
    window_building_draw_description(c, 108, text_id);
    inner_panel_draw(c->x_offset + 16, c->y_offset + 166, c->width_blocks - 2, 4);
    window_building_draw_employment_without_house_cover(c, 172);
}

void window_building_draw_well(building_info_context *c)
{
    c->help_id = 62;
    window_building_play_sound(c, "wavs/well.wav");
    outer_panel_draw(c->x_offset, c->y_offset, c->width_blocks, c->height_blocks);
    lang_text_draw_centered(109, 0, c->x_offset, c->y_offset + 10, BLOCK_SIZE * c->width_blocks, FONT_LARGE_BLACK);
    int well_necessity = map_water_supply_is_well_unnecessary(c->building_id, 2);
    int text_id = 0;
    if (well_necessity == WELL_NECESSARY) { // well is OK
        text_id = 1;
    } else if (well_necessity == WELL_UNNECESSARY_FOUNTAIN) { // all houses have fountain
        text_id = 2;
    } else if (well_necessity == WELL_UNNECESSARY_NO_HOUSES) { // no houses around
        text_id = 3;
    }
    if (text_id) {
        window_building_draw_description_at(c, BLOCK_SIZE * c->height_blocks - 160, 109, text_id);
    }
}

void window_building_draw_mission_post(building_info_context *c)
{
    c->help_id = 8;
    window_building_play_sound(c, "wavs/mission.wav");
    outer_panel_draw(c->x_offset, c->y_offset, c->width_blocks, c->height_blocks);
    lang_text_draw_centered(134, 0, c->x_offset, c->y_offset + 10, BLOCK_SIZE * c->width_blocks, FONT_LARGE_BLACK);
    window_building_draw_description(c, 134, 1);
    inner_panel_draw(c->x_offset + 16, c->y_offset + 136, c->width_blocks - 2, 4);
    window_building_draw_employment_without_house_cover(c, 142);
    window_building_draw_risks(c, c->x_offset + c->width_blocks * BLOCK_SIZE - 76, c->y_offset + 144);
}

static void draw_native(building_info_context *c, int group_id)
{
    c->help_id = 0;
    window_building_play_sound(c, "wavs/empty_land.wav");
    outer_panel_draw(c->x_offset, c->y_offset, c->width_blocks, c->height_blocks);
    lang_text_draw_centered(group_id, 0, c->x_offset, c->y_offset + 10,
        BLOCK_SIZE * c->width_blocks, FONT_LARGE_BLACK);
    window_building_draw_description_at(c, 106, group_id, 1);
}

void window_building_draw_native_hut(building_info_context *c)
{
    draw_native(c, 131);
}

void window_building_draw_native_meeting(building_info_context *c)
{
    draw_native(c, 132);
}

void window_building_draw_native_crops(building_info_context *c)
{
    draw_native(c, 133);
}

void window_building_draw_highway(building_info_context *c)
{
    //c->help_id = 0;
    //window_building_play_sound(c, "wavs/aquaduct.wav");
    window_building_prepare_figure_list(c);
    outer_panel_draw(c->x_offset, c->y_offset, c->width_blocks, c->height_blocks);
    lang_text_draw_centered(CUSTOM_TRANSLATION, TR_BUILDING_HIGHWAY, c->x_offset, c->y_offset + 10, BLOCK_SIZE * c->width_blocks, FONT_LARGE_BLACK);
    window_building_draw_figure_list(c);
    window_building_draw_description_at(c, BLOCK_SIZE * c->height_blocks - 150, CUSTOM_TRANSLATION, TR_BUILDING_HIGHWAY_DESC);
    window_building_draw_levy(HIGHWAY_LEVY_MONTHLY, c->x_offset + 30, c->y_offset + BLOCK_SIZE * c->height_blocks - 110);
}

static void toggle_figure_state(int index, int param2)
{
    building *b = building_get(data.building_id);
    if (building_type_is_roadblock(b->type)) {
        building_roadblock_set_permission(index, b);
    }
    window_invalidate();
}



static void roadblock_orders(int index, int param2)
{
    building *b = building_get(data.building_id);
    if (index == 0) {
        building_roadblock_accept_all(b);
    } else if (index == 1) {
        building_roadblock_accept_none(b);
    }
    window_invalidate();

}

static void go_to_orders(int param1, int param2)
{
    window_building_info_show_storage_orders();
}

int window_building_handle_mouse_roadblock_button(const mouse *m, building_info_context *c)
{
    return generic_buttons_handle_mouse(
        m, c->x_offset + 80, c->y_offset + BLOCK_SIZE * c->height_blocks - 34,
        go_to_orders_button, 1, &data.focus_button_id);
}

int window_building_handle_mouse_roadblock_orders(const mouse *m, building_info_context *c)
{
    int y_offset = window_building_get_vertical_offset(c, 28);

    data.building_id = c->building_id;
    if (generic_buttons_handle_mouse(m, c->x_offset + 180, y_offset + 46,
        orders_permission_buttons, size_of_orders_permission_buttons,
        &data.figure_focus_button_id)) {
        return 1;
    }

    return generic_buttons_handle_mouse(m, c->x_offset + 80, y_offset + 404, roadblock_orders_buttons, 2, &data.orders_focus_button_id);
}
