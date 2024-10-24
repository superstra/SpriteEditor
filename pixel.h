/**
 * @authors TeamRedBullTM - Abdulla Alnuaimi, Aidan Sheehan, Carter Dean, Logan Luker, Matthew Ashton-Knochel
 * CS3505
 * a8 - Sprite Editor Implementation
 *
 * This Pixel object holds color information for a pixel.
 * Reviewed By: Matthew Ashton Knochel
 */

#ifndef PIXEL_H
#define PIXEL_H

#include <QColor>
#include <QObject>
class Pixel : public QObject
{
    Q_OBJECT
private:
    /**
     * @brief color QColor to represent color of a pixel
     */
    QColor color;
public:
    /**
     * @brief Pixel default constructor sets all pixels to white and transparent
     */
    Pixel();

    /**
     * @brief Pixel copy constructor
     */
    Pixel(const Pixel&);

    /**
     * @brief Pixel constructor takes a QColor and makes the color of the pisel this color
     * @param color Qcolor to set
     */
    Pixel(QColor color);

    /**
     *  @brief deconstructor for Pixel class
     */
    ~Pixel();

    /**
     * @brief SetColor sets the pixel to a specified color
     * @param color color to set pixel to
     */
    void SetColor(QColor color);

    /**
     * @brief GetColor gets the color of a pixel
     * @return a QColor representing the color of the object
     */
    QColor GetColor();

    /**
     * @brief operator= custom assignment operator that follows copy/swap template
     */
    void operator=(Pixel pix);

    /**
     * @brief toJson converts the pixel to json and returns it
     */
    QJsonObject toJson();
};

#endif // PIXEL_H
