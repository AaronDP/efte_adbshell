LOCAL_PATH:= $(call my-dir)
NCURSESLIB=/data/local/tmp/lib
NCURSESINC=/data/local/tmp/lib/include
include $(CLEAR_VARS)
LOCAL_SRC_FILES:= \
  cfte.cpp c_bind.cpp c_color.cpp c_config.cpp c_desktop.cpp c_hilit.cpp c_history.cpp \
  c_mode.cpp commands.cpp e_block.cpp e_buffer.cpp e_cmds.cpp e_cvslog.cpp \
  e_file.cpp e_fold.cpp e_line.cpp e_loadsave.cpp e_mark.cpp e_print.cpp \
  e_redraw.cpp e_regex.cpp e_search.cpp e_svnlog.cpp e_tags.cpp e_trans.cpp \
  e_undo.cpp egui.cpp fte.cpp g_draw.cpp g_menu.cpp gui.cpp h_ada.cpp \
  h_c.cpp h_cont.cpp h_catbs.cpp h_fte.cpp h_ipf.cpp h_make.cpp h_msg.cpp \
  h_pascal.cpp h_perl.cpp h_plain.cpp h_rexx.cpp h_sh.cpp h_simple.cpp \
  h_tex.cpp i_ascii.cpp i_choice.cpp i_complete.cpp i_input.cpp i_key.cpp \
  i_modelview.cpp i_oview.cpp i_search.cpp i_view.cpp indent.cpp log.cpp \
  o_buffer.cpp o_buflist.cpp o_cvs.cpp o_cvsbase.cpp o_cvsdiff.cpp o_directory.cpp \
  o_list.cpp o_messages.cpp o_model.cpp o_modemap.cpp o_routine.cpp o_svn.cpp \
  o_svnbase.cpp o_svndiff.cpp s_direct.cpp s_files.cpp s_string.cpp \
  s_util.cpp view.cpp \
 \
e_unix.cpp memicmp.cpp \
 \
clip_no.cpp con_ncurses.cpp g_nodlg.cpp g_text.cpp g_unix_pipe.cpp menu_text.cpp \
 defcfg.h
  
#LOCAL_C_INCLUDES += $(LOCAL_PATH) $(LOCAL_PATH)/include # objs.inc defcfg.h 
LOCAL_C_INCLUDES := bionic
LOCAL_C_INCLUDES += $(NCURSESINC)
LOCAL_CFLAGS += \
        -Wall \
        -g \
        -Wno-long-long  \
        -pedantic \
        -fomit-frame-pointer \
        -DEFTE_INSTALL_DIR=\"$(LOCAL_PATH)../output\" \
	-UUSE_LOCALE \
        -UHAVE_LOCALE_H \
        -DUNIX \
        -UUSE_GPM \
        -DHAVE_STRICMP 
LOCAL_LDLIBS := -L$(NCURSESLIB)
LOCAL_LDLIBS += -g
LOCAL_LDLIBS += -llog
LOCAL_LDLIBS += -lstdc++
LOCAL_LDLIBS += -lncurses
LOCAL_STATIC_LIBRARIES += ncurses 
LOCAL_MODULE := nefte
LOCAL_MODULE_PATH := $(TARGET_OUT_OPTIONAL_EXECUTABLES)
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
#APP_ABI:= x86
LOCAL_SRC_FILES:= bin2c.cpp
LOCAL_C_INCLUDES := bionic
LOCAL_C_INCLUDES += $(NCURSESINC)
LOCAL_CFLAGS += \
        -Wall \
        -g \
        -Wno-long-long  \
        -pedantic \
        -fomit-frame-pointer \
        -DEFTE_INSTALL_DIR=\"$(LOCAL_PATH)../output\" \
	-UUSE_LOCALE \
        -UHAVE_LOCALE_H \
        -DUNIX \
        -UUSE_GPM \
        -DHAVE_STRICMP 
LOCAL_LDLIBS := -L$(NCURSESLIB)
LOCAL_LDLIBS += -g
LOCAL_LDLIBS += -llog
LOCAL_LDLIBS += -lstdc++
LOCAL_STATIC_LIBRARIES += ncurses 
LOCAL_MODULE := bin2c
LOCAL_MODULE_PATH := $(TARGET_OUT_OPTIONAL_EXECUTABLES)
include $(BUILD_EXECUTABLE)

all: bin2c defcfg.h nefte

# $(info $(shell (adb push ../libs/armeabi-v7a/nefte /data/local/tmp/xbin)))
# $(info $(shell (adb shell mkdir /data/local/tmp/xbin/.efte)))
# $(info $(shell (adb push ~/bin/efte.config.tz /data/local/tmp/xbin/)))
# $(info $(shell (adb shell cd /data/local/tmp/xbin && ./busybox tar xvzf ./efte.config.tz . )))

defcfg.h: bin2c simple.fte
	$(info $(shell (../libs/x86/bin2c simple.fte > defcfg.h)))

#config.h:
	#sed s%\$$\{CMAKE_INSTALL_PREFIX\}%/data/local/tmp/xbin%g < config.h.in > config.h

