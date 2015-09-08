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
   output_dir_name = "gnatcov_output"

if not os.path.exists(output_dir_name):
    os.makedirs(output_dir_name)
 
output_dir_name = "./" + output_dir_name
    
invoke_bash("gnatcov run --output="+ output_dir_name +"/main.trace  ./flight_mgmt_rs/mgmt/mgmt")
invoke_bash("find -name *.c.gli | tee "+ output_dir_name +"/gli_files_list.ali")
invoke_bash("gnatcov coverage --scos=@"+ output_dir_name +"/gli_files_list.ali --annotate=html --level=stmt "+ output_dir_name +"/main.trace --output-dir="+ output_dir_name)
