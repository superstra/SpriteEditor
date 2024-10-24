/**
 * @authors TeamRedBullTM - Abdulla Alnuaimi, Aidan Sheehan, Carter Dean, Logan Luker, Matthew Ashton-Knochel
 * CS3505
 * a8 - Sprite Editor Implementation
 *
 * This Frame object handles the logic behind frames. It holds the QImage associated with the frame and can modify 
 * the QImage's pixels.
 *
 * Reviewed by: Logan Luker
 */

#include "frame.h"
#include <pixel.h>
#include <QColor>
#include <QImage>
#include <vector>
#include <algorithm>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

Frame::Frame(int width, int height){
    backingImage = QImage(width, height, QImage::Format_ARGB32);
    backingImage.fill(qRgba(255, 255, 255, 1)); // initialize image to being white
}

const QImage& Frame::getImage(){
   return this->backingImage;
}

void Frame::mirror(const Axis axis){
    //simple swicth block to decide how to reverse the pixels
    switch(axis) {
        case Axis::X:
            std::reverse(this->pixels.begin(), this->pixels.end());
            backingImage.mirror(true,false);
        break;
        case Axis::Y:
            for(auto& row : pixels) {
                std::reverse(row.begin(), row.end());
            }
            backingImage.mirror(false,true);
        break;
    }
}

void Frame::setPixel(int x, int y, QColor color){
    QPoint p(x,y);
    this->backingImage.setPixel(p, color.rgba());
    Pixel newPixel(color);
    this->pixels[y][x] = newPixel;
}

Frame::Frame(const Frame& frame) {
    this->backingImage = frame.backingImage; // the copy constructor in vector takes care of this
    this->width = frame.width;
    this->height = frame.height;
}

void Frame::operator=(Frame other){
    std::swap(this->backingImage, other.backingImage);
    std::swap(this->width, other.width);
    std::swap(this->height, other.height);
}

void Frame::setPixels(std::vector<std::vector<Pixel>> pixels){
    this->pixels = pixels;
}

QJsonObject Frame::toJson(){
    QJsonObject frameObject;
    QJsonArray rowsArray;
    for (std::vector<Pixel> row : this->pixels) {
        QJsonArray rowArray;
        for (Pixel pixel : row) {
            rowArray.append(pixel.toJson());
        }
        rowsArray.append(rowArray);
    }
    frameObject["pixels"] = rowsArray;
    return frameObject;
}
