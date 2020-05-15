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
using namespace util;

using value::date_t;

json read_csv(const string &csvfile)
{
  ifstream csv(csvfile);
  string line;
  if(!getline(csv,line))
    xcroak("failed to read header line");

  line=strip(line); 
  int i=0;
  vector<string> cols=split(',',line);
  for( auto &col : cols ) {
    col=join("", split(' ', col));
  };
  json res;
  while(getline(csv,line))
  {
    try {
      vector<string> vals=split(',',line);
      json obj;
      for( int i = 0; i<vals.size();i++)
      {
        obj[cols[i]]=vals[i];
      };
      res.push_back(obj);
    } catch ( exception &ex ) {
      xcarp( ex );
    };
  };
  return res;
};

int xmain( vector< string >& args )
{
  using namespace bittrex;
  split_stream( "log/goh.log" );
  for( auto arg : args ) {
    cout << arg << endl;
    cout << setw(4) << read_csv(arg) << endl;
    cout << arg << endl;
    cout << endl << endl;
  };
  return 0;
};
