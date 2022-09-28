#include "./slim/platforms/win32_bitmap.h"
#include "./slim/serialization/image.h"

int main(int argc, char *argv[]) {
    Image image;

    char* bitmap_file_path = argv[1];
    char* image_file_path = argv[2];
    image.gamma = (argc == 4 && argv[3][0] == '-' && argv[3][1] == 'l') ? 1.0f : 2.2f;

    u8* components = loadBitmap(bitmap_file_path, image);
    image.pixels = new Pixel[image.width * image.height];

    componentsToPixels(components, image, image.pixels);
    save(image, image_file_path);

    return 0;
}