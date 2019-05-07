
#ifndef TS_ITEMAFFECT_H
#define TS_ITEMAFFECT_H


#include <QString>
#include <QTextStream>
#include <stdio.h>
#include "types.h"
#include "constants.h"


namespace ts
{

  class ItemAffect
  {
  public:
    ItemAffect();
    ~ItemAffect();
    ItemAffect( const ItemAffect& );

    ItemAffect& operator=( const ItemAffect& );

    bool isValid() const { return !isNull(); }
    bool isNull() const { return m_type == ITEM_AFFECT_NONE; }
    void setNull() { m_type = ITEM_AFFECT_NONE; }
    int type() const { return m_type; }
    long value() const { return m_value; }
    void setType( int new_type ) { m_type = new_type; }
    void setValue( long new_value ) { m_value = new_value; }

    void load( FILE*, VNumber );
    void save( QTextStream& );

    QString dumpObject() const;
    QString typeToString() const;
    QString valueToString() const;

  private:
    int m_type;
    long m_value;

};



} // namespace ts


#endif // TS_ITEMAFFECT_H

