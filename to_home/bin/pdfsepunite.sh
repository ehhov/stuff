#!/bin/bash

input=${1%.pdf}.pdf
if [ $# = 4 ]; then
	st=$2
	end=$3
	output=${4%.pdf}.pdf
fi
if [ $# = 3 ]; then
	st=${2%-*}
	end=${2#*-}
	output=${3%.pdf}.pdf
fi
if [ -z $input ] || [ -z $st ] || [ -z $end ] || [ -z $output ]; then
	echo $0: usage:
	echo -e "\t$0 input.pdf first_page last_page output.pdf"
	exit 1
fi
input=$(pwd)/$input
output=$(pwd)/$output

echo $input pages $st to $end destination $output
echo -n Hit Enter if it is correct or Ctrl-C if not...
read 

dir=/tmp/$(date +%s)
mkdir $dir
cd $dir

pdfseparate -f $st -l $end $input page-%d.pdf
for i in $(ls -v page-*); do
	tmp="$tmp $i"
done
pdfunite $tmp $output

rm -rf $dir

unset input st end output dir tmp
