CC=g++
CFLAGS=-O3  
CFLAGS=-ggdb
SUFFIX=cpp
INCLUDE= -I src/ -I ../include
LIBS=-L ../lib -lleveldb -lDAC -pthread # -lmarisa -lsdsl
BIN=libutils.a
RUN=dfuds_trie

######################## variable

D_TEST=test
D_TOOL=tool
D_SRC=src
D_OBJ=obj
D_BIN=bin

DIR_TO_CREATE=${D_OBJ} ${D_BIN}
VPATH=${D_SRC}/:${D_OBJ}/:${D_TEST}/:${D_TOOL}

SRC_TEST=${wildcard ${D_TEST}/*.${SUFFIX}}
OBJ_TEST=${patsubst ${D_TEST}/%.${SUFFIX}, ${D_OBJ}/%.o, ${SRC_TEST}}
BIN_TEST=${D_BIN}/unit_test

SRC_TOOL=${wildcard ${D_TOOL}/*.${SUFFIX}}
OBJ_TOOL=${patsubst ${D_TOOL}/%.${SUFFIX}, ${D_OBJ}/%.o, ${SRC_TOOL}}
BIN_TOOL=${patsubst ${D_TOOL}/%.${SUFFIX}, ${D_BIN}/%, ${SRC_TOOL}}

SRC=$(wildcard ${D_SRC}/*.${SUFFIX})
OBJ=${patsubst ${D_SRC}/%.${SUFFIX}, ${D_OBJ}/%.o, ${SRC}}

HEADERS=${wildcard ${D_SRC}/*.h} ${wildcard ${D_SRC}/*.hxx}
INSTALLED_HEADERS=${patsubst ${D_SRC}/%, ../include/%, ${HEADERS}}

OBJ_ALL=${OBJ_TEST} ${OBJ_TOOL} ${OBJ}
BIN_ALL=${BIN} ${BIN_TEST} ${BIN_TOOL}

######################## phony

all: printSeparator ${D_BIN} ${BIN_ALL} 
.PHONY: all

# clean all build output files.
clean:
	rm -rf ${D_OBJ} ${D_BIN} *.o *.a
.PHONY: clean

# run test cases.
test: all
	./${BIN_TEST}
.PHONY: test

# short cut to run
run: all
	./${D_BIN}/${RUN}

# install
install: all
	cp ${HEADERS} ../include/
	cp ${BIN} ../lib/
.PHONY: install

# uninstall
uninstall:
	rm -f ${INSTALLED_HEADERS}
	rm -f ../lib/${BIN}
.PHONY: uninstall

# just print several dummy lines.
printSeparator:
	@echo
	@echo ====== dependency generation finished =======
	@echo
.PHONY: printSeparator

######################## build

# build the static library.
${BIN}: ${OBJ}
	ar cru $@ $^

# unit test
${BIN_TEST}: ${OBJ_TEST} ${BIN}
	${CC} -o $@  $^  ${LIBS} -lgtest_main -lgtest

# build tools
${D_BIN}/%: %.o ${BIN}
	${CC} -o $@  $^  ${LIBS} 

# generate dependency files.
${D_OBJ}/%.d : %.${SUFFIX}
	@mkdir -p ${D_OBJ}
	${CC} > $@    $< -MP -MM -MT ${@D}/${*F}.o ${INCLUDE}
include $(OBJ_ALL:%.o=%.d)

# compile src files.
${D_OBJ}/%.o : %.${SUFFIX}
	$(CC) $< -o $@ -c ${INCLUDE} ${CFLAGS}

# create dirs.
${DIR_TO_CREATE}:
	mkdir -p $@
