#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QThread>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private slots:
        void on_lastRow_right_clicked();
        void on_message_right_clicked();

    private:
        int totalCoins;
        bool gameIsOver;
        QVector<int> allSlots;
        Ui::MainWindow *ui;

        QVector<QString> ALL_SYMBOLS;
        int WHEEL_SIZE;

        void loopAnimation_and_getAllSlotValues();
        void updateSlot_andPossibly_addToFinalVector(QLabel* currentSlot, int loopCount, int nextEnding);
        int calculateAward(const int currentbet, const QVector<int>& allSlots);
        int calculateNewCoins(int currentBet, int award, int lastSpinEarnedCoins);
};
#endif
