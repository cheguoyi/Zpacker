NAME = Zpacker

SRC = main.c  hello.c

SRCDIR = src

OBJDIR = obj

OBJ = $(addprefix ${OBJDIR}/,$(SRC:.c=.o))

CC =gcc

CFLAGS=

LDFLAGS = -I includes


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
	@${CC} ${CFLAGS} -o $@ ${OBJ}
	@echo ${G}Success"   "[${NAME}]${X}

${OBJDIR}/%.o: ${SRCDIR}/%.c
	@echo ${B} compiling [$@]...
	@/bin/mkdir -p ${OBJDIR}
	@${CC} ${CFLAGS} ${LDFLAGS} -c -o $@ $<
	@echo ${G}Success"   "[$@]${X}
	@printf ${UP}
	
############################## GENERAL #########################################


.PHONY: all clean 
clean:
	@echo ${R}Cleaning"  "[objs]...${X}
	@/bin/rm -Rf ${OBJDIR}
	@/bin/rm -Rf ${NAME}
