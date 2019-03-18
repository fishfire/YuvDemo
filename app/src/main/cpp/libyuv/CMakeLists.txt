
# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_LIB_SRCS 变量
include_directories(include/ include/libyuv)
aux_source_directory(source/.  DIR_LIB_SRCS1)
aux_source_directory(util/.  DIR_LIB_SRCS2)
# 生成链接库
add_library (yuv ${DIR_LIB_SRCS1} ${DIR_LIB_SRCS2})
