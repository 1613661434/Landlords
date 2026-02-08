#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>

class Player : public QObject
{
    Q_OBJECT
    // == 枚举定义 ==
public:
    enum class Role : char
    {
        Load,
        Farmer
    }; // 角色

    enum class Sex : char
    {
        Man,
        Woman
    }; // 性别

    enum class Direction : char
    {
        Left,
        Right
    }; // 头像的显示方位

    enum class Type : char
    {
        Robot,
        User,
        Unknown
    }; // 玩家的类型

    // == 成员变量 ==
private:
    QString m_name;
    Role m_role;
    Sex m_sex;
    Direction m_direction;
    Type m_type;
    int m_score;
    bool m_isWin;

    // == 函数 ==
public:
    explicit Player(QObject* parent = nullptr);
    explicit Player(QString name, QObject* parent = nullptr);

    // 名字
    inline void setName(QString name) { m_name = name; }
    inline QString getName() { return m_name; }

    // 角色
    inline void setRole(Role role) { m_role = role; }
    inline Role getRole() { return m_role; }

    // 性别
    inline void setSex(Sex sex) { m_sex = sex; }
    inline Sex getSex() { return m_sex; }

    // 头像的显示方位
    inline void setDirection(Direction direction) { m_direction = direction; }
    inline Direction getDirection() { return m_direction; }

    // 玩家的类型
    inline void setType(Type type) { m_type = type; }
    inline Type getType() { return m_type; }

    // 分数
    inline void setScore(int score) { m_score = score; }
    inline int getScore() { return m_score; }

    // 游戏结果
    inline void setIsWin(bool flag) { m_isWin = flag; }
    inline bool getIsWin() { return m_isWin; }

signals:
};

#endif // PLAYER_H
