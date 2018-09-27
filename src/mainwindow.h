#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "decoder.h"
QT_BEGIN_NAMESPACE
class QTextEdit;
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
public slots:
    void about();
    void openFile(const QString &path = QString());
private:
    void setupEditor();
    void setupFileMenu();
    void setupHelpMenu();

    QTextEdit *editor;
    Decoder *ch8Decoder;
   // Highlighter *highlighter;
};

#endif // MAINWINDOW_H
