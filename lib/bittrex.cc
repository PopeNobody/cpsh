#include <bittrex.hh>
#include <db.hh>
#include <dbg.hh>
#include <util.hh>
#include <web_api.hh>
#include <util.hh>

using namespace bittrex;
using namespace pqxx;
using namespace db;
using namespace util;
namespace bittrex {
  bool fake_loads=false;
  void save_json( const string& fname, const json& json );
  const json load_json_url( const string& url, const string& save_to );
};
void bittrex::save_json( const string& fname, const json& json )
{
  xassert( fname.length() );
  ofstream ofile;
  {
    xunlink(fname.c_str());
    int fd = util::open_log( fname );
    ofile.open( fname, ios::app );
    xclose( fd );
  };
  if ( !ofile )
    xthrowre( "open:" + fname + strerror( errno ) );
  ofile << setw( 4 ) << json << endl;
  if ( !ofile )
    xthrowre( "error writing " + fname );
};
const json bittrex::load_json_url( const string& url, const string& save_to )
{
  //xtrace( __PRETTY_FUNCTION__ << "(url=" << url << ", dest=" << save_to );
  xassert( save_to.length() );
  try
  {
    string page;
    if ( bittrex::fake_loads )
    {
      xtrace( "using cached page at: " << save_to );
      ifstream str( save_to );
      if ( !str )
        xthrowre( "open:" + save_to + ":" + strerror( errno ) );
      string line;
      while ( getline( str, line ) )
        page += line;
    }
    else
    {
      page = web::load_hmac_page( url );
    };
    json jpage = json::parse( page );
    save_json( save_to, jpage );
    if ( !jpage.at( "success" ) )
    {
      throw runtime_error( "no success in result\n\n" + page );
    };
    jpage = *jpage.find( "result" );
    return jpage;
  }
  catch ( exception& ex )
  {
    xtrace( ex.what() );
    xexpose( url );
    xexpose( save_to );
    throw;
  }
  catch ( ... )
  {
    throw;
  };
};
void order_t::dbstore( connection& con )
{
  dbstore( con, list );
};
void bal_t::dbstore( connection& con )
{
  dbstore( con, list );
};
namespace db {
  string dbquote(const bittrex::sym_t &rhs)
  {
    return dbquote(string(rhs));
  };
};
void bal_t::dbstore( connection& con, const bal_m& bals )
{
  work work( con );
  ostringstream sql;
  sql << "delete from bal_n;\n\n";
  for ( auto pair : bal_t::load() )
  {
    const auto& bal = pair.second;
    sql << "insert into bal_n values(" << endl;
    sql << "   " << db::dbquote( bal.sym() ) << ",\n";
    sql << "   " << bal.bal().val() << ",\n";
    sql << "   " << bal.pend().val() << ",\n";
    sql << "   " << bal.avail().val() << "\n";
    sql << ");" << endl;
  };
  sql << " delete from bal where (sym) not in (select sym from bal_n);\n\n"
         " delete from bal_n where (sym,bal,pend,avail ) in (select * "
         "from bal);\n\n"
         " insert into bal\n"
         "  (select * from bal_n)\n"
         "   on conflict(sym)\n"
         "  do\n"
         "   update set (bal,pend,avail) =\n"
         "         (excluded.bal, excluded.pend, excluded.avail);\n\n"
         " delete from bal_n where (sym,bal,pend,avail ) in (select * "
         "from bal);\n\n"
         "\n\n";
  exec( work, sql.str() );
  work.commit();
}
