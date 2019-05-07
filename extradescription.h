
#ifndef TS_EXTRADESCRIPTION_H
#define TS_EXTRADESCRIPTION_H

#include <QString>
#include <QTextStream>
#include <stdio.h>
#include "utils.h"


namespace ts
{

  class ExtraDescription
  {
  public:
    ExtraDescription();
    ExtraDescription( const QString& extra_keys, const QString& extra_desc );
    ExtraDescription( const ExtraDescription& );
    virtual ~ExtraDescription();

    ExtraDescription& operator=( const ExtraDescription& );
    const QString& keys() const { return m_keys; }
    const QString& description() const { return m_description; }

    void setKeys( const QString& new_keys ) { m_keys = Utils::removeTilde( new_keys.simplified() ); }
    void setDescription( const QString& new_description ) { m_description = Utils::removeTilde( Utils::stripSpaceAtEnd( new_description ) ); }

    QString dumpObject() const;

    void load( FILE* );
    void save( QTextStream& );

  private:
    QString m_keys;
    QString m_description;
  };

} // namespace ts

#endif // TS_EXTRADESCRIPTION_H

