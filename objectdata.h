
#ifndef TS_OBJECTDATA_H
#define TS_OBJECTDATA_H

#include <QString>
#include <QTextStream>
#include "types.h"
#include "utils.h"


namespace ts
{

  class ObjectData
  {
  public:
    enum Type { Object_Invalid = -1, Object_Character = 0, Object_Mob = 1, Object_Room = 2,
      Object_Item = 3, Object_Shop = 4, Object_Zone = 5 };

    virtual void init() {}
    virtual void validate() {}

    ObjectData();
    ObjectData( Type );
    ObjectData( Type, VNumber, const QString& object_name = "" );
    ObjectData( const ObjectData& );

    virtual ~ObjectData();

    virtual ObjectData& operator=( const ObjectData& );
    virtual bool operator<( const ObjectData& od ) const { return m_vnumber < od.m_vnumber; }
    virtual bool operator>( const ObjectData& od ) const { return m_vnumber > od.m_vnumber; }
    virtual bool operator==( const ObjectData& od ) const { return m_vnumber == od.m_vnumber; }

    Type type() const { return m_type; }
    void setType( Type new_type ) { m_type = new_type; }
    static QString typeName( Type );
    QString typeName() const { return typeName( m_type ); }

    VNumber vnumber() const { return m_vnumber; }
    void setVNumber( VNumber new_vnumber ) { m_vnumber = new_vnumber; }
    const QString& name() const { return m_name; }
    void setName( const QString& new_name ) { m_name = Utils::removeTilde( new_name.simplified() ); }

    bool isValid() const { return m_type != Object_Invalid; }
    bool isCharacter() const { return m_type == Object_Character; }
    bool isMob() const { return m_type == Object_Mob; }
    bool isRoom() const { return m_type == Object_Room; }
    bool isItem() const { return m_type == Object_Item; }
    bool isShop() const { return m_type == Object_Shop; }
    bool isZone() const { return m_type == Object_Zone; }

    virtual QString defaultName() const { return typeName() + QString::number( m_vnumber ); }
    virtual QString dumpObject() const;

    virtual void load( QTextStream& ) {}
    virtual void save( QTextStream& ) {}

  protected:
    Type m_type;
    VNumber m_vnumber;
    QString m_name;

  };

}  // namespace ts

#endif // TS_OBJECTDATA_H

