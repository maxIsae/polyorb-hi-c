# Set to true to enable monitoring 
MONITORING = false
# Set COVERAGE to gcov/gnatcov to enable code coverage : e.g. COVERAGE = gcov
COVERAGE = gcov
# Set VALGRIND_PROF to true to enable profiling with callgrind: e.g.  VALGRIND_PROF = true
VALGRIND_PROF = true 
 
################################################################################
# Added content for integration

# I need the name of the AADL source from which the c code is generated
#AADL_SOURCE = flt_mgmt.aadl

# Two scripts to call for computing and parsing Cheddar xml and event table
#hyperperiod-cheddar:
#	python ../../../src/monitoring/cheddar_scheduling/get_cheddar_schedule.py $AADL_SOURCE
#	python ../../../src/monitoring/cheddar_scheduling/parser_cheddar_et.py

