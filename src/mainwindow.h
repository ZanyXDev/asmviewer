#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "decoder.h"
QT_BEGIN_NAMESPACE
class QPoint;
class QTextEdit;
class QByteArray;
class QMenu;
class QPrinter;
class QByteArray;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
signals:
    void fileLoaded(QByteArray &data);
    void startEmulation();
    void stopEmulation();
public slots:
    void about();

private slots:
    void fileOpen();
    bool fileSave();
    void filePrint();
    void filePrintPreview();
    void filePrintPdf();
    void printPreview(QPrinter *);
private:
    void setupEditor();
    void setupFileMenu();
    void setupPrintMenu();
    void setupHelpMenu();
    QPoint calcDeskTopCenter(int width,int height);

    QTextEdit *editor;
    Decoder *ch8Decoder;
    QRect desktopRect;
    // Highlighter *highlighter;
};

#endif // MAINWINDOW_H
