all:easysvr
clean:
	rm -f easysvr
easysvr:easysvr.cpp
	g++ -g -o easysvr easysvr.cpp
