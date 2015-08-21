#!/bin/sh
for i in "$@"
do
case $i in
    --output-dir-name=*)
    OUTPUT_DIR="${i#*=}" #ONLY NAME OF DIR
    shift # past argument=value
    ;;
    *)
            # unknown option
    ;;
esac
done

if [ -z "$OUTPUT_DIR" ]; then
  OUTPUT_DIR=./gnatcov_output
else
  OUTPUT_DIR=./$OUTPUT_DIR
fi

if (! test -d $OUTPUT_DIR )
then
  echo "Missing path: $OUTPUT_DIR. Creating directory..."
  mkdir $OUTPUT_DIR
fi

gnatcov run --output=$OUTPUT_DIR/main.trace  ./flight_mgmt_rs/mgmt/mgmt

find -name *.c.gli | tee $OUTPUT_DIR/gli_files_list.ali

gnatcov coverage --scos=@$OUTPUT_DIR/gli_files_list.ali --annotate=html --level=stmt $OUTPUT_DIR/main.trace --output-dir=$OUTPUT_DIR


