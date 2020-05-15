#include <bittrex.hh>
#include <bittrex/order.hh>
#include <dbg.hh>
#include <fmt.hh>
#include <iostream>
#include <json.hh>
#include <regex>
#include <util.hh>
#include <value.hh>
#include <web_api.hh>

using namespace std;

using value::date_t;

using namespace bittrex;
  string to_sql(const order_t &ord) ;
int xmain( vector< string >& args )
{
  using namespace bittrex;
  util::split_stream( "log/goh.log" );
  const static string url( api_url + "account/getorderhistory?" );
  vector< string > files;
  string database;
  bool query_web = true;
  for ( int i = 0; i < args.size(); i++ )
  {
    auto& arg = args[ i ];
    if ( arg.size() >= 2 )
    {
      if ( arg[ 0 ] == '-' )
      {
        if ( arg[ 1 ] == 'f' )
        {
          if ( arg.size() > 2 ) {
            files.push_back( arg.substr( 2 ) );
          } else if ( ++i < args.size() ) {
            files.push_back( args[ i ] );
          } else {
            xthrowre( "-f given without a file" );
          };
        } else if ( arg[ 1 ] == 'd' ) {
          if(database.size())
            xthrowre( "database given already!" );
        } else {
          xthrowre( "-f and -d are the only flags I know!" );
        }
      } else if ( !database.size() ) {
        database=args[i];
      } else {
        xthrowre( " bare arg given, and we already have db name");
      };
    };
  };
  string constr = db::connect_string( database );
  connection con( constr );
  //create_order_table( con );
  order_m orders = order_t::dbload( con );
  cout << orders.size() << " orders from db" << endl;
  if ( files.size() )
  {
    for( auto &file : files ) {
      cout << "reading: " << file << endl;
      order_m orders2 = order_t::read_csv( file );
      cout << orders2.size() << " orders from file" << endl;
      int count=0;
      for ( auto pair : orders2 ) {
        auto itr=orders.find(pair.first);
        if(itr==orders.end()) {
          orders.insert( pair );
          count++;
        }
      };
      cout << count << " NEW orders from file" << endl;
    };
    cout << orders.size() << " total orders;" << endl;
  } else if ( database.size() ) {
  };
  if ( query_web )
  {
    order_m orders2;
    auto jpage = load_json_url(url, "log/order.json");
    bittrex::from_json(jpage,orders2);
    cout << orders2.size() << " orders from web" << endl;
    int count=0;
    for ( auto pair : orders2 ) {
      auto itr=orders.find(pair.first);
      if(itr==orders.end()) {
        orders.insert( pair );
        count++;
      }
    };
    cout << count << " NEW orders from web" << endl;
    cout << orders.size() << " total orders;" << endl;
  };
  {
    work work( con );
    cout << db::single_query( work, "select count(*) from ord;\n" )
      << " orders in database"
      << endl;
  };
  for ( auto pair : orders )
  {
    work work( con );
    string sql( to_sql( pair.second ) );
    work.exec( sql );
    work.commit();
  };
  cout << orders.size() << " orders loaded and dumped\n" << endl;
  {
    work work( con );
    cout << db::single_query( work, "select count(*) from ord;\n" )
      << " orders in database"
      << endl;
  };

  return 0;
};
