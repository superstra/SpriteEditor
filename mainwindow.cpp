/**
 * @authors TeamRedBullTM - Abdulla Alnuaimi, Aidan Sheehan, Carter Dean, Logan Luker, Matthew Ashton-Knochel
 * CS3505
 * a8 - Sprite Editor Implementation
 *
 * MainWindow is the view and controller for our Sprite Editor. It handles updating the user interface, and all 
 * buttons, menus, hotkey, and mouse inputs.
 *
 * Reviewed by: Abdulla Alnuaimi
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "frameWidget.h"
#include <QColorDialog>
#include <QTimer>
#include <QSlider>
#include <QFileDialog>


MainWindow::MainWindow(Model& model, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    modelPtr = &model;

    //create layout for framesbox
            ui->FramesBoxContents->setLayout(ui->framesLayout);

    // Connect methods here
    connect(ui->CanvasWidget,
            &Canvas::mousePressed,
            &model,
            &Model::CanvasMousePressed);

    connect(ui->CanvasWidget,
            &Canvas::mouseMoved,
            &model,
            &Model::CanvasMouseMoved);

    connect(&model,
            &Model::displayFrame,
            this,
            &MainWindow::updateEditorWindow);

    connect(ui->colorPicker,
            &QPushButton::clicked,
            this,
            &MainWindow::openColorPicker);

    // Tell model to set pen color
    connect(this,
            &MainWindow::sendPenColor,
            &model,
            &Model::setPenColor);

    // Onion Skin Check Box
    connect(ui->OnionSkinCheckBox,
            &QCheckBox::stateChanged,
            &model,
            &Model::toggleOnionSkin);

    // Tell model to change width/height
    connect(ui->widthBox,
            &QSpinBox::valueChanged,
            &model,
            &Model::widthBoxChanged);

    connect(ui->heightBox,
            &QSpinBox::valueChanged,
            &model,
            &Model::heightBoxChanged);

    connect(ui->sizeButton,
            &QPushButton::clicked,
            &model,
            &Model::sizeButtonClicked);



    //frame button stuff
    connect(ui->AddFrameButton, &QPushButton::clicked, &model, &Model::addNewFrame);
    connect(&model, &Model::createFrameWidget, this, &MainWindow::addFrameWidget);
    connect(&model, &Model::framesSwapped, this, &MainWindow::updateAllFrameWidgetPreviews);

//    connect(ui->MirrorButton, &QPushButton::clicked, this, &)
    connect(&model, &Model::displayFrame, this, &MainWindow::updateFramePreview);

    //animation preview stuff
    connect(&model, &Model::nextPreviewFrame, this, &MainWindow::updateAnimationPreview);
    connect(ui->previewFramePlaybackSpeed, &QSlider::valueChanged, &model, &Model::changePreviewPlayBackSpeed);
    connect(ui->previewResolutionButton, &QPushButton::clicked, this, &MainWindow::displayOriginalResolutionPreview);

    //tool selector stuff
    connect(this, &MainWindow::changeToolTo, &model, &Model::changeCurrentToolTo);
    connect(ui->PencilButton, &QPushButton::pressed, this, &MainWindow::pencilButtonClicked);
    connect(ui->EraserButton, &QPushButton::pressed, this, &MainWindow::eraserButtonClicked);


    connect(ui->MirrorButton, &QPushButton::clicked, this, &MainWindow::mirrorClickedSlot);

    connect(this, &MainWindow::mirrorClicked, &model, &Model::mirrorClicked);

    model.addNewFrame(); // add the first frame to the animation on startup
    model.updateAnimationPreview(0);

    // Loading JSON file
    connect(ui->actionLoad,&QAction::triggered, this, &MainWindow::openJsonFile);
    connect(this,&MainWindow::jsonFileRead,&model, &Model::loadDataFromJSON);
    connect(&model ,&Model::resetFrames,this,&MainWindow::resetFramesSlot);
    connect(&model, &Model::deleteFirstFrame,this,&MainWindow::deleteFirstFrameSlot);
    connect(this,&MainWindow::addNewFrames,&model,&Model::addNewFrames);
    connect(&model, &Model::loadFinished, this, &MainWindow::deleteLastFrameWidget);

    // Saving to JSON file
    connect(ui->actionSave,&QAction::triggered, this, &MainWindow::saveToJsonFile);
    connect(this,&MainWindow::saveJsonFile,&model, &Model::saveDataToJSON);

}

MainWindow::~MainWindow() {
    delete ui;
    delete modelPtr;
}

// Resize Canvas
void MainWindow::changeCanvasSize(int width, int height) {
    ui->CanvasWidget->resize(width, height);
    ui->CanvasWidget->repaint();
}

void MainWindow::updateEditorWindow(const QImage& currentFrame, const QImage* previousFramePtr) {
    // Display the onion skin if enabled
    if(previousFramePtr != nullptr && modelPtr->getIsOnionSkinEnabled()) {
        QImage previousFrameImage = previousFramePtr->copy();
        QPixmap onionSkinPixmap = QPixmap::fromImage(previousFrameImage, Qt::NoOpaqueDetection).scaled(ui->onionSkin->size(), Qt::KeepAspectRatio);

        // Redraw the pixmap with lowered opacity
        QPixmap translucentPixmap(onionSkinPixmap.size());
        translucentPixmap.fill(Qt::transparent);
        QPainter painter;
        painter.begin(&translucentPixmap);
        painter.setOpacity(0.2);
        painter.drawPixmap(0, 0, onionSkinPixmap);
        painter.end();

        ui->onionSkin->setPixmap(translucentPixmap);
    } else if (previousFramePtr == nullptr || !modelPtr->getIsOnionSkinEnabled()) {
        ui->onionSkin->clear();
    }

    // Draw the provided frame on the canvas to be edited
    ui->CanvasWidget->changeImage(currentFrame);
}

void MainWindow::updateFramePreview() {
    // traverse scroll widget contents to update the frame prev at model->displayed
    QLayoutItem* widget = ui->framesLayout->itemAt(modelPtr->getDisplayedFrameIndex());
    if (widget == nullptr) {
//        qDebug() << "widget was null";
        return;
    }
    FrameWidget* frameWidget = qobject_cast<FrameWidget*>(widget->widget());
    frameWidget->setImage(modelPtr->getDisplayedQImage());
}

void MainWindow::updateAllFrameWidgetPreviews(){
    for(int i = 0; i < ui->framesLayout->count(); i++){
        QLayoutItem* widget = ui->framesLayout->itemAt(i);
        if (widget == nullptr) {
            qDebug() << "widget was null";
            return;
        }
        FrameWidget* frameWidget = qobject_cast<FrameWidget*>(widget->widget());
        frameWidget->setImage(modelPtr->getImageFromFrame(i));
    }
}

void MainWindow::addFrameWidget(Frame* f) {
    auto frameWidget = new FrameWidget(ui->FramesBoxContents, ui->FramesBox->size(), (&f->getImage()));
    ui->framesLayout->addWidget(frameWidget);
    connect(frameWidget, &FrameWidget::frameSelected, modelPtr, &Model::changeDisplayedFrame);
    connect(frameWidget, &FrameWidget::frameDeleted, modelPtr, &Model::removeFrame);
    connect(frameWidget, &FrameWidget::swapFrames, modelPtr, &Model::swapFrames);
    connect(frameWidget, &FrameWidget::frameWidgetDeleted, this, &MainWindow::removeFrameWidget);
    frameWidget->setUpButtons();
    // connect the frames selected button signal to the model's change displayed frame slot
}

void MainWindow::removeFrameWidget(FrameWidget* frameWidget) {
    //disconnect widget from model and mainwindow
    disconnect(frameWidget, &FrameWidget::frameSelected, modelPtr, &Model::changeDisplayedFrame);
    disconnect(frameWidget, &FrameWidget::frameDeleted, modelPtr, &Model::removeFrame);
    disconnect(frameWidget, &FrameWidget::swapFrames, modelPtr, &Model::swapFrames);
    disconnect(frameWidget, &FrameWidget::frameWidgetDeleted, this, &MainWindow::removeFrameWidget);

    delete frameWidget;
}

void MainWindow::keyPressEvent( QKeyEvent* event ) {
    switch ( event->key() ) {
    case Qt::Key_P:
        ui->PencilButton->click();
        break;
    case Qt::Key_E:
        ui->EraserButton->click();
        break;
    case Qt::Key_M:
        ui->MirrorButton->click();
        break;
    case Qt::Key_O:
        ui->OnionSkinCheckBox->click();
        break;
    case Qt::Key_X:
        ui->xAxisButton->click();
        break;
    case Qt::Key_Y:
        ui->yAxisButton->click();
        break;
    case Qt::Key_C:
        ui->colorPicker->click();
        break;
    case Qt::Key_A:
        ui->AddFrameButton->click();
        break;
    case Qt::Key_R:
        ui->previewResolutionButton->click();
        break;
    default:
        event->ignore();
        break;
    }
}

void MainWindow::openColorPicker() {
    // Open a new QColorDialog for the user to pick a color
    // Set the color to the color the user currently has selected
    QColor newColor = QColorDialog::getColor(modelPtr->getPenColor());
    if (newColor.isValid())
        emit sendPenColor(newColor);
}

void MainWindow::updateAnimationPreview(const QImage* image) {
        ui->previewFrameDisplayLabel->setPixmap(QPixmap::fromImage(*image).scaled(ui->previewFrameDisplayLabel->size()));
}

void MainWindow::displayOriginalResolutionPreview(){
    QDialog* originalResolutionPreviewDialog = new QDialog(this);
    originalResolutionPreviewDialog->setWindowTitle("Original Resolution Preview");
    originalResolutionPreviewDialog->setMinimumWidth(modelPtr->getWidth());
    originalResolutionPreviewDialog->setMinimumHeight(modelPtr->getHeight() + 50);
    originalResolutionPreviewDialog->setModal(true);

    QVBoxLayout* originalResolutionPreviewLayout = new QVBoxLayout(originalResolutionPreviewDialog);


    QLabel* originalResolutionImageLabel = new QLabel(originalResolutionPreviewDialog);
    originalResolutionImageLabel->setMinimumSize(originalResolutionPreviewDialog->minimumSize());

    QPushButton* originalResolutionPreviewCloseButton = new QPushButton(originalResolutionPreviewDialog);
    originalResolutionPreviewCloseButton->setText("Close");

    originalResolutionPreviewLayout->addWidget(originalResolutionImageLabel, 1);
    originalResolutionPreviewLayout->addWidget(originalResolutionPreviewCloseButton, 1);

    //connect buttons and frame update
    auto connection1 = connect(modelPtr, &Model::nextPreviewFrame, originalResolutionImageLabel, [originalResolutionImageLabel](const QImage* image){originalResolutionImageLabel->setPixmap(QPixmap::fromImage(*image));});
    connect(originalResolutionPreviewCloseButton, &QPushButton::clicked, originalResolutionPreviewDialog,
            [originalResolutionPreviewDialog, originalResolutionPreviewCloseButton, originalResolutionImageLabel, originalResolutionPreviewLayout, connection1](){
                disconnect(connection1);
                delete originalResolutionPreviewCloseButton;
                delete originalResolutionImageLabel;
                delete originalResolutionPreviewLayout;
                delete originalResolutionPreviewDialog;            }
        );

    originalResolutionPreviewDialog->show();
    originalResolutionImageLabel->show();
}

void MainWindow::mirrorClickedSlot() {
    if (ui->xAxisButton->isChecked()) {
        emit mirrorClicked(Axis::X);
    } else if (ui->yAxisButton->isChecked()){
        emit mirrorClicked(Axis::Y);
    }
}

void MainWindow::pencilButtonClicked() { emit changeToolTo(Tool::PENCIL); }
void MainWindow::eraserButtonClicked() { emit changeToolTo(Tool::ERASER); }

void MainWindow::openJsonFile() {
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open File"),QDir::homePath(),tr("SSP Files (*.ssp)"));
    emit jsonFileRead(fileName);
}

void MainWindow::saveToJsonFile(){
    QString Path = QFileDialog::getSaveFileName(this,tr("Choose where to save the file"),QDir::homePath(),tr("SSP Files (*.ssp)"));
    emit saveJsonFile(Path);
}

void MainWindow::resetFramesSlot(QByteArray jsonData){
    int prevCount = ui->framesLayout->count();
    for(int i = prevCount; i > 0 ; i--){
        QLayoutItem* widget = ui->framesLayout->itemAt(i-1);
        if (widget == nullptr) {
            qDebug() << "widget was null";
            return;
        }
        FrameWidget* frameWidget = qobject_cast<FrameWidget*>(widget->widget());
        frameWidget->deleteClicked();
    }
    emit addNewFrames(jsonData);
}

void MainWindow::deleteLastFrameWidget(){
    qobject_cast<FrameWidget*>(ui->framesLayout->itemAt(ui->framesLayout->count()-1)->widget())->deleteClicked();
}

void MainWindow::deleteFirstFrameSlot(){
    QLayoutItem* widget = ui->framesLayout->itemAt(0);
    FrameWidget* frameWidget = qobject_cast<FrameWidget*>(widget->widget());
    frameWidget->deleteClicked();
}
