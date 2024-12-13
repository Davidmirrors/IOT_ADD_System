    #Makefile格式
#目标文件:依赖的文件
#Tab 命令1
#Tab 命令2

#定义变量
#ARCH默认为x86，使用gcc编译器，
#否则使用arm编译器
# ARCH ?= arm
ARCH ?= x86
TARGET = server

#存放中间文件的路径
BUILD_DIR = out
#存放源文件的文件夹
SRC_DIR = src
#SRC_DIR += 其它源文件的路径        #增加源文件的文件夹

#存放头文件的文件夹
INC_DIR = inc .
# INC_DIR += /home/jpg/include/        #增加头文件的文件夹

# 库
LIB_NAMES =-lpthread
# LIB_NAMES +=-l其它库的名字           #增加库
#库路径
# LIB_PATH =-L /home/jpg/lib/
# LIB_PATH +=-L其它库的路径           #增加库的路径

#源文件
SRCS = $(wildcard $(SRC_DIR)/*.c)
#目标文件（*.o）
OBJS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(notdir $(SRCS)))
#头文件
DEPS = $(wildcard $(INC_DIR)/*.h)

#指定头文件的路径
CFLAGS = $(patsubst %, -I%, $(INC_DIR))

#根据输入的ARCH变量来选择编译器
#ARCH=x86，使用gcc
#ARCH=arm，使用arm-gcc
ifeq ($(ARCH),x86)
  CC = gcc
else
  CC = arm-linux-gcc
endif

#目标文件
$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIB_PATH) $(LIB_NAMES) 
# $(CC) -o $@ $^ $(CFLAGS) 
#如果有库，可以用下面代码
	

#*.o文件的生成规则
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
#创建一个编译目录，用于存放过程文件
#命令前带"@",表示不在终端上输出
	@mkdir -p $(BUILD_DIR)
	$(CC) -c -o $@ $< $(CFLAGS)

#伪目标
.PHONY: clean cleanall
#按架构删除
clean:
	rm -rf $(BUILD_DIR)

#全部删除
cleanall:
	rm -rf build_x86 build_arm