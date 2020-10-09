import gettext
from common.hardware import EON
from common.hardware_android import getprop

locale_dir = '/data/openpilot/selfdrive/assets/locales'
supported_language = ['en-US', 'zh-TW', 'zh-CN', 'ja-JP', 'ko-KR']
locale = getprop("persist.sys.locale") if EON else 'en-US'

def events():
  i18n = gettext.translation('events', localedir=locale_dir, fallback=True, languages=[locale])
  i18n.install()
  return i18n.gettext