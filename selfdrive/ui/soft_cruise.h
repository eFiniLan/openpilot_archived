#include <stdio.h>

bool is_init = false;
int cruise_speed = 0;

static void ui_draw_soft_cruise_btn(UIState *s) {
  const UIScene *scene = &s->scene;
  int ui_viz_rx = scene->ui_viz_rx;
  int ui_viz_rw = scene->ui_viz_rw;

  const int bdr_s = 30;
  const int viz_event_w = 220;
  const int viz_event_x = ((ui_viz_rx + ui_viz_rw) - (viz_event_w + (bdr_s*2)));

  const int btn_size = 160;
  const int btn_gap = 40;

  const int btn_1_x = viz_event_x + (viz_event_w-btn_size);
  const int btn_1_y = (footer_y + ((footer_h - btn_size) / 2));
  const int btn_1_txt_x = btn_1_x + 80;
  const int btn_1_txt_y = btn_1_y + 80;

  const int btn_2_x = btn_1_x;
  const int btn_2_y = btn_1_y - btn_size - btn_gap;
  const int btn_2_txt_x = btn_2_x + 80;
  const int btn_2_txt_y = btn_2_y + 80;

  const int btn_3_x = btn_2_x;
  const int btn_3_y = btn_2_y - btn_size - btn_gap;
  const int btn_3_txt_x = btn_3_x + 80;
  const int btn_3_txt_y = btn_3_y + 80;

  // set / -
  nvgBeginPath(s->vg);
  nvgRoundedRect(s->vg, btn_1_x, btn_1_y, btn_size, btn_size, 20);
  nvgStrokeColor(s->vg, nvgRGBA(255,255,255,80));
  nvgStrokeWidth(s->vg, 6);
  nvgStroke(s->vg);

  nvgTextAlign(s->vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
  nvgFontFace(s->vg, "sans-bold");
  nvgFontSize(s->vg, 72);
  nvgFillColor(s->vg, nvgRGBA(255, 255, 255, 200));
  nvgText(s->vg, btn_1_txt_x, btn_1_txt_y, "SET/-", NULL);

  // res / +
  nvgBeginPath(s->vg);
  nvgRoundedRect(s->vg, btn_2_x, btn_2_y, btn_size, btn_size, 20);
  nvgStrokeColor(s->vg, nvgRGBA(255,255,255,80));
  nvgStrokeWidth(s->vg, 6);
  nvgStroke(s->vg);

  nvgTextAlign(s->vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
  nvgFontFace(s->vg, "sans-bold");
  nvgFontSize(s->vg, 72);
  nvgFillColor(s->vg, nvgRGBA(255, 255, 255, 200));
  nvgText(s->vg, btn_2_txt_x, btn_2_txt_y, "RES/+", NULL);
}

static void set_cruise_state(int val) {
  FILE *f = fopen("/tmp/cruise_state", "wb");
  if (f != NULL) {
    fprintf(f, "%d", val);
    fclose(f);
  }
}

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

static void set_cruise_speed(int val) {
  FILE *f = fopen("/tmp/cruise_speed", "wb");
  if (f != NULL) {
    fprintf(f, "%d", val);
    fclose(f);
    cruise_speed = val;
  }
}

static void do_res(int speed) {
  if (get_cruise_state() == 1) {
    set_cruise_speed(cruise_speed+1);
  } else {
    if (cruise_speed > 0) {
      set_cruise_state(1);
    }
  }
}

static void do_set(int speed) {
  if (cruise_speed == 0 && speed > 0) {
    set_cruise_speed(speed);
  } else {
    if (get_cruise_state() == 1) {
      set_cruise_speed(cruise_speed-1);
    }
  }
  set_cruise_state(1);
}



static void on_btn_touched(UIState *s, int touch_x, int touch_y) {
  float speed = s->scene.v_ego;
  if (touch_x >= 1670 && touch_x <= 1830) {
    if (touch_y >= 830 && touch_y <= 990) {
      do_set(speed);
    }
    if (touch_y >= 630 && touch_y <= 790) {
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
  ui_draw_soft_cruise_btn(s);
  on_btn_touched(s, touch_x, touch_y);
}
