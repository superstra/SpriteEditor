/**
 * @authors TeamRedBullTM - Abdulla Alnuaimi, Aidan Sheehan, Carter Dean, Logan Luker, Matthew Ashton-Knochel
 * CS3505
 * a8 - Sprite Editor Implementation
 *
 * This Canvas object is a promoted QWidget and handles sending mouse events and drawing on the canvas.
 *
 * Reviewed By: Carter Dean
 */

#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QMouseEvent>

class Canvas : public QWidget
{
    Q_OBJECT

private:
    /**
     * @brief currentlyDisplayed a reference to the currently displayed QImage
     */
    const QImage* currentlyDisplayed;

public:
    /**
     * @brief Canvas Constructor for Canvas Widget
     * @param parent parent Widget
     * @param initialImage The first QImage to display
     */
    explicit Canvas(QWidget *parent = nullptr, const QImage* initialImage = nullptr);

protected:
    /**
     * @brief mousePressEvent event that fires when the canvas starts to draw
     * @param event the event
     */
    void mousePressEvent(QMouseEvent *event) override;
    /**
     * @brief mouseMoveEvent event that continuously checks mouse position to allow dragging
     * @param event the event
     */
    void mouseMoveEvent(QMouseEvent *event) override;
    /**
     * @brief mouseReleaseEvent event that occurs when canvas should stop drawing
     * @param event the event
     */
    void mouseReleaseEvent(QMouseEvent *event) override;

    /**
     * @brief paintEvent This fires when the canvas should update the displayed QImage
     * @param event the event
     */
    void paintEvent(QPaintEvent *event) override;

signals:
    /**
     * @brief mousePressed signal to fire when mouse is pressed
     * @param pos mouse position
     */
    void mousePressed(const QPoint &pos);
    /**
     * @brief mousePressed signal to fire when mouse is moved
     * @param pos mouse position
     */
    void mouseMoved(const QPoint &pos);
    /**
     * @brief mousePressed signal to fire when mouse is released
     * @param pos mouse position
     */
    void mouseReleased(const QPoint &pos);

public slots:
    /**
     * @brief changeImage slot to handle the changing which frame's image should be displayed
     * @param img the new QImage to display
     */
    void changeImage(const QImage& img);

};

#endif // CANVAS_H
