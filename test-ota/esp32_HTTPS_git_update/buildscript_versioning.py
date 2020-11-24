import datetime
import binascii
Import ("env")
tm = datetime.datetime.today()

FILENAME_BUILDNO = 'versioning'
FILENAME_VERCONFIG = 'verconfig'
FILENAME_VERSION_H = 'include/version.h'
version = '0.1.'

build_no = 0
try:
    with open(FILENAME_BUILDNO) as f:
        build_no = int(f.readline()) + 1
except:
    print('Starting build number from 1..')
    build_no = 1
with open(FILENAME_BUILDNO, 'w+') as f:
    f.write(str(build_no))
    print('Build number: {}'.format(build_no))

hf = """
#ifndef BUILD_NUMBER
  #define BUILD_NUMBER "{}"
#endif
#ifndef VERSION
  #define VERSION "{}"
#endif
#ifndef VERSION_DATETIME
  #define VERSION_DATETIME "{}"
#endif
""".format(build_no, version+str(build_no), str(tm.year)+('0'+str(tm.month))[-2:]+('0'+str(tm.day))[-2:] +"_"+ ('0'+str(tm.hour))[-2:] + ('0'+str(tm.minute))[-2:] + ('0'+str(tm.second))[-2:] , version+str(build_no))
with open(FILENAME_VERSION_H, 'w+') as f:
    f.write(hf)


gitRepo       = env.GetProjectOption("GitRepository")
GitDir        = env.GetProjectOption("GitDir")
product_type  = env.GetProjectOption("board_type")
sign          = env.GetProjectOption("signature")
description   = env.GetProjectOption("description")

sign = ''.join(hex(ord(c))[2:] for c in sign)
dtbuild = (str(tm.year)+('0'+str(tm.month))[-2:]+('0'+str(tm.day))[-2:] +'_'+ ('0'+str(tm.hour))[-2:] + ('0'+str(tm.minute))[-2:] + ('0'+str(tm.second))[-2:])
fw_url = ("\"fwurl\":\"https://raw.githubusercontent.com/"+gitRepo+"/main/"+GitDir+"/firmware_" +dtbuild+".bin\"")
httpsCf = ("{\"type\":\""+product_type+"\",\"fwver\":\""+ dtbuild +"\",\"signature\":\""+sign+"\"," +fw_url+",\"description\":\""+description+"\"}")

typeOTA = ''

cf = ("{\""+product_type+"\":" +httpsCf+"}")
with open(FILENAME_VERCONFIG, 'w+') as f:
  f.write(cf)


env.Replace(PROGNAME="firmware_%s" %str(dtbuild))
# env.Replace(PROGNAME="firmware_%s" %str (version+str(build_no)))
# print env['PROGNAME'] 