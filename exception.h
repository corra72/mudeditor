
#ifndef TS_EXCEPTION_H
#define TS_EXCEPTION_H

#include <QString>


namespace ts
{
  class Exception
  {
  public:
    enum Type { Unknown, Runtime, LastType };

    Exception();
    Exception( Type, const QString& );
    Exception( const Exception& );

    virtual ~Exception();

    Exception& operator=( const Exception& );

    Type type() const { return m_type; }
    const QString& description() const { return m_description; }
    void setType( Type new_type ) { m_type = new_type; }
    void setDescription( const QString& new_desc ) { m_description = new_desc; }

    QString dumpObject() const;

  protected:
    Type m_type;
    QString m_description;

  };

  class XObjectNotFound : public Exception
  {
  public:
    XObjectNotFound( const QString& desc = "" )
      : Exception( Exception::Runtime, desc )
    {}
  };

} // namespace ts

#endif // TS_EXCEPTION_H

