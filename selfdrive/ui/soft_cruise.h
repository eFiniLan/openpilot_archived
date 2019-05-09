//#include <stdio.h>

const int btn_size = 160;

int btn_x = 0;
int btn_set_y = 0;
int btn_res_y = 0;

bool is_init = false;
bool cruise_state = false;
int cruise_speed = 0;

/**
 * Draw buttons to the UI
 */
static void ui_draw_soft_cruise_btn(UIState *s) {
  if (btn_x == 0 || btn_set_y == 0 || btn_res_y == 0) {
    const UIScene *scene = &s->scene;
    int ui_viz_rx = scene->ui_viz_rx;
    int ui_viz_rw = scene->ui_viz_rw;
    const int bdr_s = 30;
    const int viz_event_w = 220;
    const int viz_event_x = ((ui_viz_rx + ui_viz_rw) - (viz_event_w + (bdr_s*2)));
    const int btn_gap = 40;
    btn_x = viz_event_x + (viz_event_w-btn_size);
    btn_set_y = (footer_y + ((footer_h - btn_size) / 2));
    btn_res_y = btn_set_y - btn_size - btn_gap;
  }

  const int btn_txt_offset = 80;
  const int btn_txt_x = btn_x + btn_txt_offset;
  const int btn_set_txt_y = btn_set_y + btn_txt_offset;
  const int btn_res_txt_y = btn_res_y + btn_txt_offset;

  // SET/-
  nvgBeginPath(s->vg);
  nvgRoundedRect(s->vg, btn_x, btn_set_y, btn_size, btn_size, 20);
  nvgStrokeColor(s->vg, nvgRGBA(255, 255, 255, 80));
  nvgStrokeWidth(s->vg, 6);
  nvgStroke(s->vg);

  nvgTextAlign(s->vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
  nvgFontFace(s->vg, "sans-bold");
  nvgFontSize(s->vg, 72);
  nvgFillColor(s->vg, nvgRGBA(255, 255, 255, 200));
  nvgText(s->vg, btn_txt_x, btn_set_txt_y, "SET/-", NULL);

  // RES/+
  nvgBeginPath(s->vg);
  nvgRoundedRect(s->vg, btn_x, btn_res_y, btn_size, btn_size, 20);
  nvgStrokeColor(s->vg, nvgRGBA(255, 255, 255, 80));
  nvgStrokeWidth(s->vg, 6);
  nvgStroke(s->vg);

  nvgTextAlign(s->vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
  nvgFontFace(s->vg, "sans-bold");
  nvgFontSize(s->vg, 72);
  nvgFillColor(s->vg, nvgRGBA(255, 255, 255, 200));
  nvgText(s->vg, btn_txt_x, btn_res_txt_y, "RES/+", NULL);
}

static void update_soft_cruise() {
  struct capn rc;
  capn_init_malloc(&rc);
  struct capn_segment *cs = capn_root(&rc).seg;

  cereal_SoftCruise_ptr p = cereal_new_SoftCruise(cs);
  struct cereal_SoftCruise sc = (struct cereal_SoftCruise) {
    .state = cruise_state,
    .speed = cruise_speed
  }
  cereal_write_SoftCruise(&sc, p);
}

/**
 * Actions when touch "RES/+" button
 */
static void do_res() {
  if (cruise_state) {
    cruise_speed += 1;
  } else {
    if (cruise_speed > 0) {
      cruise_state = true;
    }
  }
  update_soft_cruise();
}

/**
 * Actions when touch "SET/-" button
 */
static void do_set() {
  if (cruise_state) {
    cruise_speed -= 1;
  } else {
    cruise_state = true;
  }
  update_soft_cruise();
}

/**
 * Listen to touch events within the coordinate.
**/
static void on_btn_touched(UIState *s, int touch_x, int touch_y) {
  float speed = s->scene.v_ego;
  if (touch_x >= btn_x && touch_x <= (btn_x+btn_size)) {
    if (touch_y >= btn_set_y && touch_y <= (btn_set_y+btn_size)) {
      do_set();
    }
    if (touch_y >= btn_res_y && touch_y <= (btn_res_y+btn_size)) {
      do_res();
    }
  }
}


static void init() {
  update_soft_cruise();
  is_init = true;
}

void soft_cruise(UIState *s, int touch_x, int touch_y) {
  if (is_init == false) {
    init();
  }
  // only draw buttons when vision is connected
  if (s->vision_connected && s->plus_state == 0) {
    ui_draw_soft_cruise_btn(s);
    on_btn_touched(s, touch_x, touch_y);
  }
}