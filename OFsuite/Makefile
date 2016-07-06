CC=gcc
DIR_SRC=./src
DIR_BIN=./test_cases
CFLAGS = -Wall -fgnu89-inline

SRC=$(wildcard *.c)
SRC+=$(wildcard ${DIR_SRC}/*.c)
OBJ=$(patsubst %.c,%.o,${SRC})

TARGET_SRC=$(wildcard ${DIR_BIN}/*.c)
TARGET_OBJ=$(patsubst %.c,%.o,${TARGET_SRC})
BIN_TARGET=$(patsubst %.c,%,${TARGET_SRC})

${BIN_TARGET}:${OBJ}
	$(foreach target,${BIN_TARGET}, $(CC) ${CFLAGS} ${target}.c ${OBJ} -o ${target} -lpthread -lm -lrt -lpython2.7 -lssl -lcrypto -lreadline;)

.PHONY:clean

clean:
	find . -name *.o -exec rm -rf {} \;
	find ./src -name *.o -exec rm -rf {} \;
	find ./test_cases -name *.o -exec rm -rf {} \;
	rm ${BIN_TARGET}
