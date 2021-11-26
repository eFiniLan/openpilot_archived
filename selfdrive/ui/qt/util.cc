#include "selfdrive/ui/qt/util.h"

#include <QApplication>
#include <QLayoutItem>
#include <QStyleOption>

#include "selfdrive/common/params.h"
#include "selfdrive/common/swaglog.h"
#include "selfdrive/hardware/hw.h"

QString getVersion() {
  static QString version =  QString::fromStdString(Params().get("Version"));
  return version;
}

QString getBrand() {
  return Params().getBool("Passive") ? "dashcam" : "openpilot";
}

QString getBrandVersion() {
  return getBrand() + " v" + getVersion().left(14).trimmed();
}

QString getUserAgent() {
  return "openpilot-" + getVersion();
}

std::optional<QString> getDongleId() {
  std::string id = Params().get("DongleId");

  if (!id.empty() && (id != "UnregisteredDevice")) {
    return QString::fromStdString(id);
  } else {
    return {};
  }
}

void configFont(QPainter &p, const QString &family, int size, const QString &style) {
  QFont f(family);
  f.setPixelSize(size);
  f.setStyleName(style);
  p.setFont(f);
}

void clearLayout(QLayout* layout) {
  while (QLayoutItem* item = layout->takeAt(0)) {
    if (QWidget* widget = item->widget()) {
      widget->deleteLater();
    }
    if (QLayout* childLayout = item->layout()) {
      clearLayout(childLayout);
    }
    delete item;
  }
}

QString timeAgo(const QDateTime &date) {
  int diff = date.secsTo(QDateTime::currentDateTimeUtc());

  QString s;
  if (diff < 60) {
    s = "now";
  } else if (diff < 3600) { // 60 * 60
    int minutes = diff / 60;
    s = QString(minutes > 1? "%1 minutes ago" : "%1 minute ago").arg(minutes);
  } else if (diff < 86400) { // 60 * 60 * 24
    int hours = diff / 3600;
    s = QString(hours > 1? "%1 hours ago" : "%1 hour ago").arg(hours);
  } else if (diff < 604800) { // 3600 * 24 * 7
    int days = diff / 86400;
    s = QString(days > 1? "%1 days ago" : "%1 day ago").arg(days);
  } else {
    s = date.date().toString();
  }

  return s;
}

void setQtSurfaceFormat() {
  QSurfaceFormat fmt;
#ifdef __APPLE__
  fmt.setVersion(3, 2);
  fmt.setProfile(QSurfaceFormat::OpenGLContextProfile::CoreProfile);
  fmt.setRenderableType(QSurfaceFormat::OpenGL);
#else
  fmt.setRenderableType(QSurfaceFormat::OpenGLES);
#endif
  QSurfaceFormat::setDefaultFormat(fmt);
}

void initApp() {
  Hardware::set_display_power(true);
  Hardware::set_brightness(65);
  setQtSurfaceFormat();
  if (Hardware::EON()) {
    QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
  }
}

ClickableWidget::ClickableWidget(QWidget *parent) : QWidget(parent) { }

void ClickableWidget::mouseReleaseEvent(QMouseEvent *event) {
  emit clicked();
}

// Fix stylesheets
void ClickableWidget::paintEvent(QPaintEvent *) {
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}


void swagLogMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
  static std::map<QtMsgType, int> levels = {
    {QtMsgType::QtDebugMsg, CLOUDLOG_DEBUG},
    {QtMsgType::QtInfoMsg, CLOUDLOG_INFO},
    {QtMsgType::QtWarningMsg, CLOUDLOG_WARNING},
    {QtMsgType::QtCriticalMsg, CLOUDLOG_ERROR},
    {QtMsgType::QtSystemMsg, CLOUDLOG_ERROR},
    {QtMsgType::QtFatalMsg, CLOUDLOG_CRITICAL},
  };

  std::string file, function;
  if (context.file != nullptr) file = context.file;
  if (context.function != nullptr) function = context.function;

  auto bts = msg.toUtf8();
  cloudlog_e(levels[type], file.c_str(), context.line, function.c_str(), "%s", bts.constData());
}


QWidget* topWidget (QWidget* widget) {
  while (widget->parentWidget() != nullptr) widget=widget->parentWidget();
  return widget;
}
