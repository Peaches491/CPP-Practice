CXXFLAGS =	-O2 -g -Wall -fmessage-length=0 --std=c++0x

OBJS =		TestProj.o TestClass.o

LIBS =		

TARGET =	TestProj.exe

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
