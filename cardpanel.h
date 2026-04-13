#ifndef CARDPANEL_H
#define CARDPANEL_H

#include <QWidget>
#include <QPainter>
#include "card.h"
#include "player.h"

class CardPanel : public QWidget
{
    Q_OBJECT
private:
    bool m_isFront;
    bool m_isSelected;
    QPixmap m_front;
    QPixmap m_back;
    Card m_card;
    Player* m_owner;

public:
    explicit CardPanel(QWidget* parent = nullptr);

    // 设置获取图片相关
    inline void setImage(const QPixmap& front, const QPixmap& back)
    {
        m_front = front;
        m_back = back;

        setFixedSize(m_front.size());
        update();
    }
    QPixmap getImage() const { return m_front; }

    // 扑克牌显示的面相关
    inline void setFrontSide(bool isFront) { m_isFront = isFront; }
    inline bool isFrontSide() const { return m_isFront; }

    // 选中相关
    inline void setSelected(bool isSelected) { m_isSelected = isSelected; }
    inline bool isSelected() const { return m_isSelected; }

    // 花色和点数相关
    inline void setCard(const Card& card) { m_card = card; }
    inline Card getCard() const { return m_card; }

    // 所有者相关
    inline void setOwner(Player* owner) { m_owner = owner; }
    inline Player* getOwner() const { return m_owner; }

    // 模拟扑克牌的点击事件
    inline void clicked() { emit cardSelected(Qt::LeftButton); };

protected:
    void paintEvent(QPaintEvent* ev) override;
    void mousePressEvent(QMouseEvent* ev) override;

signals:
    void cardSelected(Qt::MouseButton button);
};

#endif // CARDPANEL_H
