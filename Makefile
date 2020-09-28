
BUILD_DIR = build
SRC_DIR   = src
BIN_DIR   = bin

CXX = g++
FLAGS = -std=c++17
CFLAGS = -g -fopenmp -march=native -mtune=native
LDFLAGS =
DEBUGFLAGS = -O0 -D _DEBUG
RELEASEFLAGS = -O3 -D NDEBUG

BUILDFLAGS = $(RELEASEFLAGS)

# SOURCES_TMP += scratch.cpp
SOURCES_TMP += prep.cpp
SOURCES_TMP += heugcol.cpp
SOURCES_TMP += Graph.cpp

SOURCES = $(addprefix $(SRC_DIR)/, $(SOURCES_TMP))

OBJECTS_TMP=$(SOURCES_TMP:.cpp=.o)
OBJECTS=$(addprefix $(BUILD_DIR)/, $(OBJECTS_TMP))

DEPFILES_TMP := $(SOURCES_TMP:.cpp=.d)
DEPFILES = $(addprefix $(BUILD_DIR)/, $(DEPFILES_TMP))

EXEC_NAMES = gcp

EXECS = $(addprefix $(BIN_DIR)/, $(EXEC_NAMES))

INC_DIRS += $(SRC_DIR)
INC_DIRS += /opt/gurobi902/linux64/include
INC_DIRS += /opt/boost_1_73_0
#INC_DIRS += /opt/fmt-6.2.1/include
#INC_DIRS += /opt/eigen-3.3.7

INC_PARAMS = $(addprefix -I, $(INC_DIRS))

LIB_DIRS += /opt/gurobi902/linux64/lib
#LIB_DIRS += /opt/fmt-6.2.1
LIB_DIRS += /usr/local/lib

LIB_DIR_PARAMS = $(addprefix -L, $(LIB_DIRS))

LIBS_TMP += boost_system
LIBS_TMP += boost_filesystem
LIBS_TMP += gurobi_c++
LIBS_TMP += gurobi90
LIBS_TMP += m
#LIBS_TMP += fmt

LIB_PARAMS = $(addprefix -l, $(LIBS_TMP))

VPATH = src

all: $(EXECS)

.PHONY : clean

clean:
	rm -rf $(EXECS) $(BUILD_DIR) $(BIN_DIR)
	
# $(BIN_DIR)/gcp: $(SOURCES) Makefile | ${BIN_DIR}
# 	$(CXX) $(BUILDFLAGS) $(CFLAGS) \
# 	${SOURCES} \
# 	$(LDFLAGS) $(LIB_PARAMS) \
# 	$(INC_PARAMS) \
# 	$(LIB_DIR_PARAMS) \
# 	-o $@	

$(BIN_DIR)/gcp: $(SRC_DIR)/scratch.cpp ${OBJECTS} Makefile | ${BIN_DIR}
	$(CXX) $(BUILDFLAGS) $(CFLAGS) \
	${OBJECTS} $(SRC_DIR)/scratch.cpp \
	$(LDFLAGS) $(LIB_PARAMS) \
	$(INC_PARAMS) \
	$(LIB_DIR_PARAMS) \
	-o $@
	
# $(BIN_DIR)/gcp: ${OBJECTS} Makefile | ${BIN_DIR}
# 	$(CXX) $(BUILDFLAGS) $(CFLAGS) \
# 	${OBJECTS} \
# 	$(LDFLAGS) $(LIB_PARAMS) \
# 	$(INC_PARAMS) \
# 	$(LIB_DIR_PARAMS) \
# 	-o $@	

# $(BUILD_DIR)/scratch.o: $(SRC_DIR)/scratch.cpp $(BUILD_DIR)/scratch.d Makefile | $(BUILD_DIR)
# 	$(CXX) $(BUILDFLAGS) $(CFLAGS) \
# 	$(LDFLAGS) $(LIB_PARAMS) \
# 	$(INC_PARAMS) \
# 	$(LIB_DIR_PARAMS) \
# 	-c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/%.h $(BUILD_DIR)/%.d Makefile | $(BUILD_DIR)
	$(CXX) $(BUILDFLAGS) $(CFLAGS) \
	$(LDFLAGS) $(LIB_PARAMS) \
	$(INC_PARAMS) \
	$(LIB_DIR_PARAMS) \
	-c $< -o $@

$(BUILD_DIR)/%.d: $(SRC_DIR)/%.cpp Makefile | $(BUILD_DIR)
	$(CXX) $(BUILDFLAGS) $(CFLAGS) \
	$(LDFLAGS) $(LIB_PARAMS) \
	$(INC_PARAMS) \
	$(LIB_DIR_PARAMS) \
	-MM -MT '$(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$<)' $< -MF $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)	
