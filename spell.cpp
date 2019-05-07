
#include "spell.h"

namespace ts
{

  Spell::Spell()
  {
    m_id = 0;
    m_name = "unknown";
  }
    
  Spell::Spell( int id, const QString& name )
  {
    m_id = id;
    m_name = name;
  }
    
  Spell::Spell( const Spell& s )
  {
    m_id = s.m_id;
    m_name = s.m_name;
  }
    
  Spell::~Spell()
  {}

  Spell& Spell::operator=( const Spell& s )
  {
    if( this != &s )
    {
      m_id = s.m_id;
      m_name = s.m_name;
    }
    return *this;
  }

} // namespace ts
