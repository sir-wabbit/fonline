#!/usr/bin/python

import sys, os, os.path
from distutils.dir_util import mkpath
import shutil

def needs_update(src, dst):
  if os.path.exists(dst):
    sizesEqual = os.path.getsize(dst) == os.path.getsize(src)
    sourceIsNewer = os.path.getmtime(src) > os.path.getmtime(dst)
    
    if sizesEqual and not sourceIsNewer:
      return False
    
    return True
  return True

def copy_file(src, dst, always):
  if not always and not needs_update(src, dst):
    return
    
  print ("Copying %s to %s" % (src, dst))
  shutil.copyfile(src, dst)
  
def copy_path (src, dst, always):
  if os.path.isdir(src):
    copy_dir(src, dst, always)
  else:
    copy_file(src, dst, always)
  
def copy_dir (src, dst, always):
    assert os.path.isdir(src)
    
    names = os.listdir(src)

    mkpath(dst)

    for n in names:        
        src_name = os.path.join(src, n)
        dst_name = os.path.join(dst, n)
        copy_path(src_name, dst_name, always)

if len(sys.argv) != 5:
  print ("invalid action args")
  os.exit(-1)

cwd = sys.argv[1]
src = sys.argv[2]
dst = sys.argv[3]
always = sys.argv[4] == "true"

src = os.path.join(cwd, src)
dst = os.path.join(cwd, dst)

copy_path(src, dst, always)
