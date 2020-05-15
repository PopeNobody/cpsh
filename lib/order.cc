#include <bittrex/order.hh>
#include <dbg.hh>
#include <value/date.hh>
#include <bittrex.hh>

#include <util.hh>
using value::date_t;

static void fuck() {
  exception e;
  cout << e;
};
template<typename mem_t>
void assign_to(mem_t &dst, const string &val, const string &id)
{
  try {
    // FIXME
    if(val!="")
      dst=lexical_cast<mem_t>(val);
  } catch ( exception &ex ) {
    xexpose(id);
    xexpose(val);
    xexpose(ex);
  };
};
template<>
void assign_to<bool>(bool &dst, const string &val, const string &id)
{
  try {
    bool b;
    if(val.length()) {
      if(tolower(val[0])=='t' || tolower(val[0])=='y')
        b=true;
      else if(tolower(val[0])=='f' || tolower(val[0])=='n')
        b=false;
      else
        xcarp(" how do I make '" << val << "' into a bool?");
    } else {
      b=false;
    };
    dst=b;
  } catch ( exception &ex ) {
    cout << "id: " << id << endl;
    cout << "val: " << val << endl;
    cout << ex << endl;
  };
};
template<>
void assign_to<date_t>(date_t &dst, const string &val, const string &id)
{
  try {
    date_t date(val);
    dst=date;
  } catch ( exception &ex ) {
    cout << "id: " << id << endl;
    cout << "val: " << val << endl;
    cout << ex << endl;
  };
};
namespace bittrex {
  order_m order_t::dbload( connection& con )
  {
    order_m orders;
    work work(con);
    auto set=work.exec("select * from ord;");
    for( auto const &row : set )
    {
      order_t order;
      int i=0;
#define item(t,x,y) assign_to(order.data.x,row[i++].c_str(),#t);
      order_attrs();
#undef item
      orders[order.data.uuid]=order;
    };
    return orders;
  };
  order_t::order_t()
  {
  };
  order_t::~order_t()
  {
  };
  ostream &order_t::stream(ostream &lhs) const
  {
    json rend;
    to_json(rend,*this);

    return lhs << setw(4) << rend << endl;
  };
}
inline std::ostream &operator<<(std::ostream &lhs, const std::exception &rhs);
namespace bittrex {
  ///  attr_m json_type_map=order_t::mk_json_type_map();
  order_m order_t::read_csv(const string &csvfile)
  {
    xcheckin();
    order_m orders;
    ifstream csv(csvfile);
    string line;
    if(!getline(csv,line))
      xcroak("failed to read header line");

    line=util::strip(line); 
    int i=0;
    map<string,int> col_ids;
    for( auto col : util::split(',',line) )
      col_ids[col]=i++;
    while(getline(csv,line))
    {
      try {
        map<string,string> pairs;
        vector<string> vals=util::split(',',line);
        order_t order;
#define item(t,x,y) assign_to(order.data.x,vals[col_ids[y]],y);
        order_attrs();
#undef item
        orders[order.uuid()]=order;
      } catch ( const exception &ex ) {
        exception ex2;
        xcarp( ex2 );
      };
    };
    return orders;
  };
//     string order_t::cpp_to_sql(const string type)
//     {
//       if(type=="string")
//         return "text";
//       if(type=="double")
//         return "numeric(16,8)";
//       if(type=="bool")
//         return "boolean";
//       if(type=="date_t")
//         return "timestamp";
//       return type;
//     };
//     string order_t::table_create_sql() {
//       ostringstream sql;
//       string term="";
//       sql << "create table ord (\n";
//   #define item(t,x,y) sql << "  " << #x << "  " << cpp_to_sql(#t) << ",\n";
//       order_attrs();
//   #undef item
//       sql << "  primary key ( uuid )\n";
//       sql << "\n);";
//       term="";
//       return sql.str();
//     };
};
void bittrex::order_t::dbstore( connection& con, const order_m& orders )
{
  exception ex;
  ostream &str=cout;
  //FIXME
#if 0
  ostringstream sql;
  for ( auto pair : orders )
  {
    sql << pair.second.to_sql();
  };
  try {
    work work( con );
    work.exec( sql.str() );
    work.commit();
  } catch ( const exception &ex ) {
    xtrace(ex);
    throw;
  };
#endif
};
bittrex::order_m bittrex::order_t::list;
bittrex::order_m &bittrex::order_t::load()
{
  const static string url( api_url + "account/getorderhistory?" );
  order_m orders;
  auto jpage = load_json_url(url, "log/order.json");
  bittrex::from_json(jpage,orders);
  cout << orders.size() << " orders from web" << endl;
  int count=0;
  orders.insert(list.begin(),list.end());
  swap(list,orders);
  return list;
};
string to_sql(const bittrex::order_t &ord) 
{
  ostringstream sql;
  string term="";
  sql << "insert into ord (\n";
#define item(t,x,y) sql << term << "  " << #x; term=",\n";
  order_attrs();
#undef item
  sql << "\n) values (\n";
  term="";
#define item(t,x,y) sql << term << "  " << db::dbquote(ord.x()); term=",\n";
  order_attrs();
#undef item
  sql
    << "\n)\n"
    << " on conflict do nothing\n;"
    ;
  return sql.str();
};
