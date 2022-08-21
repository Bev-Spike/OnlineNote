aux_source_directory(${NOTE_SRC_DIR} LIB_SRCS)			# 添加当前目录下所有的源文件
add_library (notelib STATIC ${LIB_SRCS})				# 当前目录下的文件生成一个链接库
target_include_directories(notelib PUBLIC ${NOTE_INCLUDE_DIR} )  #允许上级继承头文件
