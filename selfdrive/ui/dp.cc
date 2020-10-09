#include "dp.hpp"

void ui_draw_df_button(UIState *s) {
//  const UIScene *scene = &s->scene;
//  nvgBeginPath(s->vg);
//  nvgRoundedRect(s->vg, df_btn_x, df_btn_y, df_btn_w, df_btn_h, 20);
//  nvgStrokeColor(s->vg, COLOR_WHITE_ALPHA(80));
//  nvgStrokeWidth(s->vg, 6);
//  nvgStroke(s->vg);
//
//  nvgFontFaceId(s->vg,  s->font_sans_regular);
//  nvgFillColor(s->vg, COLOR_WHITE_ALPHA(200));
//  nvgFontSize(s->vg, (scene->dpLocale == "zh-TW"? 96 : scene->dpLocale == "zh-CN"? 96 : 48));
//  nvgTextAlign(s->vg, NVG_ALIGN_CENTER);
//
//  nvgText(s->vg, df_btn_x + df_btn_w / 2, df_btn_y + df_btn_h / 2,
//  scene->dpDynamicFollow == 4? (scene->dpLocale == "zh-TW"? "自動" : scene->dpLocale == "zh-CN"? "自动" : "AUTO") :
//  scene->dpDynamicFollow == 3? (scene->dpLocale == "zh-TW"? "長距" : scene->dpLocale == "zh-CN"? "长距" : "LONG") :
//  scene->dpDynamicFollow == 2? (scene->dpLocale == "zh-TW"? "正常" : scene->dpLocale == "zh-CN"? "正常" : "NORMAL") :
//  (scene->dpLocale == "zh-TW"? "短距" : scene->dpLocale == "zh-CN"? "短距" : "SHORT"),
//  NULL);
//
//  nvgFontFaceId(s->vg,  s->font_sans_regular);
//  nvgFillColor(s->vg, COLOR_WHITE_ALPHA(200));
//  nvgFontSize(s->vg, 37.5);
//  nvgTextAlign(s->vg, NVG_ALIGN_CENTER);
//  nvgText(s->vg, df_btn_x + df_btn_w / 2, df_btn_y + df_btn_h - 10,
//  (scene->dpLocale == "zh-TW"? "車距" : scene->dpLocale == "zh-CN"? "车距" : "FOLLOW"),
//  NULL);
}

void ui_draw_ap_button(UIState *s) {
//  const UIScene *scene = &s->scene;
//  nvgBeginPath(s->vg);
//  nvgRoundedRect(s->vg, ap_btn_x, ap_btn_y, ap_btn_w, ap_btn_h, 20);
//  nvgStrokeColor(s->vg, COLOR_WHITE_ALPHA(80));
//  nvgStrokeWidth(s->vg, 6);
//  nvgStroke(s->vg);
//
//  nvgFontFaceId(s->vg,  s->font_sans_regular);
//  nvgFillColor(s->vg, COLOR_WHITE_ALPHA(200));
//  nvgFontSize(s->vg, (scene->dpLocale == "zh-TW"? 96 : scene->dpLocale == "zh-CN"? 96 : 48));
//  nvgTextAlign(s->vg, NVG_ALIGN_CENTER);
//
//  nvgText(s->vg, ap_btn_x + ap_btn_w / 2, ap_btn_y + ap_btn_h / 2,
//  scene->dpAccelProfile == 3? (scene->dpLocale == "zh-TW"? "運動" : scene->dpLocale == "zh-CN"? "运动" : "SPORT") :
//  scene->dpAccelProfile == 2? (scene->dpLocale == "zh-TW"? "正常" : scene->dpLocale == "zh-CN"? "正常" : "NORMAL") :
//  (scene->dpLocale == "zh-TW"? "節能" : scene->dpLocale == "zh-CN"? "节能" : "ECO"),
//  NULL);
//
//  nvgFontFaceId(s->vg,  s->font_sans_regular);
//  nvgFillColor(s->vg, COLOR_WHITE_ALPHA(200));
//  nvgFontSize(s->vg, 37.5);
//  nvgTextAlign(s->vg, NVG_ALIGN_CENTER);
//
//  nvgText(s->vg, ap_btn_x + ap_btn_w / 2, ap_btn_y + ap_btn_h - 10,
//  (scene->dpLocale == "zh-TW"? "加速" : scene->dpLocale == "zh-CN"? "加速" : "ACCEL"),
//  NULL);
}

void ui_draw_infobar(UIState *s) {
//  const UIScene *scene = &s->scene;
//  const Rect &viz_rect = scene->viz_rect;
//  const int viz_w = s->fb_w - bdr_s * 2;
//  const int inner_height = float(viz_w) * s->fb_h / s->fb_w;

//  int ui_viz_rx = scene->ui_viz_rx;
//  bool hasSidebar = !s->scene.uilayout_sidebarcollapsed;
//  int rect_w = vwp_w - ui_viz_rx - bdr_s;
//  int rect_x = (hasSidebar? (bdr_s+sbr_w) : ui_viz_rx);
//  int rect_y = vwp_h - bdr_s - info_bar_h;
//  int text_x = rect_w / 2 + ui_viz_rx;
//  int text_y = rect_y + 55;
//
//  char infobar[100];
//  // create time string
//  char date_time[20];
//  time_t rawtime = time(NULL);
//  struct tm timeinfo;
//  localtime_r(&rawtime, &timeinfo);
//  strftime(date_time, sizeof(date_time),"%D %T", &timeinfo);
//
//  // Create temp string
//  char temp[6];
//  snprintf(temp, sizeof(temp), "%02d°C", scene->thermal.getPa0());
//
//  // create battery percentage string
//  char battery[5];
//  snprintf(battery, sizeof(battery), "%02d%%", scene->thermal.getBatteryPercent());
//
//  if (!scene->dpUiDev) {
//    char rel_steer[9];
//    snprintf(rel_steer, sizeof(rel_steer), "%s%05.1f°", scene->controls_state.getAngleSteers() < 0? "-" : "+", fabs(scene->angleSteers));
//
//    char des_steer[9];
//    if (scene->controls_state.getEnabled()) {
//      snprintf(des_steer, sizeof(des_steer), "%s%05.1f°", scene->controls_state.getAngleSteersDes() < 0? "-" : "+", fabs(scene->angleSteersDes));
//    } else {
//      snprintf(des_steer, sizeof(des_steer), "%7s", "-");
//    }
//
//    char lead_dist[8];
//    if (scene->lead_data[0].getStatus()) {
//      snprintf(lead_dist, sizeof(lead_dist), "%06.2fm", scene->lead_data[0].getDRel());
//    } else {
//      snprintf(lead_dist, sizeof(lead_dist), "%7s", "-");
//    }
//
//    snprintf(
//      infobar,
//      sizeof(infobar),
//      "%s /TMP: %s /BAT: %s /REL: %s /DES: %s /DIS: %s",
//      date_time,
//      temp,
//      battery,
//      rel_steer,
//      des_steer,
//      lead_dist
//    );
//  } else {
//    snprintf(
//      infobar,
//      sizeof(infobar),
//      "%s /TMP: %s /BAT: %s",
//      date_time,
//      temp,
//      battery
//    );
//  }
//
//  nvgBeginPath(s->vg);
//  nvgRect(s->vg, rect_x, rect_y, rect_w, info_bar_h);
//  nvgFillColor(s->vg, (scene->brakeLights? COLOR_RED_ALPHA(200) : COLOR_BLACK_ALPHA(scene->dpAppWaze? 150 : 100)));
//  nvgFill(s->vg);
//
//  nvgFontSize(s->vg, hasSidebar? 35:42);
//  nvgFontFaceId(s->vg, s->font_courbd);
//  nvgFillColor(s->vg, COLOR_WHITE_ALPHA(200));
//  nvgTextAlign(s->vg, NVG_ALIGN_CENTER);
//  nvgText(s->vg, text_x, text_y, infobar, NULL);
}

void ui_draw_blindspots(UIState *s) {
//  const UIScene *scene = &s->scene;
//  bool hasSidebar = !scene->uilayout_sidebarcollapsed;
//  int ui_viz_rx = scene->ui_viz_rx;
//  int left_x = (hasSidebar? (bdr_s+sbr_w) : ui_viz_rx);
//  int y = vwp_h - bdr_s - info_bar_h - 100;
//  int right_x = vwp_w - bdr_s;
//
//  if (scene->leftBlindspot) {
//    nvgBeginPath(s->vg);
//    nvgMoveTo(s->vg, left_x, y);
//    nvgLineTo(s->vg, left_x, y+100);
//    nvgLineTo(s->vg, left_x+100, y+100);
//    nvgClosePath(s->vg);
//    nvgFillColor(s->vg, COLOR_RED_ALPHA(200));
//    nvgFill(s->vg);
//  }
//  if (scene->rightBlindspot) {
//    nvgBeginPath(s->vg);
//    nvgMoveTo(s->vg, right_x, y);
//    nvgLineTo(s->vg, right_x, y+100);
//    nvgLineTo(s->vg, right_x-100, y+100);
//    nvgClosePath(s->vg);
//    nvgFillColor(s->vg, COLOR_RED_ALPHA(200));
//    nvgFill(s->vg);
//  }
}

//BB START: functions added for the display of various items
int bb_ui_draw_measure(UIState *s,  const char* bb_value, const char* bb_uom, const char* bb_label,
    int bb_x, int bb_y, int bb_uom_dx,
    NVGcolor bb_valueColor, NVGcolor bb_labelColor, NVGcolor bb_uomColor,
    int bb_valueFontSize, int bb_labelFontSize, int bb_uomFontSize ) {

  nvgTextAlign(s->vg, NVG_ALIGN_CENTER | NVG_ALIGN_BASELINE);
  int dx = 0;
  if (strlen(bb_uom) > 0) {
    dx = (int)(bb_uomFontSize*2.5/2);
  }
  //print value
  nvgFontFaceId(s->vg, s->font_sans_bold);
  nvgFontSize(s->vg, bb_valueFontSize*2.5);
  nvgFillColor(s->vg, bb_valueColor);
  nvgText(s->vg, bb_x-dx/2, bb_y+ (int)(bb_valueFontSize*2.5)+5, bb_value, NULL);
  //print label
  nvgFontFaceId(s->vg, s->font_sans_regular);
  nvgFontSize(s->vg, bb_labelFontSize*2.5);
  nvgFillColor(s->vg, bb_labelColor);
  nvgText(s->vg, bb_x, bb_y + (int)(bb_valueFontSize*2.5)+5 + (int)(bb_labelFontSize*2.5)+5, bb_label, NULL);
  //print uom
  if (strlen(bb_uom) > 0) {
    nvgSave(s->vg);
    int rx =bb_x + bb_uom_dx + bb_valueFontSize -3;
    int ry = bb_y + (int)(bb_valueFontSize*2.5/2)+25;
    nvgTranslate(s->vg,rx,ry);
    nvgRotate(s->vg, -1.5708); //-90deg in radians
    nvgFontFaceId(s->vg, s->font_sans_regular);
    nvgFontSize(s->vg, (int)(bb_uomFontSize*2.5));
    nvgFillColor(s->vg, bb_uomColor);
    nvgText(s->vg, 0, 0, bb_uom, NULL);
    nvgRestore(s->vg);
  }
  return (int)((bb_valueFontSize + bb_labelFontSize)*2.5) + 5;
}

void bb_ui_draw_measures_left(UIState *s, int bb_x, int bb_y, int bb_w ) {
  const UIScene *scene = &s->scene;
  int bb_rx = bb_x + (int)(bb_w/2);
  int bb_ry = bb_y;
  int bb_h = 5;
  NVGcolor lab_color = COLOR_WHITE_ALPHA(200);
  NVGcolor uom_color = COLOR_WHITE_ALPHA(200);
  int value_fontSize=30;
  int label_fontSize=15;
  int uom_fontSize = 15;
  int bb_uom_dx =  (int)(bb_w /2 - uom_fontSize*2.5) ;
  float d_rel = scene->lead_data[0].getDRel();
  float v_rel = scene->lead_data[0].getVRel();

  //add visual radar relative distance
  if (true) {
    char val_str[16];
    char uom_str[6];
    NVGcolor val_color = COLOR_WHITE_ALPHA(200);
    if (scene->lead_data[0].getStatus()) {
      //show RED if less than 5 meters
      //show orange if less than 15 meters
      if((int)(d_rel) < 15) {
        val_color = nvgRGBA(255, 188, 3, 200);
      }
      if((int)(d_rel) < 5) {
        val_color = nvgRGBA(255, 0, 0, 200);
      }
      // lead car relative distance is always in meters
      snprintf(val_str, sizeof(val_str), "%d", (int)d_rel);
    } else {
       snprintf(val_str, sizeof(val_str), "-");
    }
    snprintf(uom_str, sizeof(uom_str), "m   ");
    bb_h +=bb_ui_draw_measure(s,  val_str, uom_str,
       (s->scene.dpLocale == "zh-TW"? "真實車距" : s->scene.dpLocale == "zh-CN"? "真实车距" : "REL DIST"),
        bb_rx, bb_ry, bb_uom_dx,
        val_color, lab_color, uom_color,
        value_fontSize, label_fontSize, uom_fontSize );
    bb_ry = bb_y + bb_h;
  }

  //add visual radar relative speed
  if (true) {
    char val_str[16];
    char uom_str[6];
    NVGcolor val_color = COLOR_WHITE_ALPHA(200);
    if (scene->lead_data[0].getStatus()) {
      //show Orange if negative speed (approaching)
      //show Orange if negative speed faster than 5mph (approaching fast)
      if((int)(v_rel) < 0) {
        val_color = nvgRGBA(255, 188, 3, 200);
      }
      if((int)(v_rel) < -5) {
        val_color = nvgRGBA(255, 0, 0, 200);
      }
      // lead car relative speed is always in meters
      if (s->is_metric) {
         snprintf(val_str, sizeof(val_str), "%d", (int)(v_rel * 3.6 + 0.5));
      } else {
         snprintf(val_str, sizeof(val_str), "%d", (int)(v_rel * 2.2374144 + 0.5));
      }
    } else {
       snprintf(val_str, sizeof(val_str), "-");
    }
    if (s->is_metric) {
      snprintf(uom_str, sizeof(uom_str), "km/h");;
    } else {
      snprintf(uom_str, sizeof(uom_str), "mph");
    }
    bb_h +=bb_ui_draw_measure(s,  val_str, uom_str,
        (s->scene.dpLocale == "zh-TW"? "相對速度" : s->scene.dpLocale == "zh-CN"? "相对速度" : "REAL SPEED"),
        bb_rx, bb_ry, bb_uom_dx,
        val_color, lab_color, uom_color,
        value_fontSize, label_fontSize, uom_fontSize );
    bb_ry = bb_y + bb_h;
  }

  //finally draw the frame
  bb_h += 20;
  nvgBeginPath(s->vg);
    nvgRoundedRect(s->vg, bb_x, bb_y, bb_w, bb_h, 20);
    nvgStrokeColor(s->vg, COLOR_WHITE_ALPHA(80));
    nvgStrokeWidth(s->vg, 6);
    nvgStroke(s->vg);
}

void bb_ui_draw_measures_right(UIState *s, int bb_x, int bb_y, int bb_w ) {
  const UIScene *scene = &s->scene;
  int bb_rx = bb_x + (int)(bb_w/2);
  int bb_ry = bb_y;
  int bb_h = 5;
  NVGcolor lab_color = COLOR_WHITE_ALPHA(200);
  NVGcolor uom_color = COLOR_WHITE_ALPHA(200);
  int value_fontSize=30;
  int label_fontSize=15;
  int uom_fontSize = 15;
  int bb_uom_dx =  (int)(bb_w /2 - uom_fontSize*2.5) ;

  //add  steering angle
  if (true) {
    char val_str[16];
    char uom_str[6];
    NVGcolor val_color = COLOR_WHITE_ALPHA(200);
      //show Orange if more than 6 degrees
      //show red if  more than 12 degrees
      if(((int)(scene->angleSteers) < -6) || ((int)(scene->angleSteers) > 6)) {
        val_color = nvgRGBA(255, 188, 3, 200);
      }
      if(((int)(scene->angleSteers) < -12) || ((int)(scene->angleSteers) > 12)) {
        val_color = nvgRGBA(255, 0, 0, 200);
      }
      // steering is in degrees
      snprintf(val_str, sizeof(val_str), "%.1f°",(scene->angleSteers));

      snprintf(uom_str, sizeof(uom_str), "");
    bb_h +=bb_ui_draw_measure(s,  val_str, uom_str,
      (s->scene.dpLocale == "zh-TW"? "實際轉角" : s->scene.dpLocale == "zh-CN"? "实际转角" : "REAL STEER"),
      bb_rx, bb_ry, bb_uom_dx,
      val_color, lab_color, uom_color,
      value_fontSize, label_fontSize, uom_fontSize );
    bb_ry = bb_y + bb_h;
  }

  //add  desired steering angle
  if (true) {
    char val_str[16];
    char uom_str[6];
    NVGcolor val_color = COLOR_WHITE_ALPHA(200);
    //show Orange if more than 6 degrees
    //show red if  more than 12 degrees
    if(((int)(scene->angleSteersDes) < -6) || ((int)(scene->angleSteersDes) > 6)) {
      val_color = nvgRGBA(255, 188, 3, 200);
    }
    if(((int)(scene->angleSteersDes) < -12) || ((int)(scene->angleSteersDes) > 12)) {
      val_color = nvgRGBA(255, 0, 0, 200);
    }
    // steering is in degrees
    snprintf(val_str, sizeof(val_str), "%.1f°",(scene->angleSteersDes));

    snprintf(uom_str, sizeof(uom_str), "");
    bb_h +=bb_ui_draw_measure(s,  val_str, uom_str,
      (s->scene.dpLocale == "zh-TW"? "預測轉角" : s->scene.dpLocale == "zh-CN"? "预测转角" : "DESIR STEER"),
      bb_rx, bb_ry, bb_uom_dx,
      val_color, lab_color, uom_color,
      value_fontSize, label_fontSize, uom_fontSize );
    bb_ry = bb_y + bb_h;
  }

  //finally draw the frame
  bb_h += 20;
  nvgBeginPath(s->vg);
    nvgRoundedRect(s->vg, bb_x, bb_y, bb_w, bb_h, 20);
    nvgStrokeColor(s->vg, COLOR_WHITE_ALPHA(80));
    nvgStrokeWidth(s->vg, 6);
    nvgStroke(s->vg);
}

void ui_draw_bbui(UIState *s) {
//    const UIScene *scene = &s->scene;
//    const int bb_dml_w = 180;
//    const int bb_dml_x = (scene->ui_viz_rx + (bdr_s * 2));
//    const int bb_dml_y = (box_y + (bdr_s * 1.5)) + 220;
//
//    const int bb_dmr_w = 180;
//    const int bb_dmr_x = scene->ui_viz_rx + scene->ui_viz_rw - bb_dmr_w - (bdr_s * 2);
//    const int bb_dmr_y = (box_y + (bdr_s * 1.5)) + 220;
//
    bb_ui_draw_measures_right(s, 0,0,0);//bb_dml_x, bb_dml_y, bb_dml_w);
    bb_ui_draw_measures_left(s, 0,0,0);//bb_dmr_x, bb_dmr_y, bb_dmr_w);
}