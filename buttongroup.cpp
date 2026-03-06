#include "buttongroup.h"
#include "ui_buttongroup.h"

ButtonGroup::ButtonGroup(QWidget* parent)
    : QWidget(parent), ui(new Ui::ButtonGroup)
{
    ui->setupUi(this);
}

ButtonGroup::~ButtonGroup()
{
    delete ui;
}

void ButtonGroup::initButtons()
{
    // 开始游戏
    ui->start->setImage(":/images/start-1.png", ":/images/start-3.png", ":/images/start-2.png");
    // 出牌
    ui->playCard->setImage(":/images/chupai_btn-1.png", ":/images/chupai_btn-3.png", ":/images/chupai_btn-2.png");
    ui->playCard1->setImage(":/images/chupai_btn-1.png", ":/images/chupai_btn-3.png", ":/images/chupai_btn-2.png");
    // 不要
    ui->pass->setImage(":/images/pass_btn-1.png", ":/images/pass_btn-3.png", ":/images/pass_btn-2.png");
    // 不抢
    ui->giveUp->setImage(":/images/buqiang-1.png", ":/images/buqiang-3.png", ":/images/buqiang-2.png");
    // 抢 1,2,3分
    ui->oneScore->setImage(":/images/1fen-1.png", ":/images/1fen-3.png", ":/images/1fen-2.png");
    ui->twoScore->setImage(":/images/2fen-1.png", ":/images/2fen-3.png", ":/images/2fen-2.png");
    ui->threeScore->setImage(":/images/3fen-1.png", ":/images/3fen-3.png", ":/images/3fen-2.png");

    // 设置按钮的大小
    QVector<MyButton*> btns;
    btns << ui->start << ui->playCard << ui->playCard1 << ui->pass << ui->giveUp << ui->oneScore << ui->twoScore << ui->threeScore;
    int size = (int)btns.size();
    for (int i = 0; i < size; ++i)
    {
        btns[i]->setFixedSize(90, 45);
    }
}
