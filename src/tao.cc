#include <fcntl.h>
#include <iostream>
#include <json.hh>
#include <sys/stat.h>
#include <pqxx/connection.hxx>
#include <pqxx/transaction.hxx>
#include <time.h>
#include <util.hh>
#include <web_api.hh>
#include <bittrex.hh>
#include <dbg.hh>
#include <pqxx.hh>
#include <db.hh>

using namespace bittrex;
using namespace std;
using namespace pqxx;
using namespace util;
bool verbose=true;

class machine_t {
  mkt_m mkts;
  bal_m bals;
  goal_m goals;
  order_m ords;
  val_t btc_price;
  public:

  mkt_t &btc_mkt(const sym_t &sym)
  {
    xassert(sym!=sym_t("BTC"));
    auto itr=mkts.find("BTC-"+string(sym));
    if(itr==mkts.end())
      itr=mkts.find(string(sym)+"-BTC");
    xassert(itr!=mkts.end());
    return itr->second;
  };
  void dump() ;
  val_t sum_bals()
  {
    cout
      << left
      << setw(10) << "name" 
      << right
      << setw(20) << "qty"
      << setw(20) << "btc_spot"
      << setw(20) << "val_btc"
      << setw(20) << "val_usd"
      << endl;
    cout
      << fixed
      << setprecision(8);
    val_t btc_tot("BTC",0);
    val_t usd_tot("USD",0);
    for( auto b(bals.begin()), e(bals.end()); b!=e; b++ )
    { 
      auto &sym=b->first;
      auto &bal=b->second;
      auto btc=mkt_t::convert(bal.bal(),sym_t("BTC"),true);
      btc_tot+=btc;
      auto spt=mkt_t::convert(val_t(sym,1),sym_t("BTC"),true);
      auto usd=val_t("USD",btc.val()*btc_price.val());
      usd_tot+=usd;
      cout
        << left
        << setw(10) << sym
        << right
        << setw(20) << bal.bal().val()
        << setw(20) << spt.val()
        << setw(20) << btc.val()
        << setw(20) << usd.val()
        << endl;
    }
    cout
      << left
      << setw(10) << "total" 
      << right
      << setw(20) << "-"
      << setw(20) << "-"
      << setw(20) << btc_tot.val()
      << setw(20) << usd_tot.val()
      << endl;
    return btc_tot;
  };
  void run(connection &con) {
    mkts = mkt_t::load();
    goals = goal_t::load(con);
    bals = bal_t::load();
    ords = order_t::load();
    btc_price=mkt_t::convert(val_t("BTC",1),sym_t("USD"),true);
    auto sum = sum_bals();
    xexpose(sum);
    store_data(con);
  };
  void store_data(connection &con);
};

void usage() {
  cout
    << "usage: " << progname << " [dbname]\n"
    << "\n\n"
    << "if not provided on the command line, PGDATABASE must be set.\n\n"
    << endl;

  exit(0);
};
void machine_t::store_data(connection &con)
{
  try {
    mkt_t::dbstore(db::dbif(con));
  } catch ( exception &e ) {
    cout << e << endl;
    //cout << "ignored" << endl;
    throw;
  };
  try {
    bal_t::dbstore(con);
  } catch ( exception &e ) {
    cout << e << endl;
    //cout << "ignored" << endl;
    throw;
  };
  try {
    order_t::dbstore(con);
  } catch ( exception &e ) {
    cout << e << endl;
    //cout << "ignored" << endl;
    throw;
  };
  try {
    goal_t::dbstore(con);
  } catch ( exception &e ) {
    cout << e << endl;
    //cout << "ignored" << endl;
    throw;
  };
};
string mk_path(string dir, const string &file)
{
  if(!dir.size())
    dir=".";
  if(dir.back()!='/')
    dir+='/';
  dir+=file;
  return dir;
};
string get_home() {
  if(char *cp=getenv("HOME"))
    return cp;
  return "/tmp";
};

template<typename _con_t>
struct vals_t {
  typedef _con_t con_t;
  typedef typename con_t::const_iterator con_itr_t;
  typedef typename con_t::mapped_type value_type;
  typedef typename con_t::size_type size_type;
  const con_t &con;
  vals_t(const con_t &con)
    : con(con)
  {
  };
  vals_t(const vals_t &rhs)
    : con(con)
  {
  };
  struct const_iterator
  {
    con_itr_t real;
    const_iterator(const con_itr_t &real)
      : real(real)
    {
      
    };
    ~const_iterator()
    {
      
    };
    bool operator!=(const const_iterator &rhs) const
    {
      
      return real!=rhs.real;
    };
    bool operator==(const const_iterator &rhs) const
    {
      
      return real==rhs.real;
    };
    const_iterator &operator++()
    {
      
      ++real;
      return *this;
    };
    const_iterator operator++(int)
    {
      
      const_iterator res(*this);
      ++real;
      return res;
    };
    const value_type &operator*() const
    {
      return real->second;
    };
  };
  const_iterator begin()const
  {
    
    return const_iterator(con.begin());
  };
  const_iterator end() const
  {
    
    return const_iterator(con.end());
  };
  size_type size() const
  {
    
    return con.size();
  };
};
template<typename _con_t>
struct keys_t {
  typedef _con_t con_t;
  typedef typename con_t::const_iterator con_itr_t;
  typedef typename con_t::key_type value_type;
  typedef typename con_t::size_type size_type;
  const con_t &con;
  keys_t(const con_t &con)
    : con(con)
  {
  };
  keys_t(const keys_t &rhs)
    : con(con)
  {
  };
  struct const_iterator
  {
    con_itr_t real;
    const_iterator(const con_itr_t &real)
      : real(real)
    {
      
    };
    ~const_iterator()
    {
      
    };
    bool operator!=(const const_iterator &rhs) const
    {
      
      return real!=rhs.real;
    };
    bool operator==(const const_iterator &rhs) const
    {
      
      return real==rhs.real;
    };
    const_iterator &operator++()
    {
      
      ++real;
      return *this;
    };
    const_iterator operator++(int)
    {
      
      const_iterator res(*this);
      ++real;
      return res;
    };
    const value_type &operator*() const
    {
      return real->first;
    };
  };
  const_iterator begin()const
  {
    
    return const_iterator(con.begin());
  };
  const_iterator end() const
  {
    
    return const_iterator(con.end());
  };
  size_type size() const
  {
    
    return con.size();
  };
};
template<typename con_t>
vals_t<con_t> vals(const con_t &con)
{
  return vals_t<con_t>(con);
};
template<typename con_t>
keys_t<con_t> keys(const con_t &con)
{
  return keys_t<con_t>(con);
};
struct config_t {
  std::map<sym_t,double> goals;
  double min_trans_size;
  double max_trans_size;
};
int xmain(vector<string> &args)
{
  cout << fixed << setprecision(12) << boolalpha;
  try
  {
    split_stream("log/tao.log");

    string jpath="etc/tao.json";
    string jtext=read_file(jpath);
    if(!jtext.size())
      jtext="{}";
    json json=json::parse(jtext);
    write_file(jpath+".new",json.dump());
    string constr;
    if(args.size()) {
      constr=db::connect_string(args[0]);
    };
    try {
      connection con( constr );
      machine_t machine;
      machine.run(con);
    } catch ( exception &ex ) {
      xexpose(ex);
      throw;
    };
    return 0;
  }
  catch ( std::exception const& e )
  {
    xcarp(e);
    throw;
  }
  return -1;
}
