#!/usr/bin/python

import sys, getopt, os

import subprocess

def invoke_bash(bash_command):
	subprocess.call(bash_command, shell=True)


output_dir_name = ''

if __name__ == "__main__":
   try:
      opts, args = getopt.getopt(sys.argv[1:],"hd:",['output-dir-name='])
   except getopt.GetoptError:
      print 'Error_unknown_flag: gnatcov_gen.py [-d <directory name>|--output-dir-name=<directory name>]'
      sys.exit(2)
   for opt, arg in opts:
      if opt == '-h':
         print 'gnatcov_gen.py [-d <directory name>|--output-dir-name=<directory name>]'
         sys.exit()
      elif opt in ("-d", "--output-dir-name"):
         output_dir_name = arg

   
   
if not output_dir_name:
   output_dir_name = "gcov_output"

if not os.path.exists(output_dir_name):
    os.makedirs(output_dir_name)
 
output_dir_name = "./" + output_dir_name
    
invoke_bash("lcov -c -i -d . -o .coverage.base")
invoke_bash("lcov -c -d . -o .coverage.run")
invoke_bash("lcov -d . -a .coverage.base -a .coverage.run -o .coverage.total")
invoke_bash("genhtml --no-branch-coverage -o " + output_dir_name + " .coverage.total")
invoke_bash("rm -f .coverage.base .coverage.run .coverage.total")

