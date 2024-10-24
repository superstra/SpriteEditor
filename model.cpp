/**
 * @authors TeamRedBullTM - Abdulla Alnuaimi, Aidan Sheehan, Carter Dean, Logan Luker, Matthew Ashton-Knochel
 * CS3505
 * a8 - Sprite Editor Implementation
 *
 * Model is the model for our Sprite Editor. It stores frames, canvas size, selected tool, pencil color, among
 * other necessary data. It handles the logic for saving and loading sprite sheet projects (.ssp), changing, 
 * adding, and removing frames, changing tools and features, and drawing sprites.
 *
 * Reviewed by: Aidan Sheehan
 */

#include "model.h"
#include "frame.h"
#include "frameWidget.h"
#include "qdebug.h"
#include <QTimer>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

Model::Model() {
}

void Model::widthBoxChanged(int value) {
    newWidth = value;
}



void Model::heightBoxChanged(int value) {
    newHeight = value;
}

void Model::sizeButtonClicked() {
    width = newWidth;
    height = newHeight;

    emit changeCanvasSize(width, height);
}

void Model::CanvasMousePressed(QPoint mousePos) {
    this->CanvasMouseMoved(mousePos);
}

void Model::CanvasMouseMoved(QPoint mousePos) {
    // Calculate the ratio of the original image size to the scaled image size
    double xratio = (double )(width) / (double)canvasWidth;
    double yratio = (double) (height) / (double)canvasHeight;

    // Get the mouse position relative to the widget
    // Calculate the corresponding position in the original image
    int x = qRound(mousePos.x() * xratio);
    int y = qRound(mousePos.y() * yratio);

    if(x < 0 || y < 0 || x > width || y > height-1)
        return;

    // If the selected tool is pencil draw with the current color, otherwise erase the pixel.
    if(currentTool == PENCIL)
        this->frames[this->displayFrameIndex].setPixel(x,y,this->penColor);
    else
        this->frames[this->displayFrameIndex].setPixel(x,y,QColor(255, 255, 255, 0));

    // Update the displayed frame
    displayFrameWrapper();
}

void Model::addNewFrame() {
    Frame* newFrame = new Frame(width, height);
    frames.push_back(*newFrame);
    emit createFrameWidget(newFrame); // tell view to make a Frame Widget for this frame
    // Update the displayed frame
    displayFrameWrapper();
}

void Model::removeFrame(int index) {
    //figure out which frame to select when frame is deleted or to add a new one
    if(frames.size() > 1){
        //show frame after deleted one unless deleted frame is last
        if(index < frames.size()-1)
            changeDisplayedFrame(index + 1);
        else
            changeDisplayedFrame(index - 1);
    }
    else {
        addNewFrame();
        changeDisplayedFrame(0);
    }

    frames.erase(frames.begin() + index);

    // Update the displayed frame
    displayFrameWrapper();
}

void Model::changeDisplayedFrame(int frameIndex) {
    qDebug() << "change displayed frame called with index: "<<frameIndex;
    this->displayFrameIndex = frameIndex;
    // Update the displayed frame
    displayFrameWrapper();
}

void Model::displayFrameWrapper() {
    // If there is a previous frame, send that along with the current frame
    if(displayFrameIndex > 0 && frames.size() > 1)
        emit displayFrame(this->getDisplayedQImage(), &(this->frames.at(displayFrameIndex - 1).getImage()));
    else
        emit displayFrame(this->getDisplayedQImage());
}

int Model::getHeight() {
    return height;
}

int Model::getWidth() {
    return width;
}

QColor Model::getPenColor() {
    return penColor;
}

bool Model::getIsOnionSkinEnabled() {
    return this->isOnionSkinEnabled;
}

int Model::getDisplayedFrameIndex() {
    return displayFrameIndex;
}

const QImage& Model::getDisplayedQImage() {
    return this->frames[displayFrameIndex].getImage();
}

void Model::setPenColor(QColor newPenColor) {
    penColor = newPenColor;
}

void Model::toggleOnionSkin(int checkState) {
    this->isOnionSkinEnabled = !isOnionSkinEnabled;
    // Update the displayed frame
    displayFrameWrapper();
}

void Model::mirrorClicked(Axis axis) {
    this->frames[displayFrameIndex].mirror(axis);
    displayFrameWrapper();
}

void Model::updateAnimationPreview(int index) {
    if(index >= frames.size()) index = 0; //ensure index is never out of bounds when a frame is removed
    emit nextPreviewFrame(&frames[index].getImage());

    if(index < frames.size() - 1) index++;
    else index = 0;

    QTimer::singleShot(previewFrameDelay, [this, index]() {updateAnimationPreview(index);});

}

void Model::changePreviewPlayBackSpeed(int newVal) {
    previewFrameDelay = 1000 - newVal;
}

void Model::changeCurrentToolTo(Tool toolType) {
    currentTool = toolType;
}

const QImage& Model::getImageFromFrame(int index) {
    if(index >= 0 && index < frames.size()){
        return this->frames[index].getImage();
    }
    else return this->frames[0].getImage();
}

void Model::swapFrames(int index1, int index2) {
    std::iter_swap(frames.begin() + index1, frames.begin() + index2);
    changeDisplayedFrame(index2);
    emit framesSwapped();
    // Update the displayed frame
    displayFrameWrapper();
}


// loading and saving frames using JSON

void Model::loadDataFromJSON(QString fileName) {
    if(!fileName.isEmpty()) {
        QFile file(fileName);
        if(file.open(QIODevice::ReadOnly)) {
            QByteArray jsonData = file.readAll();
            file.close();
            emit resetFrames(jsonData);
        }else {
            qDebug() << "Could not open the file";
        }
    }
}

void Model::addLoadedFrame(std::vector<std::vector<Pixel>> Pixels) {
    Frame* loadedFrame = new Frame(width, height);
    loadedFrame->setPixels(Pixels);

    // set colors in their location
    for(int y = 0; y < Pixels.size(); ++y) {
        for(int x = 0; x < Pixels[y].size(); ++x) {
            Pixel pixel = Pixels[y][x];
            QColor color = pixel.GetColor();
            loadedFrame->setPixel(x,y,color);
        }
    }

    frames.push_back(*loadedFrame);
    emit createFrameWidget(loadedFrame); // tell view to make a Frame Widget for this frame
    // Update the displayed frame
    displayFrameWrapper();
}

void Model::saveDataToJSON(QString filePath) {
    QJsonArray framesArray;
    for(Frame& frame : frames)
        framesArray.append(frame.toJson());

    QJsonObject document;
    document["frames"] = framesArray;
    document["width"]= width;
    document["height"] = height;

    QJsonDocument jsonDocument(document);
    QFile newJsonFile(filePath);
    if(newJsonFile.open(QIODevice::WriteOnly)) {
        newJsonFile.write(jsonDocument.toJson());
        newJsonFile.close();
    } else {
        qDebug() << "Could not save at " << filePath;
    }
}


void Model::addNewFrames(QByteArray jsonData) {
    QJsonDocument jsondocument = QJsonDocument::fromJson(jsonData);
    QJsonObject jsonObject = jsondocument.object();

    QJsonArray framesArray = jsonObject["frames"].toArray();
    this->width = jsonObject["width"].toInt();
    this->height = jsonObject["height"].toInt();
    // Iterate through each frame
    for(const QJsonValue &frameValue : framesArray) {
        QJsonObject frameObject = frameValue.toObject();
        QJsonArray pixelsArray = frameObject["pixels"].toArray();

        // Iterate through each pixel in the frame
        std::vector<std::vector<Pixel>> pixels;
        for(const QJsonValue &rowValue : pixelsArray) {
            QJsonArray rowArray = rowValue.toArray();
            std::vector<Pixel> row;

            // Iterate through each rows in the pixel
            for(const QJsonValue &pixelValue : rowArray) {
                QJsonObject pixelObject = pixelValue.toObject();
                QColor loadedcolor(pixelObject["r"].toInt(), pixelObject["g"].toInt(),
                                   pixelObject["b"].toInt(), pixelObject["a"].toInt());
                Pixel pixel(loadedcolor);
                row.push_back(pixel);
            }
            pixels.push_back(row);
        }
        addLoadedFrame(pixels);
    }
    emit deleteFirstFrame();
    emit framesSwapped();
    addNewFrame();
    emit loadFinished();
}
