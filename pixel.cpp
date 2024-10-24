/**
 * @authors TeamRedBullTM - Abdulla Alnuaimi, Aidan Sheehan, Carter Dean, Logan Luker, Matthew Ashton-Knochel
 * CS3505
 * a8 - Sprite Editor Implementation
 *
 * This Pixel object holds color information for a pixel.
 *
 * Reviewed by: Matthew Ashton-Knochel
 */

#include "pixel.h"
#include <QColor>
#include <QJsonObject>

Pixel::Pixel() : color(QColor(255, 255, 255, 0)) {

}

Pixel::Pixel(QColor color) : color(color) {

}

Pixel::~Pixel() {

}

Pixel::Pixel(const Pixel& pix) {
    this->color = QColor(pix.color); // copies the color
}

void Pixel::operator=(Pixel pix) {
    std::swap(this->color, pix.color); //swap
}

void Pixel::SetColor(QColor newColor) {
    this->color = newColor;
}

QColor Pixel::GetColor() {
    return this->color;
}

QJsonObject Pixel::toJson(){
    QJsonObject pixelJson;
    pixelJson["r"] = color.red();
    pixelJson["g"] = color.green();
    pixelJson["b"] = color.blue();
    pixelJson["a"] = color.alpha();
    return pixelJson;
}
