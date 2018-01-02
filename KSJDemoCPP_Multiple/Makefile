######################################
#
######################################
#source file
#Դ�ļ����Զ�������.c��.cpp�ļ�������Ŀ�궨��Ϊͬ��.o�ļ�

PLATFORM_BIN_DIR=x64

#CROSS_COMPILE=arm-linux-gnueabihf-
#PLATFORM_BIN_DIR=arm32

#CROSS_COMPILE=aarch64-linux-gnu-
#PLATFORM_BIN_DIR=arm64


CC=$(CROSS_COMPILE)gcc
CXX=$(CROSS_COMPILE)g++


SOURCE  := $(wildcard *.c) $(wildcard *.cpp)
OBJS    := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCE)))
  
#target you can change test to what you want
#Ŀ���ļ�����������������Ҫ��ִ���ļ���
TARGET  := catchbesttest
  
#compile and lib parameter
#�������
#CC      := /home/terry/work/bubblegum/toolchain/aarch64-linux-gnu/bin/aarch64-linux-gnu-g++
#CC      := g++
LIBS    :=
#LDFLAGS :=  -L../../github.KSJApiNew/KSJApi.bin/linux/$(PLATFORM_BIN_DIR) -lpng16 -lswresample -llzma -lavcodec  -lavutil -lksjapi -lzvbi -lksjlog -lksjbayer  -ljpeg -lusb-1.0 -ldl
LDFLAGS :=  -L../../github.KSJApiNew/KSJApi.bin/linux/$(PLATFORM_BIN_DIR)  -lopencv_core -lopencv_highgui  -lksjapi -lksjlog -lksjbayer -lpthread  -ludev -ljpeg  -ldl
DEFINES :=
INCLUDE := -I. -I.. -I../KSJApi.inc 
CFLAGS  := -gdwarf-2 -g3 -Wall  $(DEFINES) $(INCLUDE) -Wl,-rpath,../../github.KSJApiNew/KSJApi.bin/linux/$(PLATFORM_BIN_DIR)
CXXFLAGS:= $(CFLAGS) -DHAVE_CONFIG_H
  
  
  
  
  
#i think you should do anything here
#����Ļ����ϲ���Ҫ���κθĶ���
.PHONY : everything objs clean veryclean rebuild
  
everything : $(TARGET)
  
all : $(TARGET)
  
objs : $(OBJS)
  
rebuild: veryclean everything
                
clean :
	rm -fr *.so
	rm -fr *.o
veryclean : clean
	rm -fr $(TARGET)
$(TARGET) : $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS) $(LIBS)
install:
	cp $(TARGET) ../build/
