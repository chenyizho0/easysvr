all: mailsvr mailclient
clean: 
	rm -f mailsvr mailclient mail.pb.h mail.pb.cc
protobuf: mail.proto
	protoc --cpp_out=. mail.proto 
mailclient: mailcli.cpp mailcli.h testmailcli.cpp  protobuf
	g++ -g -o testmailclient mailcli.cpp testmailcli.cpp mail.pb.cc -I/usr/local/include/google/protobuf -L/usr/local/lib -lprotobuf 
mailsvr: mailsvr.cpp mailsvrimpl.cpp mailsvrimpl.h protobuf
	g++ -g -o mailsvr mailsvr.cpp mailsvrimpl.cpp mail.pb.cc -I/usr/local/include/google/protobuf -L/usr/local/lib -lprotobuf 
