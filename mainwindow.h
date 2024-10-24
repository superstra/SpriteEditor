/**
 * @authors TeamRedBullTM - Abdulla Alnuaimi, Aidan Sheehan, Carter Dean, Logan Luker, Matthew Ashton-Knochel
 * CS3505
 * a8 - Sprite Editor Implementation
 *
 * MainWindow is the view and controller for our Sprite Editor. It handles updating the user interface, and all 
 * buttons, menus, hotkey, and mouse inputs.
 * Reviewed By: Matthew Ashton-Knochel
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "model.h"
#include "frameWidget.h"
#include <QMainWindow>
#include <QLayout>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/**
 * @brief The MainWindow class in charge of the view, connects slots/signals from the view to the model, and model to the view
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Model& model, QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    /**
     * @brief removeFrameWidget removes a widget from the frame selector side panel
     * @param widget
     */
    void removeFrameWidget(FrameWidget* widget);

    void deleteLastFrameWidget();

    /**
     * @brief updateAllFrameWidgetPreviews updates all the previews on the side panel
     */
    void updateAllFrameWidgetPreviews();
    /**
     * @brief openColorPicker displays a QColorDialog widget to allow the user to
     * select their pen color
     */
    void openColorPicker();

    /**
     * @brief addFrameWidget adds a new frame widget to the bar of frames on the side
     * @param frame the frame to add
     */
    void addFrameWidget(Frame* frame);

    /**
     * @brief updateFramePreview updates the preview of the frame being drawn on the side of the screen
     */
    void updateFramePreview();

    /**
     * @brief updateEditorWindow Update the editor window's display. Includes the
     * canvas and onion skin.
     * @param frame the frame to be drawn on the editor. The onion skin is the
     * Model::displayFrameIndex - 1
     */
    void updateEditorWindow(const QImage& currentFrame, const QImage* previousFramePtr);

    /**
     * @brief mirrorClickedSlot called when the mirror button is clicked
     */
    void mirrorClickedSlot();

    /**
     * @brief updateAnimationPreview updates the animation being played in the bottom right cornet
     * @param image
     */
    void updateAnimationPreview(const QImage* image);

    /**
     * @brief displayOriginalResolutionPreview makes a little window displaying the true size of the sprite
     */
    void displayOriginalResolutionPreview();

    /**
     * @brief pencilButtonClicked sets the tool to erasor
     */
    void pencilButtonClicked();

    /**
     * @brief eraserButtonClicked sets the tool to erasor
     */
    void eraserButtonClicked();

    /**
     * @brief openJsonFile called when the load button is clicked, asks the model to load frames.
     */
    void openJsonFile();

    /**
     * @brief saveToJsonFile connected to save button, asks model to begin save process
     */
    void saveToJsonFile();


    /**
     * @brief resetFramesSlot deletes all the frames on the side panel
     */
    void resetFramesSlot(QByteArray jsonData);


    /**
     * @brief deleteFirstFrameSlot deletes the frame at index 0 in the animation and removes the preview from the side bar
     */
    void deleteFirstFrameSlot();

    void changeCanvasSize(int width, int height);
private:
    Ui::MainWindow *ui;
    Model* modelPtr;
    void keyPressEvent(QKeyEvent* event);

signals:
    /**
     * @brief sendPenColor sends a signal to the model to set the Pen Color
     * @param newPenColor the QColor picked
     */
    void sendPenColor(QColor newPenColor);

    /**
     * @brief mirrorClicked asks the model to mirror the frame being displayed
     * @param axis axis to be mirrored over
     */
    void mirrorClicked(Axis axis);

    /**
     * @brief changeToolTo asks the model to change the tool
     * @param toolType tool to change to
     */
    void changeToolTo(Tool toolType);

    /**
     * @brief jsonFileRead asks the model to read json from fileName and load it into the viewer
     * @param fileName the filename to read from
     */
    void jsonFileRead(QString fileName);

    /**
     * @brief saveJsonFile the path to the json file that should be saved.
     * @param fileName
     */
    void saveJsonFile(QString fileName);

    void addNewFrames(QByteArray jsonData);
};
#endif // MAINWINDOW_H
