#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFileDialog>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("The Speechwriter");
    setupContentManager();
    setupTableView();
    setupStatusBar();
    connectControls();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupContentManager()
{
    connect(&m_contentManager, &ContentManager::selectedRowChanged, this, &MainWindow::selectRow);
}

void MainWindow::setupTableView()
{
    ui->tableView->setModel(m_contentManager.model());
    ui->tableView->setColumnWidth(static_cast<int>(SpeechwriterTableModel::Columns::Order), 50);
    ui->tableView->setColumnWidth(static_cast<int>(SpeechwriterTableModel::Columns::Text), 300);
    ui->tableView->setColumnWidth(static_cast<int>(SpeechwriterTableModel::Columns::Status), 100);
    ui->tableView->setColumnWidth(static_cast<int>(SpeechwriterTableModel::Columns::IsFirst), 100);
}

void MainWindow::setupStatusBar()
{
    m_sentencesLabel = new QLabel{this};
    m_duplicatesLabel = new QLabel{this};
    m_paragraphsLabel = new QLabel{this};
    statusBar()->addWidget(m_sentencesLabel);
    statusBar()->addWidget(m_duplicatesLabel);
    statusBar()->addWidget(m_paragraphsLabel);

    connect(m_contentManager.model(), &SpeechwriterTableModel::rowsInserted, this, [this]()
    {
        m_sentencesLabel->setText(s_sentencesCount.arg(QString::number(m_contentManager.rowCount())));
        m_duplicatesLabel->setText(s_duplicatesCount.arg(QString::number(m_contentManager.duplicatesCount())));
        m_paragraphsLabel->setText(s_paragraphsCount.arg(QString::number(m_contentManager.firstSentencesCount())));
    });

    connect(m_contentManager.model(), &SpeechwriterTableModel::rowsRemoved, this, [this]()
    {
        m_sentencesLabel->setText(s_sentencesCount.arg(QString::number(m_contentManager.rowCount())));
        m_duplicatesLabel->setText(s_duplicatesCount.arg(QString::number(m_contentManager.duplicatesCount())));
        m_paragraphsLabel->setText(s_paragraphsCount.arg(QString::number(m_contentManager.firstSentencesCount())));
    });

    connect(m_contentManager.model(), &SpeechwriterTableModel::dataChanged, this, [this]()
    {
        m_sentencesLabel->setText(s_sentencesCount.arg(QString::number(m_contentManager.rowCount())));
        m_duplicatesLabel->setText(s_duplicatesCount.arg(QString::number(m_contentManager.duplicatesCount())));
        m_paragraphsLabel->setText(s_paragraphsCount.arg(QString::number(m_contentManager.firstSentencesCount())));
    });

    connect(&m_contentManager, &ContentManager::message, this, [this](const QString &message)
    {
        statusBar()->showMessage(message, 3000);
    });
}

void MainWindow::connectControls()
{
    connect(ui->addSentencePushButton, &QPushButton::clicked, &m_contentManager, &ContentManager::addSentence);

    connect(ui->removeSentencePushButton, &QPushButton::clicked, this, [this]()
    {
        m_contentManager.removeSentence(ui->tableView->selectionModel()->selectedIndexes());
    });

    connect(ui->moveUpPushButton, &QPushButton::clicked, this, [this]()
    {
        m_contentManager.moveSentenceUp(ui->tableView->selectionModel()->selectedIndexes());
    });

    connect(ui->moveDownPushButton, &QPushButton::clicked, this, [this]()
    {
        m_contentManager.moveSentenceDown(ui->tableView->selectionModel()->selectedIndexes());
    });

    connect(ui->nextDuplicatepushButton, &QPushButton::clicked, this, [this]()
    {
        m_contentManager.goToNextDuplicate(ui->tableView->selectionModel()->selectedIndexes());
    });

    connect(ui->previousDuplicatepushButton, &QPushButton::clicked, this, [this]()
    {
        m_contentManager.goToPreviousDuplicate(ui->tableView->selectionModel()->selectedIndexes());
    });

    connect(ui->importDataPushButton, &QPushButton::clicked, this, &MainWindow::importSentences);

    connect(ui->exportDataPushButton, &QPushButton::clicked, this, &MainWindow::exportSentences);
}

void MainWindow::selectRow(int row)
{
    QModelIndex index{m_contentManager.getIndexAtRow(row)};
    ui->tableView->selectRow(row);
    ui->tableView->scrollTo(index, QAbstractItemView::PositionAtTop);
    ui->tableView->setFocus();
}

void MainWindow::importSentences()
{
    QString filePath{QFileDialog::getOpenFileName(this, "Import Sentences From A Text File",
                                                  QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                                  "Text Files (*.txt);;All Files (*)")};

    m_contentManager.importSencences(filePath);
}

void MainWindow::exportSentences()
{
    QString filePath{QFileDialog::getSaveFileName(this, "Save Sentences As A Text File",
                                                  QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                                  "Text Files (*.txt);;All Files (*)")};

    m_contentManager.exportSentences(filePath);

}
