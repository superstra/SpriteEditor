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

#ifndef FRAME_H
#define FRAME_H
#include <QObject>
#include <pixel.h>
#include <QPicture>
#include <QPainter>
#include <QColor>

enum class Axis {X,Y,}; // enum representing the X or Y axis for use with mirror tool

class Frame: public QObject
{
    Q_OBJECT

private:
    /**
     * @brief backingImage QImage Representing the Image to be used to display in app
     */
    QImage backingImage;

    //private width and height variables to help clarify code
    int width = 64;
    int height = 64;

    /**
     * @brief pixels the 2d array of pixels to be used for json saving and loading.  [x][y], [col][row]
     */
    std::vector<std::vector<Pixel>> pixels = std::vector<std::vector<Pixel>>(height, std::vector<Pixel>(width));     //2d array of pixels
public:
    /**
     * @brief Frame constructor for making a new frame
     * @param width width of the image
     * @param height height of the image
     */
    Frame(int width, int height);

    /**
     * @brief Frame copy constructor
     */
    Frame(const Frame& other);

    /**
     * @brief getImage
     * @return returns a reference to the QImage object that stores the "preview" of the frame
     */
    const QImage& getImage();

    /**
     * @brief operator = Frame Assignment operator that follows copy/swap template
     * @param other
     */
    void operator=(Frame other);

    /**
     * @brief setPixels sets the backing array for use with loading
     * @param pixels the new array
     */
    void setPixels(std::vector<std::vector<Pixel>> pixels);

    /**
     * @brief  method to convert a frame to a JSON representation
     * @return a JSON representation
     */
    QJsonObject toJson();


public slots:
    /**
     * @brief mirror mirrors the image over a certain axis
     * @param axis X or Y access
     */
    void mirror(const Axis axis);

    /**
     * @brief setPixel sets a pixel to a certain color
     * @param x x coord of pixel to change
     * @param y y coord of pixel to change
     */
    void setPixel(int x, int y, QColor color);
};

#endif // FRAME_H
