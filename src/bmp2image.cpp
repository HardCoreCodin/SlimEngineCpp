#include "./slim/platforms/win32_bitmap.h"
#include "./slim/serialization/image.h"

int main(int argc, char *argv[]) {
    ImageInfo info;

    char* bitmap_file_path = argv[1];
    char* image_file_path = argv[2];

    bool byte_color = false;
    for (u8 i = 3; i < (u8)argc; i++)
        if (     argv[i][0] == '-' && argv[i][1] == 'f') info.flags.flip = true;
        else if (argv[i][0] == '-' && argv[i][1] == 'c') info.flags.channel = true;
        else if (argv[i][0] == '-' && argv[i][1] == 'l') info.flags.linear = true;
        else if (argv[i][0] == '-' && argv[i][1] == 't') info.flags.tile = true;
        else if (argv[i][0] == '-' && argv[i][1] == 'b') byte_color = true;
        else return 0;

    u8* components = loadBitmap(bitmap_file_path, info);
    if (byte_color) {
        ByteColorImage image;
        *((ImageInfo*)(&image)) = info;
        image.content = new ByteColor[image.size];
        componentsToByteColors(components, image, image.content);
        save(image, image_file_path);
    } else if (info.flags.channel) {
        FloatImage image;
        *((ImageInfo*)(&image)) = info;
        image.content = new f32[image.size * (info.flags.alpha ? 4 : 3)];
        componentsToChannels(components, image, image.content);
        save(image, image_file_path);
    } else {
        PixelImage image;
        *((ImageInfo*)(&image)) = info;
        image.content = new Pixel[image.size];
        componentsToPixels(components, image, image.content);
        save(image, image_file_path);
    }

    return 0;
}