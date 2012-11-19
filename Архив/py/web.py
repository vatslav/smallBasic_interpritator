#-------------------------------------------------------------------------------
# Name:        модуль1
# Purpose:
#
# Author:      Vatslav
#
# Created:     15.11.2012
# Copyright:   (c) Vatslav 2012
# Licence:     <your licence>
#-------------------------------------------------------------------------------

import os
import re

vid = os.popen("host l2tp.corbina.ru")
re_dns = re.compile(r"([\d]+)\.([\d]+)\.([\d]+)\.([\d]+)")
tmp=None
for line in vid.readlines():
  hst=re_dns.search(line)
  tmp=hst
  if (hst != None):
    break
hst=tmp
host=hst.group(0)
print ("Corbina host adress is "+host)