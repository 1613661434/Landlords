#include "aboutdialog.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

AboutDialog::AboutDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("关于");
    setFixedSize(320, 300);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    // 整体背景样式
    setStyleSheet(
        "AboutDialog {"
        "  background-color: rgba(30, 30, 60, 240);"
        "  border: 2px solid rgba(255, 215, 0, 180);"
        "  border-radius: 12px;"
        "}");

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 25, 30, 20);
    mainLayout->setSpacing(10);

    // 标题
    auto* titleLabel = new QLabel("欢乐斗地主", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "QLabel {"
        "  color: #FFD700;"
        "  font-size: 22px;"
        "  font-weight: bold;"
        "  font-family: 'Microsoft YaHei';"
        "}");
    mainLayout->addWidget(titleLabel);

    auto* subtitleLabel = new QLabel("Landlords", this);
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setStyleSheet(
        "QLabel {"
        "  color: #B0B0B0;"
        "  font-size: 12px;"
        "  font-family: 'Microsoft YaHei';"
        "}");
    mainLayout->addWidget(subtitleLabel);

    // 分隔线
    auto* line1 = new QLabel(this);
    line1->setFixedHeight(2);
    line1->setStyleSheet("background-color: rgba(255, 215, 0, 100); border-radius: 1px;");
    mainLayout->addWidget(line1);

    // 原始课程信息
    auto* originLabel = new QLabel(this);
    originLabel->setText("原作者：<a href='https://subingwen.cn/' style='color:#FFD700;'>爱编程的大丙</a> Qt5 斗地主");
    originLabel->setAlignment(Qt::AlignCenter);
    originLabel->setWordWrap(true);
    originLabel->setOpenExternalLinks(true);
    originLabel->setStyleSheet(
        "QLabel {"
        "  color: #C0C0C0;"
        "  font-size: 12px;"
        "  font-family: 'Microsoft YaHei';"
        "}");
    mainLayout->addWidget(originLabel);

    // 作者改进信息
    auto* improvedLabel = new QLabel("基于原作者进行了代码优化，并适配 Qt6", this);
    improvedLabel->setAlignment(Qt::AlignCenter);
    improvedLabel->setWordWrap(true);
    improvedLabel->setStyleSheet(
        "QLabel {"
        "  color: #C0C0C0;"
        "  font-size: 12px;"
        "  font-family: 'Microsoft YaHei';"
        "}");
    mainLayout->addWidget(improvedLabel);

    // 分隔线
    auto* line2 = new QLabel(this);
    line2->setFixedHeight(2);
    line2->setStyleSheet("background-color: rgba(255, 215, 0, 100); border-radius: 1px;");
    mainLayout->addWidget(line2);

    // 作者
    auto* authorLabel = new QLabel(this);
    authorLabel->setText("作者：<a href='https://1613661434.github.io/' style='color:#FFD700;'>ol木子李lo</a>");
    authorLabel->setAlignment(Qt::AlignCenter);
    authorLabel->setOpenExternalLinks(true);
    authorLabel->setStyleSheet(
        "QLabel {"
        "  color: #E0E0E0;"
        "  font-size: 14px;"
        "  font-family: 'Microsoft YaHei';"
        "}");
    mainLayout->addWidget(authorLabel);

    // 版权
    auto* copyrightLabel = new QLabel("Copyright © 2026 ol木子李lo", this);
    copyrightLabel->setAlignment(Qt::AlignCenter);
    copyrightLabel->setStyleSheet(
        "QLabel {"
        "  color: #909090;"
        "  font-size: 12px;"
        "  font-family: 'Microsoft YaHei';"
        "}");
    mainLayout->addWidget(copyrightLabel);

    auto* licenseLabel = new QLabel("Licensed under LGPL v3.0", this);
    licenseLabel->setAlignment(Qt::AlignCenter);
    licenseLabel->setStyleSheet(
        "QLabel {"
        "  color: #808080;"
        "  font-size: 11px;"
        "  font-family: 'Microsoft YaHei';"
        "}");
    mainLayout->addWidget(licenseLabel);

    mainLayout->addStretch();

    // 关闭按钮
    auto* closeBtn = new QPushButton("关 闭", this);
    closeBtn->setFixedSize(100, 32);
    closeBtn->setCursor(Qt::PointingHandCursor);
    closeBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: rgba(255, 215, 0, 180);"
        "  color: #1E1E3C;"
        "  border: none;"
        "  border-radius: 6px;"
        "  font-size: 14px;"
        "  font-weight: bold;"
        "  font-family: 'Microsoft YaHei';"
        "}"
        "QPushButton:hover {"
        "  background-color: rgba(255, 215, 0, 220);"
        "}"
        "QPushButton:pressed {"
        "  background-color: rgba(255, 215, 0, 140);"
        "}");

    auto* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(closeBtn);
    btnLayout->addStretch();
    mainLayout->addLayout(btnLayout);

    connect(closeBtn, &QPushButton::clicked, this, &QDialog::close);
}

AboutDialog::~AboutDialog()
{
}
