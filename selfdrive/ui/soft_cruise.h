//#include <stdio.h>

const int btn_size = 160;

int btn_x = 0;
int btn_set_y = 0;
int btn_res_y = 0;

bool is_init = false;
int cruise_speed = 0;

/**
 * Write cruise state to a tmp file so it's accessible everywhere
 */
static void set_cruise_state(int val) {
  char cmd[100];
  sprintf(cmd, "echo %d > /tmp/cruise_state", val);
  system(cmd);
}

/**
 * Read cruise state from the tmp file because the state can be changed outside
 */
static int get_cruise_state() {
    int fd = open("/tmp/cruise_state", O_RDONLY);
    if (fd < 0) {
      set_cruise_state(0);
      return 0;
    }
    char buffer[1];
    read(fd, buffer, 1);
    return atoi(buffer);
}

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

/**
 * Write cruise speed to a tmp file so it's accessible everywhere
 */
static void set_cruise_speed(int val) {
  char cmd[100];
  sprintf(cmd, "echo %d > /tmp/cruise_speed", val);
  system(cmd);
  cruise_speed = val;
}

/**
 * Actions when touch "RES/+" button
 */
static void do_res(int speed) {
  if (get_cruise_state() == 1) {
    set_cruise_speed(cruise_speed+1);
  } else {
    if (cruise_speed > 0) {
      set_cruise_state(1);
    }
  }
}

/**
 * Actions when touch "SET/-" button
 */
static void do_set(int speed) {
  if (get_cruise_state() == 1) {
    set_cruise_speed(cruise_speed-1);
  } else {
    set_cruise_speed(speed);
    set_cruise_state(1);
  }
}

/**
 * Listen to touch events within the coordinate.
**/
static void on_btn_touched(UIState *s, int touch_x, int touch_y) {
  float speed = s->scene.v_ego;
  if (touch_x >= btn_x && touch_x <= (btn_x+btn_size)) {
    if (touch_y >= btn_set_y && touch_y <= (btn_set_y+btn_size)) {
      do_set(speed);
    }
    if (touch_y >= btn_res_y && touch_y <= (btn_res_y+btn_size)) {
      do_res(speed);
    }
  }
}

static void init() {
  set_cruise_state(0);
  set_cruise_speed(0);
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