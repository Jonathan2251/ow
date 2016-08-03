#! /bin/bash

echo "Formating bechmark log $1"
export PROESS_FILE=$1
sed -i "s/>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>/,/" $PROESS_FILE
sed -i "s/[>][>]*[ ]*/,\n/g" $PROESS_FILE
sed -i "/^[<]/d" $PROESS_FILE
sed -i "s/[-][-]*//g" $PROESS_FILE
sed -i "s/, [ ]*/,/g" $PROESS_FILE
sed -i "s/[ ][ ]*/,/g" $PROESS_FILE

