Translate a python script
--
1. add following codes to the top
```python
# This Python file uses the following encoding: utf-8
# -*- coding: utf-8 -*-
from common.i18n import events
_ = events()
```
2. wrap ```_()``` function around the string, check out events.py as example.

3. generate pot file (assuming you are in openpilot/ directory.)
```bash
xgettext -d base -o selfdrive/assets/locales/events.pot selfdrive/controls/lib/events.py
```

4. add po file to languages
```bash
cp selfdrive/assets/locales/events.pot selfdrive/assets/locales/zh-TW/LC_MESSAGES/events.po
```

5. translate po file with your favorite editor.

6. generate mo file.
```bash
msgfmt -o selfdrive/assets/locales/zh-TW/LC_MESSAGES/events.mo selfdrive/assets/locales/zh-TW/LC_MESSAGES/events
```

Update translations
---
1. add ```_()``` around the new strings

2. generate pot file again (assuming you are in openpilot/ directory.)
```bash
xgettext -d base -o selfdrive/assets/locales/events.pot selfdrive/controls/lib/events.py
```

3. update already translated file (merge)
```bash
msgmerge --update selfdrive/assets/locales/zh-TW/LC_MESSAGES/events.po selfdrive/assets/locales/events.pot
```

4. generate mo file again.
```bash
msgfmt -o selfdrive/assets/locales/zh-TW/LC_MESSAGES/events.mo selfdrive/assets/locales/zh-TW/LC_MESSAGES/events
```

Reference
---
https://simpleit.rocks/python/how-to-translate-a-python-project-with-gettext-the-easy-way/
