#!/bin/sh

EXEC_FILE_PATH=./flight_mgmt_rs/mgmt/mgmt

for i in "$@"
do
case $i in
    --output-dir-name=*)
    OUTPUT_DIR="${i#*=}" #ONLY NAME OF DIR
    shift # past argument=value
    ;;
    --check=*)
    CHECK_OPTION="${i#*=}" #call:callgrind or cache:cachegrind
    shift # past argument=value
    ;;
    *)
            # unknown option
    ;;
esac
done


if [ -z "$OUTPUT_DIR" ]; then
  OUTPUT_DIR=./prof_output
else
  OUTPUT_DIR=./$OUTPUT_DIR
fi

if (! test -d $OUTPUT_DIR )
then
  echo "Missing path: $OUTPUT_DIR. Creating directory..."
  mkdir $OUTPUT_DIR
fi

if [ "$CHECK_OPTION" = "cache" ]; then
  
  
valgrind --tool=cachegrind --cachegrind-out-file=$OUTPUT_DIR/cachegrind.out $EXEC_FILE_PATH
cg_annotate --show=Dr,Dw $OUTPUT_DIR/cachegrind.out > $OUTPUT_DIR/cachegrind_an.txt

#To tell callgrind_annotate to dump out the xxx.c file and to annotate each line with the number of instrustions it took:
#$ callgrind_annotate {callgrind.out path}/callgrind.out {xxx.c path}/xxx.c

elif [ "$CHECK_OPTION" = "call" ]; then

valgrind --dsymutil=yes --tool=callgrind --callgrind-out-file=$OUTPUT_DIR/callgrind.out $EXEC_FILE_PATH
callgrind_annotate $OUTPUT_DIR/callgrind.out > $OUTPUT_DIR/callgrind_an.txt

else

echo "Unknown option"

fi




