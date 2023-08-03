#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

#include "ContentManager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void selectedRowChanged(int row);

private slots:
    void selectRow(int row);
    void importSentences();
    void exportSentences();

private:
    void initializeTableModel();
    void setupTableView();
    void setupStatusBar();
    void connectControls();

    Ui::MainWindow          *ui;
    ContentManager          m_contentManager;
    QLabel                  *m_sentencesLabel;
    QLabel                  *m_paragraphsLabel;

    static inline QString s_sentencesCount{"Number of sentences: %1"};
    static inline QString s_paragraphsCount{"Number of paragraphs: %1"};
};
#endif // MAINWINDOW_H
