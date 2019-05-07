
#ifndef TS_DICE_H
#define TS_DICE_H

#include <QString>


namespace ts
{

  class Dice
  {
  public:
    void init();

    Dice();
    explicit Dice( const QString& sDiceXdYplusZ );
    Dice( const Dice& );

    virtual ~Dice();

    Dice& operator=( const Dice& );

    int num() const { return m_num; }
    int faces() const { return m_faces; }
    int bonus() const { return m_bonus; }
    void setNum( int new_value ) { m_num = new_value; }
    void setFaces( int new_value ) { m_faces = new_value; }
    void setBonus( int new_value ) { m_bonus = new_value; }

    QString toString() const;

    int roll() const;

  protected:
    int m_num;
    int m_faces;
    int m_bonus;

  };


} // namespace ts

#endif // TS_DICE_H
