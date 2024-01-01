#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    totalCoins = 10;
    gameIsOver = false;

    ui->setupUi(this);
    ui->totalCoins_middle->setText(QString("%1").arg(totalCoins));
    ui->currentBet_right->setMaximum(totalCoins);

    ALL_SYMBOLS = {"🍒", "🎈", "🌙", "🍀", "💎", "🎩", "🚀", "🎁", "⭐", "🌈"};
    WHEEL_SIZE = ALL_SYMBOLS.size();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_lastRow_right_clicked()
{
    ui->currentBet_right->setReadOnly(true);
    ui->lastRow_group->hide();
    loopAnimation_and_getAllSlotValues();

    int currentBet = ui->currentBet_right->value();
    int award = calculateAward(currentBet, allSlots);
    bool lastSpinEarnedCoins = (award > 0);
    totalCoins = calculateNewCoins(currentBet, award, lastSpinEarnedCoins);
    gameIsOver = (totalCoins <= 0);
    QString leftText, rightText;

    if(gameIsOver) {
        leftText = "0 coins left! Game over!";
        rightText = "Exit";
    }
    else {
        leftText = "You just %1 %2 coin%3!";

        if(lastSpinEarnedCoins) {
            leftText = leftText.arg("earned").arg(award).arg("s");
        }
        else {
            QString possibleS = (currentBet == 1) ? "" : "s";
            leftText = leftText.arg("lost").arg(currentBet).arg(possibleS);
        }
        rightText = "Continue";
    }
    ui->stack_group->setCurrentIndex(1);
    ui->message_left->setText(leftText);
    ui->message_right->setText(rightText);
    QCoreApplication::processEvents();
    int putBreakpointHere = 0;
}

void MainWindow::loopAnimation_and_getAllSlotValues()
{
    const int LOOP_COUNT = 120;
    const int LOOP_COUNT_PER_SLOT = LOOP_COUNT / 3;

    for(int loopCount = 1; loopCount <= LOOP_COUNT; loopCount++) {
        QEventLoop wait;
        QTimer::singleShot(50, &wait, &QEventLoop::quit);
        wait.exec();

        if(loopCount <= LOOP_COUNT_PER_SLOT)
        {
            updateSlot_andPossibly_addToFinalVector(ui->allSlots_left, loopCount, LOOP_COUNT_PER_SLOT);
        }
        if(loopCount <= LOOP_COUNT_PER_SLOT * 2)
        {
            updateSlot_andPossibly_addToFinalVector(ui->allSlots_middle, loopCount, LOOP_COUNT_PER_SLOT * 2);
        }
        if(loopCount <= LOOP_COUNT_PER_SLOT * 3)
        {
            updateSlot_andPossibly_addToFinalVector(ui->allSlots_right, loopCount, LOOP_COUNT_PER_SLOT * 3);
        }
    }
}

void MainWindow::updateSlot_andPossibly_addToFinalVector(QLabel* currentSlot, int loopCount, int nextEnding) {
    int i = (rand() % WHEEL_SIZE);
    currentSlot->setText(ALL_SYMBOLS[i]);
    currentSlot->repaint();

    if(loopCount == nextEnding) {
        allSlots.push_back(i);
    }
}

int MainWindow::calculateAward(const int currentbet, const QVector<int>& allSlots)
{
    int mulitiplier = 0;

    if(allSlots[0] == allSlots[1] && allSlots[1] == allSlots[2]) {
        mulitiplier = 4;
    }
    else if(allSlots[0] == allSlots[1] || allSlots[1] == allSlots[2] || allSlots[0] == allSlots[2]) {
        mulitiplier = 2;
    }
    return (currentbet * mulitiplier);
}

int MainWindow::calculateNewCoins(int currentBet, int award, int lastSpinEarnedCoins)
{
    int newCoins;

    if(lastSpinEarnedCoins) {
        newCoins = totalCoins + award;
    }
    else {
        newCoins = totalCoins - currentBet;
    }
    return newCoins;
}

void MainWindow::on_message_right_clicked()
{
    if(gameIsOver) {
        exit(0);
    }
    else {
        allSlots.clear();
        ui->allSlots_left->setText("💲");
        ui->allSlots_middle->setText("💲");
        ui->allSlots_right->setText("💲");
        ui->totalCoins_middle->setText(QString("%1").arg(totalCoins));
        ui->currentBet_right->setMaximum(totalCoins);
        ui->currentBet_right->setReadOnly(false);
        ui->currentBet_right->setValue(1);
        ui->stack_group->setCurrentIndex(0);
        QCoreApplication::processEvents();
    }
}
