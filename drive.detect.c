#include <apple2/src/include/fujinet-bus-apple2.h>
#include <stdio.h>
#include <peekpoke.h>

#define IWM_CTRL_CLEAR_ENSEEN   0x08
#define IWM_STATUS_ENSEEN       0x08

#define DISKII_SEL_DRIVE1	0x0a
#define DISKII_SEL_DRIVE2	0x0b
#define DISKII_MOTOR_ON		0x09
#define DISKII_MOTOR_OFF	0x08

uint8_t find_diskii(uint8_t start_slot)
{
  uint16_t offset;
  uint8_t idx;


  for (idx = start_slot; idx; idx--) {
    offset = 0xC000 + (idx << 8);
    if (PEEK(offset + 1) == 0x20 && PEEK(offset + 3) == 0 && PEEK(offset + 5) == 3
	&& PEEK(offset + 255) == 0)
      return idx;
  }

  return idx;
}

void enable_diskii(uint8_t slot, uint8_t drive)
{
  uint16_t offset;


  offset = 0xC080 + (slot << 4);
  POKE(offset + DISKII_SEL_DRIVE1 + ((drive - 1) & 1), 0);
  POKE(offset + DISKII_MOTOR_ON, 0);
  POKE(offset + DISKII_MOTOR_OFF, 0);
  return;
}

void main()
{
  uint8_t dev_id, slot, drive, status;
  int8_t err;


  dev_id = sp_get_fuji_id();
  printf("Fuji ID: %i\n", dev_id);

  if (dev_id) {
    for (slot = 7; slot; slot--) {
      slot = find_diskii(slot);
      if (!slot)
	break;

      for (drive = 1; drive <= 2; drive++) {
	sp_control(dev_id, IWM_CTRL_CLEAR_ENSEEN);
	enable_diskii(slot, drive);
	err = sp_status(dev_id, IWM_STATUS_ENSEEN);
	status = sp_payload[0];
	printf("Enable S%i,D%i: %02x\n", slot, drive, status);
      }
    }
  }
  
  return;
}
