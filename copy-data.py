import sys, os, os.path
from distutils.dir_util import mkpath
from shutil import copyfile

def copytree (src, dst):
    assert os.path.isdir(src)
    
    names = os.listdir(src)

    mkpath(dst)

    for n in names:        
        src_name = os.path.join(src, n)
        dst_name = os.path.join(dst, n)
        
        if os.path.exists(dst_name):
          sizesEqual = os.path.getsize(dst_name) == os.path.getsize(src_name)
          sourceIsNewer = os.path.getmtime(src_name) > os.path.getmtime(dst_name)
          
          if sizesEqual and not sourceIsNewer:
            continue
        
        if os.path.isdir(src_name):
            copytree(src_name, dst_name)
        else:
            print "Copying %s to %s" % (src_name, dst_name)
            copyfile(src_name, dst_name)

if len(sys.argv) != 4:
  print "invalid action args"
  os.exit(-1)

cwd = sys.argv[1]
src = sys.argv[2]
dst = sys.argv[3]

src = os.path.join(cwd, src)
dst = os.path.join(cwd, dst)

if (os.path.isdir(src)):
  copytree(src, dst)
else:
  copyfile(src, dst)