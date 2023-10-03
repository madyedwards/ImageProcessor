#include <iostream>
#include <vector>
#include <fstream>
#include "Header.h"
using namespace std;

void LoadFile(string fileName, vector<Image>& imageList) {

    ifstream inFile(fileName, ios_base::binary); //since reading as binary, is ok to iterate total

    Image::Header newHeader;

    inFile.read(&newHeader.idLength, sizeof(newHeader.idLength));
    inFile.read(&newHeader.colorMapType, sizeof(newHeader.colorMapType));
    inFile.read(&newHeader.dataTypeCode, sizeof(newHeader.dataTypeCode));
    inFile.read((char *) &newHeader.colorMapOrigin, sizeof(newHeader.colorMapOrigin));
    inFile.read((char *) &newHeader.colorMapLength, sizeof(newHeader.colorMapLength));
    inFile.read((char *) &newHeader.colorMapDepth, sizeof(newHeader.colorMapDepth));
    inFile.read((char *) &newHeader.xOrigin, sizeof(newHeader.xOrigin));
    inFile.read((char *) &newHeader.yOrigin, sizeof(newHeader.yOrigin));
    inFile.read((char *) &newHeader.width, sizeof(newHeader.width));
    inFile.read((char *) &newHeader.height, sizeof(newHeader.height));
    inFile.read((char *) &newHeader.bitsPerPixel, sizeof(newHeader.bitsPerPixel));
    inFile.read((char *) &newHeader.imageDescriptor, sizeof(newHeader.imageDescriptor));

    unsigned int imageSize  = newHeader.height * newHeader.width;
    vector<Image::Pixel> pixelList;

    for (unsigned int i = 0; i < imageSize; i++ ) { //alternate: read as vector and vector, with height and width separate

        Image::Pixel newPixel;
        inFile.read((char*)&newPixel.redPixel, sizeof (newPixel.redPixel));
        inFile.read((char*)&newPixel.greenPixel, sizeof (newPixel.greenPixel));
        inFile.read((char*)&newPixel.bluePixel, sizeof (newPixel.bluePixel));

        pixelList.push_back(newPixel);

    }
    Image newImage(newHeader,pixelList);
    imageList.push_back(newImage);

    inFile.close();

}
void OutputFile(string newOutputFileName, Image existingImage) {

    ofstream outFile(newOutputFileName, ios_base::binary);


    outFile.write(&existingImage.newHeader.idLength, sizeof (existingImage.newHeader.idLength));
    outFile.write(&existingImage.newHeader.colorMapType, sizeof (existingImage.newHeader.colorMapType));
    outFile.write(&existingImage.newHeader.dataTypeCode, sizeof (existingImage.newHeader.dataTypeCode));
    outFile.write((char*)&existingImage.newHeader.colorMapOrigin, sizeof (existingImage.newHeader.colorMapOrigin));
    outFile.write((char*)&existingImage.newHeader.colorMapLength, sizeof (existingImage.newHeader.colorMapLength));
    outFile.write((char*)&existingImage.newHeader.colorMapDepth, sizeof (existingImage.newHeader.colorMapDepth));
    outFile.write((char*)&existingImage.newHeader.xOrigin, sizeof(existingImage.newHeader.xOrigin));
    outFile.write((char*)&existingImage.newHeader.yOrigin, sizeof (existingImage.newHeader.yOrigin));
    outFile.write((char*)&existingImage.newHeader.width, sizeof (existingImage.newHeader.width));
    outFile.write((char*)&existingImage.newHeader.height, sizeof (existingImage.newHeader.height));
    outFile.write((char*)&existingImage.newHeader.bitsPerPixel, sizeof (existingImage.newHeader.bitsPerPixel));
    outFile.write((char*)&existingImage.newHeader.imageDescriptor, sizeof (existingImage.newHeader.imageDescriptor));

    for (int i = 0; i < existingImage.pixelList.size(); i++) {

        Image::Pixel currentPixel = existingImage.pixelList[i];

        outFile.write((char*)&currentPixel.redPixel, 1);
        outFile.write((char*)&currentPixel.greenPixel,1);
        outFile.write((char*)&currentPixel.bluePixel,1);

    }
    outFile.close();

}
float FloatValue(float newFloat) {

    newFloat = newFloat + 0.5f;
    return newFloat;

}
unsigned int ClampValues(int passedValue) {

    if (passedValue > 255)
        passedValue = 255;
    if (passedValue < 0)
        passedValue = 0;

    return (unsigned int)passedValue;

}
Image Multiply(Image &topImage, Image &bottomImage) {
    Image multipliedImage;
    Image::Header multipliedHeader = topImage.GetHeader();
    multipliedImage.SetHeader(multipliedHeader);

    vector<Image::Pixel> multipliedPixelList;

    for (int i = 0; i < topImage.pixelList.size(); i++) {

        Image::Pixel multipliedPixel;

        unsigned int productRed = (unsigned int) (FloatValue(topImage.pixelList[i].redPixel * bottomImage.pixelList[i].redPixel / 255.0f));
        unsigned char productRedChar = topImage.UIntToUChar(productRed);
        multipliedPixel.redPixel = productRedChar;

        unsigned int productGreen = (unsigned int) (FloatValue(topImage.pixelList[i].greenPixel * bottomImage.pixelList[i].greenPixel / 255.0f));
        unsigned char productGreenChar = topImage.UIntToUChar(productGreen);
        multipliedPixel.greenPixel = productGreenChar;

        unsigned int productBlue = (unsigned int) (FloatValue(topImage.pixelList[i].bluePixel * bottomImage.pixelList[i].bluePixel / 255.0f));
        unsigned char productBlueChar = topImage.UIntToUChar(productBlue);
        multipliedPixel.bluePixel = productBlueChar;

        multipliedImage.pixelList.push_back(multipliedPixel);

    }
    multipliedImage.SetPixelList(multipliedPixelList);
    return multipliedImage;

}
Image Subtract(Image &topImage, Image &bottomImage) {
    Image subtractedImage;
    Image::Header subtractedHeader = topImage.GetHeader();
    subtractedImage.SetHeader(subtractedHeader);

    vector<Image::Pixel> subtractedPixelList;

    for (int i = 0; i < topImage.pixelList.size() ;i++) {

        Image::Pixel subtractedPixel;
        unsigned int topImageRedU = topImage.UCharToUInt(topImage.pixelList[i].redPixel);
        unsigned int bottomImageRedU = bottomImage.UCharToUInt(bottomImage.pixelList[i].redPixel);
        unsigned int topImageGreenU = topImage.UCharToUInt(topImage.pixelList[i].greenPixel);
        unsigned int bottomImageGreenU = bottomImage.UCharToUInt(bottomImage.pixelList[i].greenPixel);
        unsigned int topImageBlueU = topImage.UCharToUInt(topImage.pixelList[i].bluePixel);
        unsigned int bottomImageBlueU = bottomImage.UCharToUInt(bottomImage.pixelList[i].bluePixel);

        int differenceRed = topImageRedU - bottomImageRedU;
        unsigned int clampedRed = ClampValues(differenceRed);
        unsigned char redChar = topImage.UIntToUChar(clampedRed);
        subtractedPixel.redPixel = redChar;

        int differenceGreen = topImageGreenU - bottomImageGreenU;
        unsigned int clampedGreen = ClampValues(differenceGreen);
        unsigned char greenChar = topImage.UIntToUChar(clampedGreen);
        subtractedPixel.greenPixel = greenChar;

        int differenceBlue = topImageBlueU - bottomImageBlueU;
        unsigned int clampedBlue = ClampValues(differenceBlue);
        unsigned char blueChar = topImage.UIntToUChar(clampedBlue);
        subtractedPixel.bluePixel = blueChar;

        subtractedPixelList.push_back(subtractedPixel);

    }
    subtractedImage.SetPixelList(subtractedPixelList);
    return subtractedImage;


}
float ScreenFloat(unsigned int topInt, unsigned int bottomInt) {
    float result = 1.0f - ((1.0f - (((float)(topInt) / 255.0f))) * ((1.0f - ((float)(bottomInt) / 255.0f))));
    return result;

}
float ScreenFloat2(float topInt, float bottomInt) {
    float result = 1.0f - ((1.0f - (((float)(topInt) / 255.0f))) * ((1.0f - ((float)(bottomInt) / 255.0f))));
    return result;

}
Image Screen(Image &topImage, Image &bottomImage) {

    Image screenImage;
    Image::Header screenImageHeader = topImage.GetHeader();
    screenImage.SetHeader(screenImageHeader);

    vector <Image::Pixel> screenPixelList;

    for (int i = 0; i < topImage.pixelList.size(); i++) {

        Image::Pixel screenImagePixel;

        //makes them unsigned ints to do calculations
        unsigned int topImageRedU = topImage.UCharToUInt(topImage.pixelList[i].redPixel);
        unsigned int bottomImageRedU = bottomImage.UCharToUInt(bottomImage.pixelList[i].redPixel);
        unsigned int topImageGreenU = topImage.UCharToUInt(topImage.pixelList[i].greenPixel);
        unsigned int bottomImageGreenU = bottomImage.UCharToUInt(bottomImage.pixelList[i].greenPixel);
        unsigned int topImageBlueU = topImage.UCharToUInt(topImage.pixelList[i].bluePixel);
        unsigned int bottomImageBlueU = bottomImage.UCharToUInt(bottomImage.pixelList[i].bluePixel);

        float redFloat = ScreenFloat(topImageRedU, bottomImageRedU);
        unsigned int redInt = (unsigned int) FloatValue(redFloat * 255.0f);
        unsigned char redChar = topImage.UIntToUChar(redInt);
        screenImagePixel.redPixel = redChar;

        float greenFloat = ScreenFloat(topImageGreenU, bottomImageGreenU);
        unsigned int greenInt = (unsigned int) FloatValue(greenFloat * 255.0f);
        unsigned char greenChar = topImage.UIntToUChar(greenInt);
        screenImagePixel.greenPixel = greenChar;

        float blueFloat = ScreenFloat(topImageBlueU, bottomImageBlueU);
        unsigned int blueInt = (unsigned int) FloatValue(blueFloat * 255.0f);
        unsigned char blueChar = topImage.UIntToUChar(blueInt);
        screenImagePixel.bluePixel = blueChar;

        screenPixelList.push_back(screenImagePixel);
    }
    screenImage.SetPixelList(screenPixelList);
    return screenImage;

}
bool DarkBackground(float passedValue){
    return (passedValue <= 127.5f);

}
Image Overlay(Image &topImage, Image &bottomImage) {

    Image overlayImage;
    Image::Header overlayImageHeader = topImage.GetHeader();
    overlayImage.SetHeader(overlayImageHeader);

    vector<Image::Pixel> overlayPixelVector;

    for (int i = 0; i < topImage.pixelList.size(); i++) {

        Image::Pixel overlayPixel;
        float overlayRedU, overlayGreenU, overlayBlueU;

        float topImageRedU =  (float) topImage.pixelList[i].redPixel;
        float bottomImageRedU = (float)bottomImage.pixelList[i].redPixel;
        float topImageGreenU = (float)topImage.pixelList[i].greenPixel;
        float bottomImageGreenU = (float) bottomImage.pixelList[i].greenPixel;
        float topImageBlueU = (float)topImage.pixelList[i].bluePixel;
        float bottomImageBlueU = (float)bottomImage.pixelList[i].bluePixel;

        if ((DarkBackground(bottomImageRedU)) || DarkBackground(bottomImageGreenU) || DarkBackground(bottomImageBlueU)) {
            overlayRedU = FloatValue(2.0f * (topImageRedU * bottomImageRedU/255.0f));
            overlayGreenU = FloatValue(2.0f * (topImageGreenU * bottomImageGreenU/255.0f));
            overlayBlueU = FloatValue( 2.0f * (topImageBlueU * bottomImageBlueU/255.0f));

        }
        else {
            overlayRedU = FloatValue(ScreenFloat2(topImageRedU, bottomImageRedU) * 255.0f);
            overlayGreenU = FloatValue(ScreenFloat2(topImageGreenU, bottomImageGreenU) * 255.0f);
            overlayBlueU = FloatValue(ScreenFloat2(topImageBlueU, bottomImageBlueU) * 255.0f);

        }
        overlayRedU = ClampValues(overlayRedU);
        overlayGreenU = ClampValues(overlayGreenU);
        overlayBlueU = ClampValues(overlayBlueU);

        unsigned char redChar = (unsigned char)overlayRedU;
        overlayPixel.redPixel = redChar;

        unsigned char greenChar = (unsigned char) overlayGreenU;
        overlayPixel.greenPixel = greenChar;

        unsigned char blueChar = (unsigned char) overlayBlueU;
        overlayPixel.bluePixel = blueChar;

        overlayPixelVector.push_back(overlayPixel);
    }
    overlayImage.SetPixelList(overlayPixelVector);
    return overlayImage;

}
Image ExtraCredit(Image &imageCar, Image &imageCircle, Image &imageText, Image &imagePattern) {

    Image combinedImage;
    Image::Header combinedHeader = imageCar.GetHeader();
    combinedImage.SetHeader(combinedHeader);

    short newWidth = imageCar.newHeader.width * 2;
    combinedImage.newHeader.width = newWidth;
    short newHeight = imageCar.newHeader.height * 2;
    combinedImage.newHeader.height = newHeight;

    short iteration = newWidth * newHeight;
    vector<Image::Pixel> combinedPixelVector;

    for (short i = 0; i < imageText.newHeader.height; i++) {

        for (short j = 0; j < imageText.newHeader.width; j++) { //needs to include an i
            Image::Pixel combinedPixel;

            combinedPixel.redPixel = imageText.pixelList[j + (512 * i)].redPixel;
            combinedPixel.greenPixel = imageText.pixelList[j + (512 * i)].greenPixel;
            combinedPixel.bluePixel = imageText.pixelList[j + (512 * i)].bluePixel;

            combinedPixelVector.push_back(combinedPixel);
        }
        for (short j = 0; j < imagePattern.newHeader.width; j++) {
            Image::Pixel combinedPixel;

            combinedPixel.redPixel = imagePattern.pixelList[j + (512 * i)].redPixel;
            combinedPixel.greenPixel = imagePattern.pixelList[j + (512 * i)].greenPixel;
            combinedPixel.bluePixel = imagePattern.pixelList[j + (512 * i)].bluePixel;

            combinedPixelVector.push_back(combinedPixel);
        }

    }
    for (short i = 0; i < imageCar.newHeader.height; i++) {

        for (short j = 0; j < imageCar.newHeader.width; j++) {
            Image::Pixel combinedPixel;

            combinedPixel.redPixel = imageCar.pixelList[j + (512 * i)].redPixel;
            combinedPixel.greenPixel = imageCar.pixelList[j + (512 * i)].greenPixel;
            combinedPixel.bluePixel = imageCar.pixelList[j + (512 * i)].bluePixel;

            combinedPixelVector.push_back(combinedPixel);
        }
        for (short j = 0; j < imageCircle.newHeader.width; j++) {
            Image::Pixel combinedPixel;

            combinedPixel.redPixel = imageCircle.pixelList[j + (512 * i)].redPixel;
            combinedPixel.greenPixel = imageCircle.pixelList[j + (512 * i)].greenPixel;
            combinedPixel.bluePixel = imageCircle.pixelList[j + (512 * i)].bluePixel;

            combinedPixelVector.push_back(combinedPixel);
        }

    }
    combinedImage.SetPixelList(combinedPixelVector);
    return combinedImage;

}
Image GreenMethod(Image &image) {

    Image greenImage;
    Image::Header greenHeader = image.GetHeader();
    greenImage.SetHeader(greenHeader);

    vector<Image::Pixel> greenImagePixels;

    for (int i = 0; i < image.pixelList.size(); i++) {

        Image::Pixel newPixel;
        newPixel.redPixel = image.pixelList[i].redPixel;
        newPixel.bluePixel = image.pixelList[i].bluePixel;

        unsigned int greenInt = image.UCharToUInt(image.pixelList[i].greenPixel);
        int sum = greenInt + 200;
        unsigned gInt = ClampValues(sum);
        unsigned char greenChar = image.UIntToUChar(gInt);
        newPixel.greenPixel = greenChar;

        greenImagePixels.push_back(newPixel);
    }
    greenImage.SetPixelList(greenImagePixels);
    return greenImage;

}
Image Part7(Image &image){

    Image newImage;
    Image::Header newImageHeader = image.GetHeader();
    newImage.SetHeader(newImageHeader);

    vector<Image::Pixel> newPixelVector;

    for (int i = 0; i < image.pixelList.size(); i++) {

        Image::Pixel newPixel;

        unsigned char tempR = image.pixelList[i].bluePixel;
        //scale red by 4
        unsigned int redInt = image.UCharToUInt(tempR);
        float redFloat = ((float)redInt/255.0f) * 4.0f;
        redInt = (unsigned int)FloatValue(redFloat * 255.0f);
        redInt = ClampValues(redInt);
        unsigned char redChar = image.UIntToUChar(redInt);


        newPixel.greenPixel = image.pixelList[i].greenPixel;
        //scale blue by 0
        unsigned int blueInt = 0;
        unsigned char blueChar = image.UIntToUChar(blueInt);

        newPixel.redPixel = blueChar;
        newPixel.bluePixel = redChar;

        newPixelVector.push_back(newPixel);
    }
    newImage.SetPixelList(newPixelVector);
    return newImage;
}
Image Part8 (string imageChannel, Image &image) {

    Image newImage;
    Image::Header newHeader = image.GetHeader();
    newImage.SetHeader(newHeader);

    vector<Image::Pixel> newPixelList;
    vector<Image::Pixel> imagePixelList = image.GetPixelList();

    for (int i = 0; i < image.pixelList.size(); i++) {

        Image::Pixel newPixel;
        unsigned char pixelChannel;

        if (imageChannel == "red")
            pixelChannel = imagePixelList[i].redPixel;
        else if (imageChannel == "green")
            pixelChannel = imagePixelList[i].greenPixel;
        else if (imageChannel == "blue")
            pixelChannel = imagePixelList[i].bluePixel;

        newPixel.redPixel = pixelChannel;
        newPixel.greenPixel = pixelChannel;
        newPixel.bluePixel = pixelChannel;

        newPixelList.push_back(newPixel);

    }
    newImage.SetPixelList(newPixelList);
    return newImage;

}
Image Part9(Image &redImage, Image &greenImage, Image &blueImage) {

    Image blendedImage;
    Image::Header blendedHeader = redImage.GetHeader();
    blendedImage.SetHeader(blendedHeader);

    vector<Image::Pixel> blendedPixelVector;

    for (int i = 0;i < redImage.pixelList.size(); i ++){

        Image::Pixel blendedPixel;

        blendedPixel.bluePixel = redImage.pixelList[i].redPixel;
        blendedPixel.greenPixel = greenImage.pixelList[i].greenPixel;
        blendedPixel.redPixel = blueImage.pixelList[i].bluePixel;

        blendedPixelVector.push_back(blendedPixel);

    }
    blendedImage.SetPixelList(blendedPixelVector);
    return blendedImage;


}
Image Part10(Image &image){

    Image flippedImage;
    Image::Header flippedHeader = image.GetHeader();
    flippedImage.SetHeader(flippedHeader);

    vector<Image::Pixel> flippedPixelVector;

    for (int i = image.pixelList.size()-1; i >= 0; i--) {

        Image::Pixel flippedPixel;

        flippedPixel.redPixel = image.pixelList[i].redPixel;
        flippedPixel.greenPixel = image.pixelList[i].greenPixel;
        flippedPixel.bluePixel = image.pixelList[i].bluePixel;

        flippedPixelVector.push_back(flippedPixel);

    }
    flippedImage.SetPixelList(flippedPixelVector);
    return flippedImage;

}
bool TestCase(Image image1, Image image2){

    for (int i = 0; i < 10; i++) {
        if (image1.pixelList[i].redPixel != image2.pixelList[i].redPixel) {
            return false;
            break;
        }
        if (image1.pixelList[i].greenPixel != image2.pixelList[i].greenPixel) {
            return false;
            break;
        }
        if (image1.pixelList[i].bluePixel != image2.pixelList[i].bluePixel) {
            return false;
            break;
        }
    }
    return true;

}

int main() {
    vector<Image> imageList;

    //LOADED FILES
    LoadFile("input/layer1.tga",imageList);
    Image layer1Image = imageList[0];

    LoadFile("input/pattern1.tga",imageList);
    Image pattern1Image = imageList[1];

    LoadFile("input/layer2.tga", imageList);
    Image layer2Image = imageList[2];

    LoadFile("input/car.tga", imageList);
    Image carImage = imageList[3];

    LoadFile("input/pattern2.tga", imageList);
    Image pattern2Image = imageList[4];

    LoadFile("input/text.tga", imageList);
    Image textImage = imageList[5];

    LoadFile("input/circles.tga", imageList);
    Image circlesImage = imageList[6];

    LoadFile("input/layer_red.tga", imageList);
    Image layerRedImage = imageList[7];

    LoadFile("input/layer_green.tga", imageList);
    Image layerGreenImage = imageList[8];

    LoadFile("input/layer_blue.tga", imageList);
    Image layerBlueImage = imageList[9];

    LoadFile("input/text2.tga", imageList);
    Image text2Image = imageList[10];


    //PART1 passed
    imageList.push_back(Multiply(layer1Image,pattern1Image));
    Image part1Image = imageList[11];
    OutputFile("output/part1.tga",part1Image);

    //PART 2 passed
    imageList.push_back(Subtract(carImage,layer2Image));
    Image part2Image = imageList[12];
    OutputFile("output/part2.tga", part2Image);

    //PART 3 passed
    imageList.push_back(Multiply(layer1Image,pattern2Image));
    Image tempPart3 = imageList[13];
    imageList.push_back(Screen(textImage,tempPart3));
    Image part3Result = imageList[14];
    OutputFile("output/part3.tga", part3Result);

    //PART 4 passed
    imageList.push_back(Multiply(layer2Image,circlesImage));
    Image tempPart4 = imageList[15];
    imageList.push_back(Subtract(tempPart4,pattern2Image));
    Image part4Result = imageList[16];
    OutputFile("output/part4.tga", part4Result);

    //PART 5 //some red and blue differ by 1
    imageList.push_back(Overlay(layer1Image, pattern1Image));
    Image part5Result = imageList[17];
    OutputFile("output/part5.tga", part5Result);

    //PART 6 passed
    imageList.push_back(GreenMethod(carImage));
    Image part6Result = imageList[18];
    OutputFile("output/part6.tga", part6Result);

    //PART 7 passed
    imageList.push_back(Part7(carImage));
    Image part7result = imageList[19];
    OutputFile("output/part7.tga", part7result);

    //PART8 passed
    imageList.push_back(Part8("red", carImage));
    Image part8red = imageList[20];
    OutputFile("output/part8_r.tga", part8red);
    imageList.push_back(Part8("green", carImage));
    Image part8green = imageList[21];
    OutputFile("output/part8_g.tga", part8green);
    imageList.push_back(Part8("blue", carImage));
    Image part8blue = imageList[22];
    OutputFile("output/part8_b.tga", part8blue);

    //PART 9 passed
    imageList.push_back(Part9(layerRedImage,layerGreenImage,layerBlueImage));
    Image part9result = imageList[23];
    OutputFile("output/part9.tga", part9result);

    //PART10 passed
    imageList.push_back(Part10(text2Image));
    Image part10result = imageList[24];
    OutputFile("output/part10.tga", part10result);

    //EC
    imageList.push_back(ExtraCredit(carImage,circlesImage,textImage,pattern1Image));
    Image extracredit = imageList[25];
    OutputFile("output/extracredit.tga", extracredit);

    //tests
    LoadFile("examples/EXAMPLE_part1.tga", imageList);
    Image originalTest1 = imageList[26];   //this location will change
    bool value1 = TestCase(part1Image,originalTest1);
    if (value1){
        cout << "Part 1: Passed!" << endl;
    }
    else {
        cout << "Part 1: Failed!" << endl;
    }
    
    LoadFile("examples/EXAMPLE_part2.tga",imageList);
    Image originalTest2 = imageList[27];
    bool value2 = TestCase(part2Image,originalTest2);
    if (value2){
        cout << "Part 2: Passed!" << endl;
    }
    else {
        cout << "Part 2: Failed!" << endl;
    }

    LoadFile("examples/EXAMPLE_part3.tga", imageList);
    Image originalTest3 = imageList[28]; //dont forget to change this
    bool value3= TestCase(part3Result,originalTest3);
    if (value3){
        cout << "Part 3: Passed!" << endl;
    }
    else {
        cout << "Part 3: Failed!" << endl;
    }

    LoadFile("examples/EXAMPLE_part4.tga", imageList);
    Image originalTest4 = imageList[29];
    bool value4 = TestCase(part4Result, originalTest4);
    if (value4){
        cout << "Part 4: Passed!" << endl;
    }
    else {
        cout << "Part 4: Failed!" << endl;
    }

    LoadFile("examples/EXAMPLE_part5.tga", imageList);
    Image originalTest5 = imageList[30];
    bool value5 = TestCase(part5Result,originalTest5);
    if (value5){
        cout << "Part 5: Passed!" << endl;
    }
    else {
        cout << "Part 5: Failed!" << endl;
    }

    LoadFile("examples/EXAMPLE_part6.tga", imageList);
    Image originalTest6 = imageList[31];
    bool value6 = TestCase(part6Result,originalTest6);
    if (value6){
        cout << "Part 6: Passed!" << endl;
    }
    else {
        cout << "Part 6: Failed!" << endl;
    }

    LoadFile("examples/EXAMPLE_part7.tga", imageList);
    Image orig7 = imageList[32];
    bool value7 = TestCase(orig7,part7result);
    if (value7){
        cout << "Part 7: Passed!" << endl;
    }
    else {
        cout << "Part 7: Failed!" << endl;
    }

    LoadFile("examples/EXAMPLE_part8_b.tga", imageList);
    Image orig8R = imageList[33];
    bool value8A = TestCase(orig8R, part8red);
    if (value8A){
        cout << "Part 8A: Passed!" << endl;
    }
    else {
        cout << "Part 8A: Failed!" << endl;
    }

    LoadFile("examples/EXAMPLE_part8_g.tga", imageList);
    Image orig8G = imageList[34];
    bool value8B = TestCase(orig8G, part8green);
    if (value8B){
        cout << "Part 8B: Passed!" << endl;
    }
    else {
        cout << "Part 8B: Failed!" << endl;
    }

    LoadFile("examples/EXAMPLE_part8_r.tga", imageList);
    Image orig8B = imageList[35];
    bool value8C = TestCase(orig8B, part8blue);
    if (value8C){
        cout << "Part 8C: Passed!" << endl;
    }
    else {
        cout << "Part 8C: Failed!" << endl;
    }

    LoadFile("examples/EXAMPLE_part9.tga", imageList);
    Image orig9 = imageList[36];
    bool value9 = TestCase(orig9, part9result);
    if (value9){
        cout << "Part 9: Passed!" << endl;
    }
    else {
        cout << "Part 9: Failed!" << endl;
    }

    LoadFile("examples/EXAMPLE_part10.tga", imageList);
    Image orig10 = imageList[37];
    bool value10 = TestCase(orig10, part10result);
    if (value10){
        cout << "Part 10: Passed!" << endl;
    }
    else {
        cout << "Part 10: Failed!" << endl;
    }

    LoadFile("examples/EXAMPLE_extracredit.tga", imageList);
    Image ec = imageList[38];
    bool valueEC = TestCase(ec,extracredit);
    if (valueEC){
        cout << "Part EC: Passed!" << endl;
    }
    else {
        cout << "Part EC: Failed!" << endl;
    }


}

/*
 * i know building it by storing every image as a vector was kind of stupid, but i tried to change it halfway through and the images
 * straight up would not compile so this at least works (not perfectly though)
 *
 * part8: image pixels 56, 44, 62
 *
 * references:
 * https://stackoverflow.com/questions/10477463/in-c-and-objective-c-should-we-use-0-5f-or-0-5
 *
 */