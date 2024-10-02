#1.cmake verson，指定cmake版本
cmake_minimum_required(VERSION 3.2)
	
#2.project name，指定项目的名称，一般和项目的文件夹名称对应,声明一个cmake工程，工程名为stardtest
PROJECT(sylar)
# #也可以写上项目的版本号，如下所示
# project(stardtest VERSION 0.1.0)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O0  -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function")

#3.添加c++ 11标准支持，如果程序中使用了C++11标准，则需要设置告诉编译器，没有可以不用写。
# set( CMAKE_CXX_FLAGS "-std=c++11")

#4.设置编译器编译模式，对于编译用的Debug模式和调试用的Release模式，在Debug模式中，程序运行较慢，当可以在IDE中进行断点调试，而Release模式则速度较快，但没有调试信息。不设置默认是Debug模式。
# set( CMAKE_BUILD_TYPE "Debug")

#5.添加引用的第三方头文件，此项目main.cpp引用的头文件有Sum.h，因此需要添加头文件目录，因为和CMakeList.txt同级，所以此处目录只写include即可，应写对头文件所在的目录，以防找不到相应的头文件而报错，如果难以确定就直接写上绝对路径即可
include_directories(.)
# include_directories(${PROJECT_SOURCE_DIR}/sylar)    #其实头文件也可以放在源文件所在的文件夹中，这样就不用添加此命令,但是为了整个工程的简洁干净便于管理，所以建议放到include文件夹中。
# #也可以将include_directories全部大写，如下所示，是一样的表达，其他的类似，也是可以全部大写表示的，例如add_executable写成ADD_EXECUTABLE
# INCLUDE_DIRECTORIES(include)


# #------------------------src下面有多个.cpp需要编译,相应的库函数也都有,这个时候可以执行下面的步骤------------------------------------
# #6.source directory，源文件目录,j将源文件目录/路径 src赋值给DIR_SRCS
# AUX_SOURCE_DIRECTORY(src DIR_SRCS)
# #7.set environment variable，设置环境变量，编译用到的源文件全部都要放到这里，否则编译能够通过，但是执行的时候会出现各种问题，比如"symbol lookup error xxxxx , undefined symbol"
# SET(TEST_MATH ${DIR_SRCS}) #将编译用的源文件所在路径DIR_SRCS赋值给TEST_MATH
# # #8.编译生成库文件，将Sum.cpp生成共享库文件libSum.so，这条命令告诉cmake，把这些源文件编译成一个叫作“Sum”的共享库,其实有了6和7没有必要再执行这一步
# # add_library(Sum SHARED src/xxx1.cpp src/xxx2.cpp ...)
# #9.add executable file，添加要编译的可执行文件
# ADD_EXECUTABLE(${PROJECT_NAME} ${TEST_MATH}) #这里生成的可执行文件的名字为项目名字，${PROJECT_NAME}就是#2中PROJECT(stardtest)的项目名字stardtest
# #10.add link library，添加可执行文件所需要的库，比如我们用到了libm.so（命名规则：lib+name+.so），以及生成的libxxx.so,就添加这些库的名称
# TARGET_LINK_LIBRARIES(${PROJECT_NAME} m)

set(LIB_SRC
sylar/log.cpp
sylar/util.cpp
sylar/config.cpp
sylar/thread.cpp
)
#6.编译生成库文件，将Sum.cpp生成共享库文件libSum.so，这条命令告诉cmake，把这些源文件编译成一个叫作“Sum”的共享库
add_library(sylar SHARED ${LIB_SRC})

set(LIB_LIB 
    sylar
    pthread
    yaml-cpp)

#使用add_library(Sum Sum.cpp)能同时生成静态库文件libSum.a和动态库文件libSum.so
#使用add_library(Sum STATIC Sum.cpp)能生成静态库文件libSum.a
#在linux中，库文件分为静态库和动态库两种，静态库以.a作为后缀名，共享库以.so结尾。所有库都是一些函数打包后的集合，差别在于静态库每次被调用都会生成一个副本，而共享库则只有一个副本，更省空间。
#同时add_library(Sum SHARED Sum.cpp)后面的源文件可以接任意多个，同时生成一个库文件，例如add_library(Sum SHARED Sum.cpp b.cpp d.cpp hello.a ...)
# #对于已经有的外部依赖库的添加实例如下:
# # 寻找OpenCV库,感觉下面两步没啥作用
# set(OpenCV_DIR /home/chaofan/opt/opencv-3.4.4/release) #相当于将后面参数赋给前面的参数,类似于给路径起了个别名
# find_package( OpenCV 3 REQUIRED )
# # 指定库文件路径,全局请求所有库文件,并赋值给Opencv3.0_LIB
# file(GLOB_RECURSE Opencv3.0_LIB "/home/LiuMC/software/third_lib/opencv3.0-install/lib/*.so")
# # 指定头文件路径
# set(Opencv3_INLCUDE_DIRS "/home/LiuMC/software/third_lib/opencv3.0-install/include")
# # 添加头文件到工程中
# include_directories(include ${OpenCV_INCLUDE_DIRS})
# include_directories(${CMAKE_SOURCE_DIR}/sylar) 
# # 执行主文件imageBasics.cpp,生成可执行文件imageBasics
# add_executable( imageBasics imageBasics.cpp )
# # 链接OpenCV库文件到工程中
# target_link_libraries( imageBasics ${OpenCV3.0_LIB} 

#7.add executable file，添加要编译的可执行文件，编译main.cpp，生成可执行文件main，也可以将main写成${PROJECT_NAME}，即为当前项目名称，就是#2中PROJECT(stardtest)的项目名字stardtest
add_executable(test tests/test.cpp) 
add_dependencies(test sylar)
 
#8.add link library，添加可执行文件所需要的库，比如我们用到了libSum.so（命名规则：lib+name+.so），就添加该库的名称
target_link_libraries(test ${LIB_LIB})  #生成的主文件可执行文件main链接到共享库文件库libSum.so，可执行程序即可调用库文件中的函数
link_directories(/usr/local/lib)
add_executable(test_config tests/test_config.cpp) 
add_dependencies(test_config sylar)
target_link_libraries(test_config sylar ${LIB_LIB})

add_executable(test_thread tests/test_thread.cpp) 
add_dependencies(test_thread sylar)
target_link_libraries(test_thread sylar ${LIB_LIB})
 

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

