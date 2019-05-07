
#ifndef TS_REPORT_H
#define TS_REPORT_H

#include <QString>
#include "area.h"


namespace ts
{

  class Report
  {
  public:
    static QString coins( const Area& );
    static QString treasures( const Area& );
    static QString exits( const Area& );
    static QString keys( const Area& );
    static QString deathRooms( const Area& );

  protected:
    Report() {}
    virtual ~Report() {}

  };

} // namespace ts

#endif // TS_REPORT_H
