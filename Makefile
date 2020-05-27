NAME = Zpacker

SRC = main.c endian.c file.c iterators.c packer.c safe.c elf64_viewer.c\
	  pack_step/adjust_references.c \
	  pack_step/adjust_sizes.c \
	  pack_step/change_entry.c \
	  pack_step/copy_clone_file.c \
	  pack_step/define_shift.c \
	  pack_step/find_entry.c \
	  pack_step/load_code.c \
	  encrypt.s \
	  load_code.s


SRCDIR = src

OBJDIR = obj

OBJC = $(addprefix ${OBJDIR}/,$(SRC:.c=.o))

OBJ = $(OBJC:.s=.o)

CC = gcc

DEP = $(addprefix ${OBJDIR}/, $(SRC:.c=.d))

CFLAGS=  -Wall -Wextra -g -MMD 

AS = nasm

ASFLAGS= -f elf64 -g

LDFLAGS = -I includes/


############################## COLORS ##########################################

BY = "\033[33;1m"
BR = "\033[31;1m"
BG = "\033[32;1m"
BB = "\033[34;1m"
BM = "\033[35;1m"
BC = "\033[36;1m"
BW = "\033[37;1m"
Y = "\033[33m"
R = "\033[31m"
G = "\033[32m"
B = "\033[34m"
M = "\033[35m"
C = "\033[36m"
W = "\033[0m""\033[32;1m"
WR = "\033[0m""\033[31;5m"
WY = "\033[0m""\033[33;5m"
X = "\033[0m"
UP = "\033[A"
CUT = "\033[K"

############################## RULES ###########################################
all:${NAME}

${NAME}:${OBJ}
	@echo ${B} compiling and linking [${OBJ}]...
	@${CC} ${CFLAGS} ${LDFLAGS}  -o $@ ${OBJ}
	@echo ${G}Success"   "[${NAME}]${X}

${OBJDIR}/%.o: ${SRCDIR}/%.s
	@echo ${Y}Compiling [$@]...${X}
	@/bin/mkdir -p ${OBJDIR} 
	@${AS} ${ASFLAGS} -o $@ $< 
	@printf ${UP}

${OBJDIR}/%.o: ${SRCDIR}/%.c
	@echo ${B} compiling [$@]...
	@/bin/mkdir -p ${OBJDIR} ${OBJDIR}/pack_step
	@${CC} ${CFLAGS} ${LDFLAGS} -c -o $@ $<
	@echo ${G}Success"   "[$@]${X}
	@printf ${UP}

############################## GENERAL #########################################

exist = $(shell if [ -f $(NAME) ]; then echo "exist"; else echo "notexist"; fi;)

.PHONY: all clean 
clean:
	@echo ${R}Cleaning"  "[${OBJ}]...${X}
	@/bin/rm -Rf ${OBJDIR}
ifeq (${exist}, exist)
	@/bin/rm ${NAME}
endif

-include ${DEP}
