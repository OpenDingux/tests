ifdef V
	CMD:=
	SUM:=@\#
else
	CMD:=@
	SUM:=@echo
endif

TARGETS := test_vpu.elf

EXTRA_OBJS := head.o header.o printf.o trace.o vring.o

CROSS_COMPILE ?= mipsel-linux-
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
NM = $(CROSS_COMPILE)nm

CFLAGS	:= -Wall -O2 -fno-pic -mno-abicalls -mno-check-zero-division \
           -ffreestanding -flto -I include -mno-llsc \
		   -mno-flush-func -mno-synci #-mno-float
LDFLAGS	:= -nostdlib -EL -T vpu.ld

.PHONY: all

all: $(TARGETS)

%.elf: %.o $(EXTRA_OBJS)
	@mkdir -p $(@D)
	$(SUM) "  LD      $@"
	$(CMD)$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

%.o: %.c
	@mkdir -p $(@D)
	$(SUM) "  CC      $@"
	$(CMD)$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

%.o: %.S
	@mkdir -p $(@D)
	$(SUM) "  CC      $@"
	$(CMD)$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(TARGETS) $(TARGETS:.elf=.o) $(EXTRA_OBJS)
