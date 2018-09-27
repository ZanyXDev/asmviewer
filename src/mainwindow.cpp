#include <QMessageBox>
#include <QFileDialog>
#include <QStatusBar>
#include <QTextEdit>
#include <QByteArray>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QApplication>
#include <QDesktopWidget>

#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printer)
#if QT_CONFIG(printdialog)
#include <QPrintDialog>
#endif

#include <QPrinter>
#if QT_CONFIG(printpreviewdialog)
#include <QPrintPreviewDialog>
#endif
#endif
#endif

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupFileMenu();
    setupPrintMenu();
    setupHelpMenu();
    setupEditor();
    setupEmulator();

    setCentralWidget(editor);
    setWindowTitle(tr("CHIP-8 Assembler syntax highlighter"));

    QDesktopWidget desktop;
    desktopRect = desktop.availableGeometry(desktop.primaryScreen());

    this->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::MinimumExpanding);
    this->setMaximumSize(desktopRect.width() - 5, desktopRect.height() - 5);

    //Move app window to center desktop
    this->move(calcDeskTopCenter(this->width(),this->height()));
    this->setMinimumSize(800,600);

}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Syntax Highlighter"),
                       tr("<p>The <b>Syntax Highlighter</b> example shows how " \
                          "to perform simple syntax highlighting by subclassing " \
                          "the QSyntaxHighlighter class and describing " \
                          "highlighting rules using regular expressions.</p>"));
}

void MainWindow::fileOpen()
{
    QString selectedFilter;
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Select chip-8 game file"),
                                                    "",
                                                    tr("Chip-8 game files (*.ch8)"),
                                                    &selectedFilter);

    if (!fileName.isEmpty()) {

        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            return;
        }
        QByteArray tmp = file.readAll();

        emit fileLoaded( tmp );
        emit startDisassembler();
    }
}

bool MainWindow::fileSave()
{
    return true;
}

void MainWindow::filePrint()
{

}

void MainWindow::filePrintPreview()
{
#if QT_CONFIG(printpreviewdialog)
    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer, this);
    connect(&preview, &QPrintPreviewDialog::paintRequested, this, &MainWindow::printPreview);
    preview.exec();
#endif
}

void MainWindow::printPreview(QPrinter *printer)
{
#ifdef QT_NO_PRINTER
    Q_UNUSED(printer);
#else
    editor->print(printer);
#endif
}

void MainWindow::showAsmText(QStringList &text)
{
    if (!text.isEmpty()){
        foreach (const QString &str, text) {
            editor->append(str);
        }
    }
}

void MainWindow::filePrintPdf()
{
#ifndef QT_NO_PRINTER

    QFileDialog fileDialog(this, tr("Export PDF"));
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setMimeTypeFilters(QStringList("application/pdf"));
    fileDialog.setDefaultSuffix("pdf");
    if (fileDialog.exec() != QDialog::Accepted){

        return;
    }
    QString fileName = fileDialog.selectedFiles().first();
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    editor->document()->print(&printer);
    statusBar()->showMessage(tr("Exported \"%1\"")
                             .arg(QDir::toNativeSeparators(fileName)));

#endif
}

void MainWindow::setupEditor()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    editor = new QTextEdit;
    editor->setFont(font);

    //highlighter = new Highlighter(editor->document());
}

void MainWindow::setupFileMenu()
{
    QMenu *fileMenu = new QMenu(tr("&File"), this);
    menuBar()->addMenu(fileMenu);

    fileMenu->addAction(tr("&Open..."), this, SLOT(fileOpen()), QKeySequence::Open);
    fileMenu->addAction(tr("E&xit"), qApp, SLOT(quit()), QKeySequence::Quit);
}

void MainWindow::setupPrintMenu()
{
#ifndef QT_NO_PRINTER
    QMenu *printMenu = new QMenu(tr("&Print"), this);
    menuBar()->addMenu(printMenu);

    printMenu->addAction(tr("&Print..."), this, SLOT(filePrint()), QKeySequence::Print);
    printMenu->addAction(tr("&Export PDF..."), this, SLOT(filePrintPdf()), Qt::CTRL + Qt::Key_D);

#endif
}

void MainWindow::setupHelpMenu()
{
    QMenu *helpMenu = new QMenu(tr("&Help"), this);
    menuBar()->addMenu(helpMenu);

    helpMenu->addAction(tr("&About"), this, SLOT(about()));
    helpMenu->addAction(tr("About &Qt"), qApp, SLOT(aboutQt()));
}

void MainWindow::setupEmulator()
{
    ch8Decoder = new Chip8Emu(this);

    connect(this,&MainWindow::fileLoaded,
            ch8Decoder,&Chip8Emu::loadData2Memory);

    connect(this,&MainWindow::startDisassembler,
            ch8Decoder,&Chip8Emu::startDisassembler);

    connect(ch8Decoder,&Chip8Emu::assemblerTextListing,
            this,&MainWindow::showAsmText);
}

MainWindow::~MainWindow()
{

}

QPoint MainWindow::calcDeskTopCenter(int width,int height)
{

    QPoint centerWindow;
    //получаем прямоугольник с размерами как у экрана
    centerWindow = desktopRect.center(); //получаем координаты центра экрана
    centerWindow.setX(centerWindow.x() - (width / 2) );
    centerWindow.setY(centerWindow.y() - (height / 2) );
    return centerWindow;
}

