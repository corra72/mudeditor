#include "dice.h"
#include <stdio.h>
#include "random.h"

namespace ts
{
  void Dice::init()
  {
    m_num = 1;
    m_faces = 1;
    m_bonus = 0;
  }

  Dice::Dice()
  {
    init();
  }

  Dice::Dice( const QString& sDiceXdYplusZ )
  {
    init();
    sscanf( sDiceXdYplusZ.toUtf8().data(), "%dd%d+%d", &m_num, &m_faces, &m_bonus );
  }

  Dice::Dice( const Dice& d )
  {
    m_num = d.m_num;
    m_faces = d.m_faces;
    m_bonus = d.m_bonus;
  }

  Dice::~Dice()
  {
  }

  Dice& Dice::operator=( const Dice& d )
  {
    m_num = d.m_num;
    m_faces = d.m_faces;
    m_bonus = d.m_bonus;
    return *this;
  }

  QString Dice::toString() const
  {
    QString sRet = "";
    sRet.sprintf( "%dd%d+%d", m_num, m_faces, m_bonus );
    return sRet;
  }

  int Dice::roll() const
  {
    return Random::dice( m_num, m_faces, m_bonus );
  }

} // namespace ts
