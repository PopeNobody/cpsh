#include <dbg.hh>
#include <value/date.hh>

using namespace std;
using util::quote;
using value::date_t;
static vector< string > date_fmts = {
  "%Y-%m-%d",
  "%m-%d-%Y",
  "%Y/%m/%d",
  "%m/%d/%Y"
};
static vector< string > seps = {
  "T", " ", ""
};
static vector< string > time_fmts = {
  "%I:%M:%S %p",
  "%I:%M %p",
  "%H:%M:%S",
  "%H:%M",
  ""
};

struct xtm : public tm
{
  xtm() { memset( this, 0, sizeof( *this ) ); };
  xtm( const tm& rhs )
      : tm( rhs ){};
};
template < typename val_t >
bool between( val_t lb, val_t v, val_t ub )
{
  return ( lb <= v ) && ( ub >= v );
};
time_t finish_time( tm& tm, const char* rest )
{
  while ( isspace( *rest ) )
    ++rest;
  const char* end = rest;
  int let = 0;
  while ( between( 'A', *end, 'Z' ) )
    ++end;
  if ( end - rest >= 3 )
    tm.tm_zone = rest;

  time_t ret = mktime( &tm );
  return ret;
}

time_t time_from_string( const string& str )
{
  xtm a_time;
  const char* pos = str.c_str();
  const char* res = 0;
  for ( auto const& dfmt : date_fmts )
  {
    for ( auto const& tfmt : time_fmts )
    {
      for ( auto const& sep : seps )
      {
        string fmt = dfmt + sep + tfmt;
        res = strptime( pos, fmt.c_str(), &a_time );
        if ( res )
          return finish_time( a_time, res );
      };
    };
  };
  xthrowre( "unable to convert " << quote( str ) << " to date" );
};
inline ostream& operator<<( ostream& lhs, const tm& rhs )
{
  using namespace std;
  ostringstream str;
  str << setfill( '0' ) << setw( 4 ) << ( 1900 + rhs.tm_year ) << '-'
      << setw( 2 ) << rhs.tm_mon << '-' << setw( 2 ) << rhs.tm_mday << ' '
      << setw( 2 ) << rhs.tm_hour << ':' << setw( 2 ) << rhs.tm_min << ':'
      << setw( 2 ) << rhs.tm_sec << ' ' << setfill( ' ' ) << setw( 4 )
      << ( rhs.tm_isdst ? "DST" : "   " ) << ' ' << setw( 12 )
      << ( rhs.tm_zone ? rhs.tm_zone : "" ) << setw( 8 ) << rhs.tm_gmtoff;

  int w = lhs.width();
  lhs << setw( w ) << str.str();
  return lhs;
};
int xmain( std::vector< std::string >& argv )
{
  if ( !argv.size() )
  {
    argv = { "2009/09/09 09:09:09 GMT",    "2009/09/09 09:09:09 EST",
             "2009/09/09 09:09:09 PST",    "2009/09/09 09:09:09 am EST",
             "2009/09/09 09:09:09 am GMT", "2009/09/09 09:09:09 pm EST",
             "2009/09/09 09:09:09 pm GMT", "2012/12/12" };
  };
  time_t val = time( 0 );
  xtm parts = *gmtime( &val );
  vector< const char* > tzs = {
    "UTC+16", "UTC+15", "UTC+14", "UTC+13", "UTC+12", "UTC+11",
    "UTC+10", "UTC+9",  "UTC+8",  "UTC+7",  "UTC+6",  "UTC+5",
    "UTC+4",  "UTC+3",  "UTC+2",  "UTC+1",  "UTC+0",  "UTC",
    "UTC-0",  "UTC-1",  "UTC-2",  "UTC-3",  "UTC-4",  "UTC-5",
    "UTC-6",  "UTC-7",  "UTC-8",  "UTC-9",  "UTC-10", "UTC-11",
    "UTC-12", "UTC-13", "UTC-14", "UTC-15", "UTC-16", "",
  };
  tzs.push_back( (char*)nullptr );
  return 0;
};
