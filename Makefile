# Makefile

UNAME := $(shell uname)

ifeq ($(UNAME), Darwin)
	CC = g++
	STD = -std=c++11 -stdlib=libc++
else
	CC = g++
	STD = -std=c++11
endif

TARGET = inverse-code-3

SRC_PATH = src

TEST_PATH = tests

BUILD_PATH = build


#OPTIMIZATION = -O4
OPTIMIZATION = -O0 -g
CFLAGS = -Wall -Wfatal-errors -Wno-missing-braces $(OPTIMIZATION) $(STD) -Isrc/
LIBS_PREFIX =
LIBS_SUFFIX =

HEADERS = $(shell find $(SRC_PATH) -type f \( \( -name "*.h" -o -name "*.hpp" -o -name "*.hxx" \) \))
SOURCES = $(shell find $(SRC_PATH) -type f \( \( -name "*.c" -o -name "*.cpp" -o -name "*.cxx" \) \))
SRC_OBJECTS = $(patsubst %.cpp, $(BUILD_PATH)/%.o, $(SOURCES))

TESTS = $(shell find $(TEST_PATH) -type f \( -name "*.c" -o -name "*.cpp" -o -name "*.cxx" \))
TEST_OBJECTS = $(patsubst %.cpp, $(BUILD_PATH)/%.o, $(TESTS))

.PHONY:	default all clean

.PRECIOUS:	$(TARGET) $(SRC_OBJECTS) $(TEST_OBJECTS)

default:	$(TARGET)
all:	default

$(BUILD_PATH)/%.o:	%.cpp $(HEADERS)
	mkdir -p "$(@D)"
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET):	$(SRC_OBJECTS)
	$(CC) $(CFLAGS) $(LIBS_PREFIX) $^ -o $@ $(LIBS_SUFFIX)

test:	$(TEST_OBJECTS) $(filter-out build/src/main.o, $(SRC_OBJECTS))
	$(CC) $(CFLAGS) $(LIBS_PREFIX) $^ -o $@ $(LIBS_SUFFIX)

clean:
	-rm -rf $(BUILD_PATH) $(TARGET)
