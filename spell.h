#ifndef _TS_SPELL_H
#define _TS_SPELL_H

#include <qstring.h>


namespace ts
{

  class Spell
  {
  public:
    Spell();
    Spell( int, const QString& );
    Spell( const Spell& );

    virtual ~Spell();

    Spell& operator=( const Spell& );

    int id() const { return m_id; }
    const QString& name() const { return m_name; }

  protected:
    int m_id;
    QString m_name;

  };

} // namespace ts

#endif // TS_SPELL_H
