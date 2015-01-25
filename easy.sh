#########################################################################
# File Name: easy.sh
# Author: laixukai
# mail: laixukai@126.com
# Created Time: 2015年01月17日 星期六 02时19分19秒
#########################################################################
#!/bin/bash

protoc --cpp_out=.  $1.proto
/home/laixukai/easysvr/easysvr $1
if [ $? -ne 0 ]
then
	exit
fi
cat /home/laixukai/easysvr/svr.template.part1 $1svr.template.part3   $1svr.template.part4 | eval sed 's/TEMPLATE/$1/g' > $1svr.cpp
cat /home/laixukai/easysvr/cli.template.part1 $1cli.template.part2 | eval sed 's/TEMPLATE/$1/g' > $1cli.cpp
