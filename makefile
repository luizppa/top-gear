CC=g++
CFLAGS=-Wall -Wextra

TARGET_NAME=./top-gear
BUILD_PATH=./build
SOURCE_PATH=./src
LIB_PATH=./include
OUTPUT_PATH=./output

SOURCE_EXT := c
SOURCES := $(shell find $(SOURCE_PATH) -path '*.$(SOURCE_EXT)')
OBJECTS := $(patsubst $(SOURCE_PATH)/%,$(BUILD_PATH)/%,$(SOURCES:.$(SOURCE_EXT)=.o))

ALLEGRO_LIBS= -lallegro -lallegro_acodec -lallegro_audio -lallegro_video -lallegro_color -lallegro_dialog -lallegro_image -lallegro_main -lallegro_memfile -lallegro_physfs -lallegro_primitives -lallegro_ttf -lallegro_font -lallegro
ALLEGRO_INCLUDE=-I allegro-5.0.10/allegro-5.0.10-mingw-4.7.0/include
ALLEGRO_BIN=-L allegro-5.0.10/allegro-5.0.10-mingw-4.7.0/bin
ALLEGRO_CONFIG=$(ALLEGRO_INCLUDE) $(ALLEGRO_BIN) $(ALLEGRO_LIBS)
ALEGRO_MONOLITH=allegro-5.0.10/allegro-5.0.10-mingw-4.7.0/lib/liballegro-5.0.10-monolith-mt.a


all: $(TARGET_NAME)

$(BUILD_PATH)/%.o: $(SOURCE_PATH)/%.$(SOURCE_EXT) $(LIB_PATH)/%.h
	mkdir -p $(BUILD_PATH)
	$(CC) -c $(CFLAGS) $(ALLEGRO_CONFIG) -o $@ $<

$(BUILD_PATH)/main.o: main.c

$(TARGET_NAME): $(OBJECTS) $(BUILD_PATH)/main.o
	$(CC) $(CFLAGS) -o $(TARGET_NAME) $(BUILD_PATH)/main.o $(OBJECTS) $(ALEGRO_MONOLITH)

$(TARGET_NAME):
	$(CC) -C $(CFLAGS) $(ALLEGRO_INCLUDE) 
	$(CC) -o $(TARGET_NAME) $(CFLAGS) main.c helpers/maps.c helpers/object.c helpers/utils.c helpers/car.c helpers/game.c helpers/menus.c helpers/display.c helpers/colors.c helpers/environment.c helpers/sounds.c helpers/fonts.c $(ALEGRO_MONOLITH) -lm $(ALLEGRO_CONFIG)

clean:
	rm -rf $(BUILD_PATH)/*
	rm $(TARGET_NAME)

run:
	$(TARGET_NAME)

mem:
	valgrind --leak-check=full --show-leak-kinds=all $(TARGET_NAME) > ./output/mem.log