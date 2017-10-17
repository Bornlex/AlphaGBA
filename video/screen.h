#ifndef SCREEN_H
# define SCREEN_H

# define SCREEN_HEIGHT 160
# define SCREEN_WIDTH  240

# define MEM_IO   0x04000000
# define MEM_PAL  0x05000000
# define MEM_VRAM 0x06000000
# define MEM_OAM  0x07000000

# define oam_mem            ((volatile obj_attrs *) MEM_OAM)
# define tile_mem           ((volatile tile_block *) MEM_VRAM)
# define object_palette_mem ((volatile rgb15 *) (MEM_PAL + 0x200))

# define REG_DISPLAY        (*((volatile uint32 *)(MEM_IO)))
# define REG_DISPLAY_VCOUNT (*((volatile uint32 *)(MEM_IO + 0x0006)))
# define REG_KEY_INPUT      (*((volatile uint32 *)(MEM_IO + 0x0130)))

# define KEY_UP   0x0040
# define KEY_DOWN 0x0080
# define KEY_ANY  0x03ff

# define OBJECT_ATTR0_Y_MASK 0x0ff
# define OBJECT_ATTR1_X_MASK 0x1ff

# define VIDEO_MODE 0x03
# define BACKGROUND 0x04

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef uint16 rgb15;
typedef struct obj_attrs {
  uint16 attr0;
  uint16 attr1;
  uint16 attr2;
  uint16 pad;
} __attribute__((packed, aligned(4))) obj_attrs;

typedef uint32    tile_4bpp[8];
typedef tile_4bpp tile_block[512];

//?BBBBBGGGGGRRRRR
static inline rgb15 RGB15(int r, int g, int b)
{
  return (r | (g << 5) | (b << 10));
}

//set the position of an object to specified x and y coordinates
static inline int set_object_position(volatile obj_attrs *object, int x, int y)
{
  object->attr0 = ((object->attr0 & ~OBJECT_ATTR0_Y_MASK) | (y & OBJECT_ATTR0_Y_MASK));
  object->attr1 = ((object->attr1 & ~OBJECT_ATTR1_X_MASK) | (x & OBJECT_ATTR1_X_MASK));
}

static inline int clamp(int value, int min, int max)
{
  return (value < min ? min : (value > max ? max : value));
}

#endif
