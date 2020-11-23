import datetime
Import ("env")
tm = datetime.datetime.today()

FILENAME_BUILDNO = 'versioning'
FILENAME_VERSION_H = 'include/version.h'
version = '0.1.'
# str(tm.year)[-2:]+('0'+str(tm.month))[-2:]+('0'+str(tm.day))[-2:] + ('0'+str(tm.hour))[-2:] + ('0'+str(tm.minute))[-2:] + ('0'+str(tm.second))[-2:]+ '_'

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

    

env.Replace(PROGNAME="firmware_%s" %str(tm.year)+('0'+str(tm.month))[-2:]+('0'+str(tm.day))[-2:] +'_'+ ('0'+str(tm.hour))[-2:] + ('0'+str(tm.minute))[-2:] + ('0'+str(tm.second))[-2:])
# env.Replace(PROGNAME="firmware_%s" %str (version+str(build_no)))
# print env['PROGNAME'] 