#!/usr/bin/python

import sys, getopt, os

import subprocess


def invoke_bash(bash_command):
	subprocess.call(bash_command, shell=True)


output_dir_name = ''
check_opt = ''
exec_file_path="./flight_mgmt_rs/mgmt/mgmt"

if __name__ == "__main__":
   try:
      opts, args = getopt.getopt(sys.argv[1:],"hd:",['output-dir-name=','check='])
   except getopt.GetoptError:
      print 'Error_unknown_flag: prof_gen.py --check=[cache|call|mem|massif] [-d <directory name>|--output-dir-name=<directory name>]'
      sys.exit(2)
   for opt, arg in opts:
      if opt == '-h':
         print 'prof_gen.py --check=[cache|call|mem|massif] [-d <directory name>|--output-dir-name=<directory name>]'
         sys.exit()
      elif opt in ("-d", "--output-dir-name"):
         output_dir_name = arg
      elif opt == "--check":
      	 if arg in ("cache","call","mem","massif"):
    		 check_opt = arg
         else:	 
		     print 'Error_unknown_option: Unknown option for --check'
		     sys.exit(2)
         	 
   
if not check_opt:
   print 'Error_unknown_option: Unknown option for --check'
   sys.exit(2)
   
if not output_dir_name:
   output_dir_name = "prof_output"

if not os.path.exists(output_dir_name):
    os.makedirs(output_dir_name)
 
output_dir_name = "./" + output_dir_name

if check_opt == "cache":
 
	invoke_bash("valgrind --tool=cachegrind --cachegrind-out-file=" + output_dir_name + "/cachegrind.out " + exec_file_path)
	invoke_bash("cg_annotate --show=Dr,Dw " + output_dir_name + "/cachegrind.out > " + output_dir_name + "/cachegrind_an.txt")

if check_opt == "call" :

	invoke_bash("valgrind --dsymutil=yes --tool=callgrind --callgrind-out-file=" + output_dir_name + "/callgrind.out " + exec_file_path)
	invoke_bash("callgrind_annotate " + output_dir_name + "/callgrind.out > " + output_dir_name + "/callgrind_an.txt")

if check_opt == "massif" :

	invoke_bash("valgrind --tool=massif --stacks=yes --massif-out-file=" + output_dir_name + "/massif.out " + exec_file_path)
	invoke_bash("ms_print " + output_dir_name + "/massif.out > " + output_dir_name + "/massif_an.txt")

if check_opt == "mem" :

	invoke_bash("valgrind --tool=memcheck --leak-check=full " + exec_file_path + "  2>  " + output_dir_name + "/memcheck_an.txt")


