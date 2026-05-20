RAYLIB_DIR  := external/raylib
RAYLIB_SRC  := $(RAYLIB_DIR)/src
RAYLIB_LIB  := $(RAYLIB_SRC)/libraylib.a

CFLAGS      := -std=c11 -Wall -Wextra -pedantic -O2
CFLAGS      += -I$(RAYLIB_SRC) -I$(RAYLIB_DIR)/src/external

LDFLAGS     := -L$(RAYLIB_SRC) -lraylib -lm -lpthread -ldl -lrt -lX11

.PHONY: all clean run

all: gnist

$(RAYLIB_LIB):
	@echo ">> cloning raylib..."
	git clone --depth 1 https://github.com/raysan5/raylib.git $(RAYLIB_DIR)
	@echo ">> building raylib..."
	$(MAKE) -C $(RAYLIB_SRC) PLATFORM=PLATFORM_DESKTOP

gnist: $(RAYLIB_LIB) main.c
	$(CC) $(CFLAGS) -o $@ main.c $(LDFLAGS)

run: gnist
	./gnist

clean:
	rm -f gnist

distclean: clean
	rm -rf $(RAYLIB_DIR)
