/**
 * @authors TeamRedBullTM - Abdulla Alnuaimi, Aidan Sheehan, Carter Dean, Logan Luker, Matthew Ashton-Knochel
 * CS3505
 * a8 - Sprite Editor Implementation
 *
 * This Canvas object is a promoted QWidget and handles sending mouse events and drawing on the canvas.
 *
 * Reviewed by: Carter Dean
 */

#include "canvas.h"
#include "qpainter.h"

Canvas::Canvas(QWidget *parent, const QImage* initialImage): QWidget{parent}, currentlyDisplayed(initialImage) {
    setAttribute(Qt::WA_StaticContents);
}

void Canvas::mousePressEvent(QMouseEvent *event) {
    emit mousePressed(event->pos());
}

void Canvas::mouseMoveEvent(QMouseEvent *event) {
    emit mouseMoved(event->pos());
}

void Canvas::mouseReleaseEvent(QMouseEvent *event) {
    emit mouseReleased(event->pos());

}

void Canvas::changeImage(const QImage& newImage) {
    this->currentlyDisplayed = &newImage;
    update(); // repaints, and schedules a call to paintEvent
}

void Canvas::paintEvent(QPaintEvent *event) {
    QPainter painter(this); // painter with canvas as parent
    QRect dirtyRect = event->rect(); // get rectangle to dictate size of image to draw
    if (this->currentlyDisplayed != nullptr) {
        QImage scaledImage = this->currentlyDisplayed->scaled(dirtyRect.size(), Qt::KeepAspectRatio); // scale the image to fit
        painter.drawImage(dirtyRect, scaledImage, QRect(QPoint(0, 0), scaledImage.size())); // draw image
    } else {
        qDebug() << "ERROR: currentlyDisplayed should probably never be null" << Qt::endl;
    }
}
