PROJECT=drive.detect
PLATFORM=apple2enh
CC=cl65
FUJINET_LIB_DIR=$(HOME)/source/fujinet/fujinet-lib
CFLAGS=-O -I$(FUJINET_LIB_DIR)
LIBS=-L $(FUJINET_LIB_DIR)/build fujinet.lib

define single-to-double
  unsingle $< && mv $<.ad $@ && mv .AppleDouble/$<.ad .AppleDouble/$@
endef

define link-bin
  $(CC) -m $@.map -vm -t $(PLATFORM) $(LDFLAGS) $^ $(LIBS) -o $@
endef

all: $(PROJECT)

$(PROJECT): $(PROJECT).bin
	$(single-to-double)
$(PROJECT).bin: $(PROJECT).o
	$(link-bin)

$(PROJECT).o: $(PROJECT).c

.c.o:
	$(CC) -l $(basename $@).lst -c $(CFLAGS) -t $(PLATFORM) $< -o $@

.s.o:
	$(CC) -l $(basename $@).lst -c -t $(PLATFORM) $< -o $@

.bin.o:
	$(CC) -t $(PLATFORM) $< -o $@
