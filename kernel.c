#include <stdbool.h>
#include <stddef.h>
#include <stdint.h> // this line is subject to quick replacement in a dedicated types header.

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

#if defined(__linux__)
#error "You are not using a cross-compiler. Nanami/OS requires an i686-elf cross-compiler."
#endif

// hardware text mode colours
enum vgaColour {
	VGA_COLOUR_BLACK        = 0,
	VGA_COLOUR_BLUE         = 1,
	VGA_COLOUR_GREEN        = 2,
	VGA_COLOUR_CYAN         = 3,
	VGA_COLOUR_RED          = 4,
	VGA_COLOUR_MAGENTA      = 5,
	VGA_COLOUR_BROWN        = 6,
	VGA_COLOUR_LIGHTGREY    = 7,
	VGA_COLOUR_DRAKGREY     = 8,
	VGA_COLOUR_LIGHTBLUE    = 9,
	VGA_COLOUR_LIGHTGREEN   = 10,
	VGA_COLOUR_LIGHTCYAN    = 11,
	VGA_COLOUR_LIGHTRED     = 12,
	VGA_COLOUR_LIGHTMAGENTA = 13,
	VGA_COLOUR_LIGHTBROWN   = 14,
	VGA_COLOUR_WHITE        = 15,
};

static inline uint8_t vgaEntryColour(enum vgaColour foreground, enum vgaColour background)
{
	return foreground | background << 4;
}

static inline uint16_t vgaEntry(unsigned char uc, uint8_t colour)
{
	return (uint16_t) uc | (uint16_t) colour << 8;
}

size_t strlen(const char* str)
{
	size_t len = 0;
	while (str[len]) len++;
	return len;
}

size_t   terminalRow;
size_t   terminalColumn;
uint8_t  terminalColour;
uint16_t* terminalBuffer = (uint16_t*)VGA_MEMORY;

void TerminalInitialise(void)
{
	terminalRow = 0;
	terminalColumn = 0;
	terminalColour = vgaEntryColour(VGA_COLOUR_LIGHTGREY, VGA_COLOUR_BLACK);

	for (size_t y = 0; y < VGA_HEIGHT; y++)
	{
		for (size_t x = 0; x < VGA_WIDTH; x++)
		{
			const size_t index = y * VGA_WIDTH + x;
			terminalBuffer[index] = vgaEntry(' ', terminalColour);
		}
	}
}

void TerminalColourSet(uint8_t colour)
{
	terminalColour = colour;
}

void TerminalPutEntryAt(char c, uint8_t colour, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminalBuffer[index] = vgaEntry(c, colour);
}

void TerminalPutChar(char c)
{
	TerminalPutEntryAt(c, terminalColour, terminalColumn, terminalRow);
	if (++terminalColumn == VGA_WIDTH)
	{
		terminalColumn = 0;
		if (++terminalRow == VGA_HEIGHT) terminalRow = 0;
	}
}

void TerminalWrite(const char* data, size_t size)
{
	for (size_t i = 0; i < size; i++) TerminalPutChar(data[i]);
}

void TerminalWriteStr(const char* data)
{
	TerminalWrite(data, strlen(data));
}

void kernel_main(void)
{
	TerminalInitialise();

	TerminalWriteStr("Nanami/OS build 1!!!\nHello world!\n");
}

