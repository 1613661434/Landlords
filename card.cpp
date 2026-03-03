#include "card.h"

Card::Card() {}

Card::Card(CardPoint point, CardSuit suit)
{
    m_point = point;
    m_suit = suit;
}
