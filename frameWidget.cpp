/**
 * @authors TeamRedBullTM - Abdulla Alnuaimi, Aidan Sheehan, Carter Dean, Logan Luker, Matthew Ashton-Knochel
 * CS3505
 * a8 - Sprite Editor Implementation
 *
 * This Frame object is a promoted QWidget, and it handles displaying individual frame previews and their control 
 * buttons. 
 *
 * Reviewed by: Matthew Ashton-Knochel
 */

#include "frameWidget.h"
#include <QPushButton>
#include <QLayout>
#include <QGraphicsView>
#include <QTimer>

FrameWidget::FrameWidget(QWidget* parent, QSize size, const QImage* frame) : QWidget(parent), frameImage(frame) {
    // set size of the widget to insure nothing is squished/stretched
    this->setMaximumSize(size.width()-20, 150);
    this->setMinimumSize(size.width()-20, 150);
    this->parent = parent;

    // create delete button
    deleteButton = new QPushButton(this);
    deleteButton->setText("Del");
    deleteButton->setFixedWidth(this->width()/5);

    // create up button
    upButton = new QPushButton(this);
    upButton->setText("Up");
    upButton->setFixedWidth(this->width()/5);;

    // create down button
    downButton = new QPushButton(this);
    downButton->setText("Down");
    downButton->setFixedWidth(this->width()/5);

    // create select button
    selectButton = new QPushButton(this);
    selectButton->setText("Sel");
    selectButton->setFixedWidth(this->width()/5);

    // create label to display preview
    imageLabel = new QLabel(this);
    imageLabel->setStyleSheet("border: 1px solid black;");
    imageLabel->setMinimumSize(QSize(128, 128));

    // create a layout to contain everything and add everything to it
    layout = new QGridLayout(this);
    layout->setHorizontalSpacing(0);
    layout->addWidget(upButton, 0, 0, 1, 1);
    layout->addWidget(downButton, 1, 0, 1, 1);
    layout->addWidget(selectButton, 2, 0, 1, 1);
    layout->addWidget(deleteButton, 3, 0, 1, 1);
    layout->addWidget(imageLabel, 0, 1, 4, 3);

    imageLabel->setPixmap(QPixmap::fromImage(*frame).scaled(imageLabel->size())); // set the initial pixmap for the preview

    this->setLayout(layout); // assign the layout to the frame Widget
}

FrameWidget::~FrameWidget() {
    //delete everything that was allocated
    delete deleteButton;
    delete upButton;
    delete downButton;
    delete layout;
    delete imageLabel;
}

void FrameWidget::deleteClicked() {
    // find index of this frame widget
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(parent->findChild<QVBoxLayout*>());
    int index = layout->indexOf(this);

    //emit necessary signals to delete the frame from the model and the widget from the view
    emit frameDeleted(index);
    emit frameWidgetDeleted(this);
}
void FrameWidget::selectClicked() {
    //find index of this frame widget
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(parent->findChild<QVBoxLayout*>());
    int index = layout->indexOf(this);

    // signal model to change the currently selected frame to this
    emit frameSelected(index);
}
void FrameWidget::upClicked() {
    //find index of this frame widet
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(parent->findChild<QVBoxLayout*>());
    int index = layout->indexOf(this);

    // do nothing if first frame, otherwise, move this frame up in order
    if(index > 0) emit swapFrames(index, index - 1);
}
void FrameWidget::downClicked() {
    //find index of this frame widget
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(parent->findChild<QVBoxLayout*>());
    int index = layout->indexOf(this);

    // do nothing if last frame, otherwise move the frame down in the order
    if(index < layout->count() - 1) emit swapFrames(index, index + 1);
}

void FrameWidget::setImage(const QImage& img) {
    imageLabel->setPixmap(QPixmap::fromImage(img).scaled(imageLabel->size())); //set the pixmap to the new image
    update(); //update the frame widget, redrawing the preview
}

void FrameWidget::setUpButtons() {
    //connect all buttons
    connect(selectButton, &QAbstractButton::clicked, this, &FrameWidget::selectClicked);
    connect(deleteButton, &QAbstractButton::clicked, this, &FrameWidget::deleteClicked);
    connect(upButton, &QAbstractButton::clicked, this, &FrameWidget::upClicked);
    connect(downButton, &QAbstractButton::clicked, this, &FrameWidget::downClicked);

    // automatically select the frame that has just been added
    this->selectClicked();
}


