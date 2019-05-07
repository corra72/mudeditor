
#ifndef TS_ZONECOMMAND_H
#define TS_ZONECOMMAND_H

#include <QString>
#include <QTextStream>
#include <stdio.h>
#include "types.h"


namespace ts
{

  class ZoneCommand
  {
  public:
    enum Argument { Argument0 = 0, Argument1, Argument2, Argument3, Argument4,
                    LastArgument };

    void initArguments();
    void init();
    void validate();

    ZoneCommand();
    ZoneCommand( const ZoneCommand& );

    virtual ~ZoneCommand();

    virtual ZoneCommand& operator=( const ZoneCommand& );
    // Useful only in DoorInits
    virtual bool operator<( const ZoneCommand& zc ) const { return priority() < zc.priority(); }
    virtual bool operator>( const ZoneCommand& zc ) const { return priority() > zc.priority(); }

    QChar type() const { return m_type; }
    long argument( Argument arg ) const { return mp_args[ arg ]; }
    bool hasArgument( Argument arg ) const { return mp_args[ arg ] >= 0; }
    VNumber id() const { return m_id; }
    QString comment() const { return m_comment; }
    QString commentSeparator() const { return m_commentSeparator; }
    void setType( QChar new_type ) { m_type = new_type.toUpper(); initArguments(); }
    void setArgument( Argument arg, long new_value ) { mp_args[ arg ] = new_value; }
    void setId( VNumber new_value ) { m_id = new_value; }
    void setComment( const QString& txt ) { m_comment = txt.simplified(); }
    void setCommentSeparator( const QString& sep ) { m_commentSeparator = sep.trimmed(); }

    bool isMobLoad() const { return m_type == 'M'; }
    bool isMobFollower() const { return m_type == 'C'; }
    bool isMobFear() const { return m_type == 'F'; }
    bool isMobHate() const { return m_type == 'H'; }
    bool isMobEquip() const { return m_type == 'E'; }
    bool isMobGive() const { return m_type == 'G'; }
    bool isItemLoad() const { return m_type == 'O'; }
    bool isItemPut() const { return m_type == 'P'; }
    bool isDoorInit() const { return m_type == 'D'; }
    bool isOnlyComment() const { return m_type == '*' || m_type == ';'; }
    bool isZoneEnd() const { return m_type == 'S'; }

    bool isValidType() const { return isMobLoad() || isMobFollower() ||
                                 isMobFear() || isMobHate() || isItemPut() || isMobEquip() ||
                                 isMobGive() || isItemLoad() || isDoorInit() ||
                                 isOnlyComment(); }

    bool isMobCommand() const { return isMobLoad() || isMobFollower() ||
                                  isMobFear() || isMobHate() || isMobEquip() || isMobGive(); }

    bool isItemCommand() const { return isItemPut() || isItemLoad(); }

    bool isEmptyCommand() const { return isOnlyComment() && m_comment.trimmed().isEmpty(); }

    bool hasParent() const { return isMobFear() || isMobHate() || isItemPut() ||
                               isMobEquip() || isMobGive(); }

    bool hasMaxField() const { return isMobLoad() || isMobFollower() || isItemPut() ||
                               isMobEquip() || isMobGive() || isItemLoad(); }

    bool hasMobField() const { return isMobLoad() || isMobFollower(); }

    bool hasItemField() const { return isItemPut() || isMobEquip() || isMobGive() ||
                                  isItemLoad(); }

    QString dumpObject() const;

    void load( FILE* );
    void save( QTextStream& ) const;
    void loadComment( const QString& );

    QString toString( bool append_comment ) const;

    int priority() const;

  protected:
    QChar m_type;
    long mp_args[ LastArgument ];
    int m_numArgs;
    VNumber m_id;
    QString m_comment;
    QString m_commentSeparator;
  };

} // namespace ts


#endif // TS_ZONECOMMAND_H
