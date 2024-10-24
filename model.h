/**
 * @authors TeamRedBullTM - Abdulla Alnuaimi, Aidan Sheehan, Carter Dean, Logan Luker, Matthew Ashton-Knochel
 * CS3505
 * a8 - Sprite Editor Implementation
 *
 * Model is the model for our Sprite Editor. It stores frames, canvas size, selected tool, pencil color, among
 * other necessary data. It handles the logic for saving and loading sprite sheet projects (.ssp), changing, 
 * adding, and removing frames, changing tools and features, and drawing sprites.
 */

#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <frame.h>


enum Tool{PENCIL,ERASER};

/**
 * @brief The Model class stores frames, canvas size, selected tool, pencil color, among other necessary data. 
 * It handles the logic for saving and loading sprite sheet projects (.ssp), changing, adding, and removing 
 * frames, changing tools and features, and drawing sprites.
 */
class Model: public QObject
{
    Q_OBJECT

private:
    /**
     * @brief displayFrameIndex The index of the frame being displayed in the main editor.
     */
    int displayFrameIndex = 0;
    /**
     * @brief frames The vector storing all the frame objects.
     */
    std::vector<Frame> frames;
    /**
     * @brief previewFrameDelay The delay in milliseconds between frames in the animation preview.
     */
    int previewFrameDelay = 500;

    /**
     * @brief width The actual width of the sprite in pixels.
     */
    int width = 64;
    /**
     * @brief height The actual height of the sprite in pixels.
     */
    int height = 64;

    int newWidth = 64;
    int newHeight = 64;
    
    /**
     * @brief canvasWidth The canvas width in pixels.
     */
    int canvasWidth = 661;
    /**
     * @brief canvasHeight The canvas height in pixels.
     */
    int canvasHeight = 661;

    /**
     * @brief currentTool The currently selected tool. Defaults to pencil.
     */
    Tool currentTool = PENCIL;
    /**
     * @brief penColor The current pencil color that gets drawn.
     */
    QColor penColor = Qt::black;
    
    /**
     * @brief isOnionSkinEnabled The state of the onion skin feature. Either enabled or disabled.
     */
    bool isOnionSkinEnabled = false;

    /**
     * @brief displayFrameWrapper Is a wrapper method to cut down on repeated code.
     */
    void displayFrameWrapper();


public:
    /**
     * @brief Model default constructor for model
     */
    Model();

    /**
     * @brief Model model constructor with width and height
     */
    Model(int width, int height);

    /**
     * @brief getWidth Getter for the actual width of the sprite.
     */
    int getWidth();
    /**
     * @brief getHeight Getter for the actual height of the sprite.
     */
    int getHeight();

    /**
     * @brief getPenColor Returns the current pen color
     * @return The current pen color
     */
    QColor getPenColor();

    /**
     * @brief getDisplayedFrameIndex returns the index of the frame that is displayed.  0 is first, frames.size-1 is last
     * @return an int index
     */
    int getDisplayedFrameIndex();

    /**
     * @brief getDisplayedQImage returns the backing QImage of the currently displayed frame
     * @return a Qimage for drawing to the screen
     */
    const QImage& getDisplayedQImage();

    /**
     * @brief getImageFromFrame gets the backing image from the frame at index
     * @param index the index of the frame to get
     * @return a const QImage Reference
     */
    const QImage& getImageFromFrame(int index);

    /**
     * @brief updateAnimationPreview recursive method that iterates through the frames and tells the view which to display.  changes to preview frame delay are reflected
     * @param index next index
     */
    void updateAnimationPreview(int index);

    /**
     * @brief addLoadedFrame adds a frame formed from the supplied 2d vector of pixels
     * @param Pixels a 2d vector of pixels to set as the frame's contents
     */
    void addLoadedFrame(std::vector<std::vector<Pixel>> Pixels);

    /**
     * @brief clearFrames removes all frames
     */
    void clearFrames();

    /**
     * @brief getIsOnionSkinEnabled checks if the onionskin should be enabled
     * @return bool representing state of the onion skin
     */
    bool getIsOnionSkinEnabled();


public slots:
    /**
     * @brief addNewFrame adds a new empty frame to the animation
     */
    void addNewFrame();
    /**
     * @brief removeFrame removes a frame from the animation
     */
    void removeFrame(int index);

    /**
     * @brief changeDisplayedFrame switch to editing a different frame
     * @param frameIndex index of the frame to be displayed
     */
    void changeDisplayedFrame(int frameIndex);

    /**
     * @brief CanvasMousePressed slot for when the canvas recieves a mouse pressed event
     * @param mousePos position of mouse
     */
    void CanvasMousePressed(QPoint mousePos);

    /**
     * @brief CanvasMouseMoved slot for when the canvas recieves a mouse moved event
     * @param mousePos the mouse position
     */
    void CanvasMouseMoved(QPoint mousePos);

    /**
     * @brief mirrorClicked slot for when mirror is clicked
     * @param axis the axis to mirror about
     */
    void mirrorClicked(Axis axis);

    /**
     * @brief setPenColor Sets penColor to the given QColor
     * @param newPenColor the new QColor to set
     */
    void setPenColor(QColor newPenColor);

    /**
     * @brief toggleOnionSkin Toggles the Onion Skin feature. If it is currently enabled, disable it and vice versa.
     * @param checkState The state of the check box as the int value
     *      Qt::Unchecked           0	The item is unchecked.
     *      Qt::PartiallyChecked	1	The item is partially checked. Items in hierarchical models may be partially checked if some, but not all, of their children are checked.
     *      Qt::Checked             2	The item is checked.
     */
    void toggleOnionSkin(int checkState);

    /**
     * @brief changePreviewPlayBackSpeed updates the delay variable to the new value
     * @param newVal val to change to
     */
    void changePreviewPlayBackSpeed(int newVal);

    /**
     * @brief changeCurrentToolTo slot used to change the tool
     * @param toolType the tool to change the current tool to
     */
    void changeCurrentToolTo(Tool toolType);

    /**
     * @brief swapFrames slot that swaps any two frames' position in the frame list vector
     * @param index1 index of the first frame
     * @param index2 index of the second frame
     */
    void swapFrames(int index1, int index2);

    /**
     * @brief loadDataFromJSON slot to load data from JSON
     * @param fileName the filename of the file to load
     */
    void loadDataFromJSON(QString fileName);

    /**
     * @brief saveDataToJSON
     * @param filePath
     */
    void saveDataToJSON(QString filePath);

    /**
     * @brief widthBoxChanged new width has been recieved
     * @param value new width
     */
    void widthBoxChanged(int value);

    /**
     * @brief heightBoxChanged new height has been recieved
     * @param value new height
     */
    void heightBoxChanged(int value);

    /**
     * @brief sizeButtonClicked slot to handle when the size button is clicked
     */
    void sizeButtonClicked();

    /**
     * @brief addNewFrames adds new frames from loaded jsonData
     * @param jsonData the loaded data
     */
    void addNewFrames(QByteArray jsonData);



signals:
    /**
     * @brief displayFrame any time a frame is updated, this signal is emitted to the view letting it know that the frame parameter is to be displayed
     * @param currentFrame the frame that is currently selected.
     * @param previousFramePtr the pointer to the frame before the current frame.
     */
    void displayFrame(const QImage& currentFrame, const QImage* previousFramePtr = nullptr);
    /**
     * @brief createFrameWidget tells view to make a Frame Widget for this frame
     * @param frame the frame poinnter to give to FrameWidget
     */
    void createFrameWidget(Frame* frame);
    /**
     * @brief nextPreviewFrame updates the animation preview with the given image
     * @param image the QImage to display
     */
    void nextPreviewFrame(const QImage* image);
    /**
     * @brief framesSwapped tells the view that frame widgets should be updated
     */
    void framesSwapped();
    /**
     * @brief resetFrames tells the view to reset the frame widgets. Used for JSON loading
     */
    void resetFrames(QByteArray jsonData);
    /**
     * @brief deleteFirstFrame tells the view to delete the first frame. Used for JSON loading
     */
    void deleteFirstFrame();
    /**
     * @brief changeCanvasSize tells the view to change the canvas size to the given width and height in pixels
     * @param width the number of pixels wide
     * @param height the number of pixels tall
     */
    void changeCanvasSize(int width, int height);

    void loadFinished();
};

#endif // MODEL_H
