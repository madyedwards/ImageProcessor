#include <iostream>
#include <vector>
using namespace std;



class Image {
public:
    struct Pixel {
        //does this go inside Header? or remove Image as a class, and replace as "header"
        unsigned char redPixel;
        unsigned char greenPixel;
        unsigned char bluePixel;

    };
    struct Header {
        char idLength;
        char colorMapType;
        char dataTypeCode;
        short colorMapOrigin;
        short colorMapLength;
        char colorMapDepth;
        short xOrigin;
        short yOrigin;
        short width;
        short height;
        char bitsPerPixel;
        char imageDescriptor;

    };

    Image::Header newHeader;
    vector<Image::Pixel> pixelList;

    //basic image
    Image();

    //image w values
    Image(Image::Header newHeader, vector<Image::Pixel> pixelList);

    //header w values
    void SetHeader(Header &newHeader);

    //return the header
    Header GetHeader();

    //get pixel list
    vector<Image::Pixel> GetPixelList();

    //set pixel list
    void SetPixelList(vector<Image::Pixel>& pixelList);

    //conversions
    unsigned int UCharToUInt(unsigned char passedChar);
    unsigned char UIntToUChar(unsigned int passedInt);

    //setting the unsignedInts
    //void SetUnsignedInts();


private:
    vector<Image::Header> imageList;
    Pixel newPixel;

};
