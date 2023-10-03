#include <fstream>
#include "Header.h"
#include <iostream>
#include <vector>
using namespace std;


Image::Image() {
    newHeader.idLength = '0';
    newHeader.colorMapType = '0';
    newHeader.dataTypeCode = '0';
    newHeader.colorMapOrigin = 0;
    newHeader.colorMapLength = 0;
    newHeader.colorMapDepth = 0;
    newHeader.xOrigin = 0;
    newHeader.yOrigin = 0;
    newHeader.width = 0;
    newHeader.height = 0;
    newHeader.bitsPerPixel = '0';
    newHeader.imageDescriptor = '0';


}
Image::Image(Image::Header newHeader, vector<Image::Pixel> pixelList) {

    SetHeader(newHeader);
    this->pixelList = pixelList;

}

Image::Header Image::GetHeader() {
    return newHeader;

}

void Image::SetHeader(Image::Header &newHeader) {

    this->newHeader.idLength = newHeader.idLength;
    this->newHeader.colorMapType = newHeader.colorMapType;
    this->newHeader.dataTypeCode = newHeader.dataTypeCode;
    this->newHeader.colorMapOrigin = newHeader.colorMapOrigin;
    this->newHeader.colorMapLength = newHeader.colorMapLength;
    this->newHeader.colorMapDepth = newHeader.colorMapDepth;
    this->newHeader.xOrigin = newHeader.xOrigin;
    this->newHeader.yOrigin = newHeader.yOrigin;
    this->newHeader.width = newHeader.width;
    this->newHeader.height = newHeader.height;
    this->newHeader.bitsPerPixel = newHeader.bitsPerPixel;
    this->newHeader.imageDescriptor = newHeader.imageDescriptor;
}

vector<Image::Pixel> Image::GetPixelList() {
    return pixelList;

}

void Image::SetPixelList(vector<Image::Pixel> &pixelListSetter) {

    for (unsigned int i = 0; i < pixelListSetter.size(); i++) {

        pixelList.push_back(pixelListSetter[i]);
    }

}
unsigned int Image::UCharToUInt(unsigned char passedChar) {

    return (unsigned int)(passedChar - '\0'); //do i need to signify '\0' here ?
}
unsigned char Image::UIntToUChar(unsigned int passedInt) {

    return (unsigned char)(passedInt + '\0');
}

/*void Image::SetUnsignedInts() {

    for (int i = 0; )

}
 */
