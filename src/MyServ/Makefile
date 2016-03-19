CXXFLAGS =	-O2 -g -fmessage-length=0 -I/usr/include/libxml2

OBJS =		MyServ.o CMD_Map.o CMD_OBJ.o

LIBS=-lxml2

TARGET =	../../web-control-center/usr/bin/web-control-center/MyServ

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
