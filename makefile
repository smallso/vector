#
# Vector Helper Makefile
# --------
# Version: 1.0.0 Modify 2018/9/23 01:59
# Website: https://www.xiaoyy.org/
# --------
# Copyright 2018 SmallSO Studios.
# Feel free to modify this Makefile and package it again.
#

FILENAME:= libvector
TARGET  := bin/$(FILENAME).so
TARGET_S:= bin/$(FILENAME).a

DEFINES := -D "NDEBUG"

SOURCE = src/vector_helper.c
OBJS = src/vector_helper.o

.PHONY : clean uninstall

main: $(TARGET)  
	@ar -rcs $(TARGET_S) $(OBJS)
	@echo "vector_helper: make static link library successfully, at $(TARGET_S)"
	@echo "vector_helper: make dynamic link library successfully, at $(TARGET)"

clean:
	@rm -f src/*.o
	@rm -f $(TARGET) $(TARGET_S)
	@echo "vector_helper: cleanup successfully"

install:
  ifeq (,$(wildcard /usr/local/lib/))
	@mkdir /usr/local/lib/
  endif
  ifeq (,$(wildcard /usr/local/lib64/))
	@mkdir /usr/local/lib64/
  endif
  ifeq (,$(wildcard /etc/ld.so.conf.d/))
	@mkdir /etc/ld.so.conf.d
  endif
  ifneq (,$(wildcard /etc/ld.so.conf.d/libvector.conf))
	@rm -f /etc/ld.so.conf.d/libvector.conf
  endif
  ifeq ($(shell getconf LONG_BIT),32)
	@cp $(TARGET) /usr/local/lib/
	@cp $(TARGET_S) /usr/local/lib/
	@echo "/usr/local/lib" > /etc/ld.so.conf.d/libvector.conf
  else
	@cp $(TARGET) /usr/local/lib64/
	@cp $(TARGET_S) /usr/local/lib64/
	@echo "/usr/local/lib64" > /etc/ld.so.conf.d/libvector.conf
  endif
	@cp src/vector_helper.h /usr/local/include/
	@ldconfig
	@echo "vector_helper: installation completed successfully"

uninstall:
	@rm -f /etc/ld.so.conf.d/libvector.conf
	@rm -f /usr/local/include/vector_helper.h
  ifeq ($(shell getconf LONG_BIT),32)
	@rm -f /usr/local/lib/$(FILENAME).a
	@rm -f /usr/local/lib/$(FILENAME).so
  else
	@rm -f /usr/local/lib64/$(FILENAME).a
	@rm -f /usr/local/lib64/$(FILENAME).so
  endif
	@ldconfig
	@echo "vector_helper: successfully removed installation"

$(TARGET) : $(OBJS)
	@gcc -o $(TARGET) -Wl,-z,relro -Wl,-z,now -Wl,--strip-all -shared -Wl,-z,noexecstack -Wl,--no-undefined $(OBJS)

$(OBJS) : $(SOURCE)
	@gcc -c $(SOURCE) -Wall -Werror -std=c11 -fPIC -fomit-frame-pointer -funroll-all-loops -g0 -fno-exceptions -O3 $(DEFINES) -o $(OBJS)
