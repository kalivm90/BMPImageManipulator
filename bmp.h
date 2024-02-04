#pragma pack(2)
typedef struct {
    char signature[2];
    unsigned int size;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int offset;
} BMPHeader;

typedef struct {
    unsigned int size;
    int width;
    int height;
    unsigned short planes;
    unsigned short bitsPerPixel;
    unsigned int compression;
    unsigned int imageSize;
    int xPixelsPerMeter;
    int yPixelsPerMeter;
    unsigned int colorsUsed;
    unsigned int colorsImportant;
} BMPInfoHeader;

// For <= 8 bits of colors
typedef struct {
    unsigned char Red;
    unsigned char Green;
    unsigned char Blue;
    unsigned char reserved;
} BMPColorTable;

// For true color pixel
typedef struct {
    unsigned char Red;
    unsigned char Green;
    unsigned char Blue;
} BMPPixel;
#pragma pack()