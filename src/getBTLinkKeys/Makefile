OBJS =		getBTLinkKeys.o
LIBS = -lbluetooth 
TARGET =	../../web-control-center/usr/bin/web-control-center/getBTLinkKeys

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
