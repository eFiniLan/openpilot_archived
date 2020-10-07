import gettext
from common.hardware_android import getprop

is_android = True #android.ANDROID
locale_dir = '/data/openpilot/selfdrive/assets/locales'
supported_language = ['en-US', 'zh-TW', 'zh-CN', 'ja-JP', 'ko-KR']
locale = getprop("persist.sys.locale") if is_android else 'en-US'

def events():
  i18n = gettext.translation('events', localedir=locale_dir, fallback=True, languages=[locale])
  i18n.install()
  return i18n.gettext