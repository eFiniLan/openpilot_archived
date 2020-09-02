import os
import gettext
from common.hardware import HARDWARE, ANDROID

supported_language = ['en-US', 'fr-FR', 'ja-JP', 'ko-KR', 'pt-BR', 'zh-CN', 'zh-TW']
locale = HARDWARE.getprop("persist.sys.locale") if ANDROID else 'en-US'

def events():
  i18n = gettext.translation('events', localedir=os.environ['BASEDIR'] + '/locales', fallback=True, languages=[locale])
  i18n.install()
  return i18n.gettext
