/**
 * @authors TeamRedBullTM - Abdulla Alnuaimi, Aidan Sheehan, Carter Dean, Logan Luker, Matthew Ashton-Knochel
 * CS3505
 * a8 - Sprite Editor Implementation
 *
 * This Frame object is a promoted QWidget, and it handles displaying individual frame previews and their control 
 * buttons. 
 * Reviewed By: Matthew Ashton Knochel
 */

#ifndef FRAMEWIDGET_H
#define FRAMEWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLayout>
#include <QLabel>

class FrameWidget : public QWidget
{
    Q_OBJECT
private:
    // reference to the QImage from the associated frame
    const QImage* frameImage;
    //button used to delete frames
    QPushButton* deleteButton;
    //button to move this frame up/ swap with the revious frame
    QPushButton* upButton;
    //button to move this frame down/swap with the next frame
    QPushButton* downButton;
    // button to select this frame and edit it from the canvas
    QPushButton* selectButton;
    // grid layout that will contain and place all the buttons and frame preview nicely
    QGridLayout* layout;
    // label used to display the preview
    QLabel* imageLabel;
    // parent widget
    QWidget* parent;
    // the index of this frame, possibly an un-needed legacy variable
    int index;
signals:
    /**
     * @brief frameSelected signal to emit when the select button is clicked
     * @param index the index of this frame in the scrollview
     */
    void frameSelected(int index);
    /**
     * @brief frameDeleted signal to emit when the delete button is clicked
     * @param index the index of this frame in the scrollview
     */
    void frameDeleted(int index);
    /**
     * @brief frameWidgetDeleted signal to emit when the delete button is clicked
     * @param widget a reference to this widget so that mainwindow can delete it
     */
    void frameWidgetDeleted(FrameWidget* widget);
    /**
     * @brief swapFrames signal to emit when move up/down buttons are clicked
     * @param index1 index of this frame
     * @param index2 index of frame to swap with
     */
    void swapFrames(int index1, int index2);

public:
    /**
     * @brief FrameWidget Constructor for a frame Widget
     * @param parent the scrollview containing the frame widgets
     * @param Size size of the scrollview
     * @param frame QImage representing the frame's image to display
     */
    FrameWidget(QWidget* parent, QSize Size, const QImage* frame);
    /**
     * @brief ~FrameWidget the deconstructor for a frame widget which frees all dynamically allocated memory
     */
    ~FrameWidget();
    /**
     * @brief setImage sets the image to display to a new image and updates the preview
     * @param img image to display in preview
     */
    void setImage(const QImage& img);

    /**
     * @brief setUpButtons this needs to be called by the mainview right after the Frame Widget is added to the view this method
     *  is in charge of connecting the selected button to the appropriate model slot and making sure the widget is automatically selected
     *  when first added.
     */
    void setUpButtons();

public slots:
    /**
     * @brief deleteClicked slot to recieve input from delete button
     */
    void deleteClicked();
    /**
     * @brief upClicked slot to recieve input from up button
     */
    void upClicked();
    /**
     * @brief selectClicked slot to recieve input from select button
     */
    void selectClicked();
    /**
     * @brief downClicked slot to recieve input from down button
     */
    void downClicked();
};

#endif // FRAMEWIDGET_H
