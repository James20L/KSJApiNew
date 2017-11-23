######################################
#
######################################
#source file
#Դ�ļ����Զ�������.c��.cpp�ļ�������Ŀ�궨��Ϊͬ��.o�ļ�
SOURCE  := $(wildcard *.c) $(wildcard *.cpp)
OBJS    := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCE)))
  
#target you can change test to what you want
#Ŀ���ļ�����������������Ҫ��ִ���ļ���
TARGET  := catchbesttest
  
#compile and lib parameter
#�������
#CC      := /home/terry/work/bubblegum/toolchain/aarch64-linux-gnu/bin/aarch64-linux-gnu-g++
CC      := g++
LIBS    :=
LDFLAGS :=  -L../../github.KSJApiNew/KSJApi.bin/linux -L../../../101/libusb.bin/lib  -L. -L.. -L/usr/local/cuda-8.0/lib64/ -I/usr/local/cuda-8.0/include  -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_contrib -lopencv_imgproc -lOpenCL -lksjapi -lusb-1.0 -ldl
#LDFLAGS :=  -L. -L.. -L/usr/local/cuda-8.0/lib64/ -I/usr/local/cuda-8.0/include  -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_contrib -lopencv_imgproc -lOpenCL ../libksj_640.a  
DEFINES :=
INCLUDE := -I. -I.. -I../KSJApi.inc 
CFLAGS  := -gdwarf-2 -g3 -Wall  $(DEFINES) $(INCLUDE)
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
	$(CC) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS) $(LIBS)
install:
	cp $(TARGET) ../build/
