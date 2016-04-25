#!/usr/bin/python
import base64

base = base64.encode(r'binary\x00strin')
print base
print base64.decode(base)
l = r'i\x7\x1d\xfb\xef\xff'
base1 = base64.b64encode(l)
#base1 = base64.b64encode(l)
print base1
base11 = base64.urlsafe_b64encode(l)
print base11
print base64.urlsafe_b64decode(l)
