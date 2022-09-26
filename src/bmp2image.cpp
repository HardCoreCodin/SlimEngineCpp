#ifdef COMPILER_CLANG
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#else
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "./slim/core/base.h"
#include "./slim/platforms/win32_base.h"

unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
    void *filePtr;  //our file pointer
    BITMAPFILEHEADER bitmapFileHeader;  //our bitmap file header
    unsigned char *bitmapImage;  //store image data

    //open file in read binary mode
    filePtr = win32_openFileForReading(filename);
    if (filePtr == nullptr)
        return nullptr;

    //read the bitmap file header
    win32_readFromFile(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), filePtr);

    //verify that this is a .BMP file by checking bitmap id
    if (bitmapFileHeader.bfType !=0x4D42)
    {
        win32_closeFile(filePtr);
        return nullptr;
    }

    //read the bitmap info header
    win32_readFromFile(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), filePtr);

    //move file pointer to the beginning of bitmap data
    SetFilePointer(filePtr, (LONG)bitmapFileHeader.bfOffBits, nullptr, FILE_BEGIN);

    u32 bytes_per_pixel = bitmapInfoHeader->biBitCount / 8;
    if (bitmapInfoHeader->biSizeImage == 0) {
        bitmapInfoHeader->biSizeImage = bytes_per_pixel;
        bitmapInfoHeader->biSizeImage *= bitmapInfoHeader->biWidth;
        bitmapInfoHeader->biSizeImage *= bitmapInfoHeader->biHeight < 0 ? -bitmapInfoHeader->biHeight : bitmapInfoHeader->biHeight;
    }
    //allocate enough memory for the bitmap image data
    bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);

    //verify memory allocation
    if (!bitmapImage) {
        free(bitmapImage);
        win32_closeFile(filePtr);
        return nullptr;
    }

    //read in the bitmap image data
    win32_readFromFile(bitmapImage, bitmapInfoHeader->biSizeImage, filePtr);

    //close file and return bitmap image data
    win32_closeFile(filePtr);

    if (bitmapInfoHeader->biHeight > 0) {
        unsigned char* flipped_bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);
        u32 stride = bitmapInfoHeader->biWidth * bytes_per_pixel;
        u32 trg_offset = (bitmapInfoHeader->biHeight - 1) * stride;
        u32 src_offset = 0;
        for (i32 y = 0; y < bitmapInfoHeader->biHeight; y++) {
            for (i32 x = 0; x < bitmapInfoHeader->biWidth; x++) {
                flipped_bitmapImage[trg_offset++] = bitmapImage[src_offset++];
                flipped_bitmapImage[trg_offset++] = bitmapImage[src_offset++];
                flipped_bitmapImage[trg_offset++] = bitmapImage[src_offset++];
            }
            trg_offset -= stride;
            trg_offset -= stride;
        }
        free(bitmapImage);
        return flipped_bitmapImage;
    }

    return bitmapImage;
}

int bmp2image(char* bmp_file_path, char* image_file_path, float gamma = 2.2f) {
    BITMAPINFOHEADER bitmapInfoHeader;
    u8* component = LoadBitmapFile(bmp_file_path, &bitmapInfoHeader);

    Image image{};
    image.width = (u16)bitmapInfoHeader.biWidth;
    image.height = (u16)bitmapInfoHeader.biHeight;
    image.pixels = new Pixel[image.width * image.height];
    Pixel* pixel = image.pixels;
    for (u16 y = 0; y < image.height; y++) {
        for (u16 x = 0; x < image.width; x++, pixel++) {
            pixel->color.b = powf((f32)(*(component++)) * COLOR_COMPONENT_TO_FLOAT, gamma);
            pixel->color.g = powf((f32)(*(component++)) * COLOR_COMPONENT_TO_FLOAT, gamma);
            pixel->color.r = powf((f32)(*(component++)) * COLOR_COMPONENT_TO_FLOAT, gamma);
            if (bitmapInfoHeader.biBitCount == 32)
                pixel->opacity = (f32)(*(component++)) * COLOR_COMPONENT_TO_FLOAT;
        }
    }

    void* file = win32_openFileForWriting(image_file_path);

    win32_writeToFile(&image.width,  sizeof(u16), file);
    win32_writeToFile(&image.height,  sizeof(u16), file);
    win32_writeToFile(image.pixels,  sizeof(Pixel) * image.width * image.height, file);

    win32_closeFile(file);

    return 0;
}

int main(int argc, char *argv[]) {
    return bmp2image(argv[1], argv[2], (argc == 4 && argv[3][0] == '-' && argv[3][1] == 'l') ? 1.0f : 2.2f);
}