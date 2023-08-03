#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

#include "ContentManager.h"
#include "SpeechwriterTableModel.h"

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
    void setupContentManager();
    void setupTableView();
    void setupStatusBar();
    void connectControls();

    Ui::MainWindow          *ui;
    SpeechwriterTableModel  *m_tableModel;
    ContentManager          m_contentManager;
    QLabel                  *m_sentencesLabel;
    QLabel                  *m_duplicatesLabel;
    QLabel                  *m_paragraphsLabel;
    int                     m_currentDuplicateRow{-1};

    static inline QString s_sentencesCount{"Number of sentences: %1"};
    static inline QString s_duplicatesCount{"Number of duplicates: %1"};
    static inline QString s_paragraphsCount{"Number of paragraphs: %1"};
};
#endif // MAINWINDOW_H
