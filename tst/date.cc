#include <web_api.hh>
#include <fmt.hh>
#include <json.hh>
#include <dbg.hh>
#include <iostream>
#include <regex>
#include <util.hh>
#include <value.hh>
#include <bittrex.hh>

using namespace std;
using namespace util;

using value::date_t;
void check_date(const date_t &date1)
{
  const string str1=lexical_cast<string>(date1);
  date_t date2=date_t(str1);
  xassert((date1>=date2));
  xassert((date1==date2));
  xassert(!(date1!=date2));
  xassert((date1<=date2));
  xassert((date1>=date2));
  xassert(!(date1>date2));
  xassert(!(date1<date2));
  date2=date_t(date1.utime()+1);
  xassert(!(date1==date2));
  xassert((date1!=date2));
  xassert((date1<=date2));
  xassert(!(date1>=date2));
  xassert(!(date1>date2));
  xassert((date1<date2));
  date2=date_t(date1.utime()-1);
  xassert(!(date1==date2));
  xassert((date1!=date2));
  xassert(!(date1<=date2));
  xassert((date1>=date2));
  xassert((date1>date2));
  xassert(!(date1<date2));

};
void check_date(const string &str)
{
  check_date(date_t(str));
};
using nlohmann::detail::value_t;
//   enum class value_t : std::uint8_t
//   {
//       null,             ///< null value
//       object,           ///< object (unordered set of name/value pairs)
//       array,            ///< array (ordered collection of values)
//       string,           ///< string value
//       boolean,          ///< boolean value
//       number_integer,   ///< number value (signed integer)
//       number_unsigned,  ///< number value (unsigned integer)
//       number_float,     ///< number value (floating-point)
//       discarded         ///< discarded by the parser callback function
//   };
static map<value_t,string> val_names = {
  { value_t::null, "null" },
  { value_t::object, "object" },
  { value_t::array, "array" },
  { value_t::string, "string" },
  { value_t::boolean, "boolean" },
  { value_t::number_integer, "number_integer" },
  { value_t::number_unsigned, "number_unsigned" },
  { value_t::number_float, "number_float" },
  { value_t::discarded, "discarded" },
};
ostream &operator<<(ostream &lhs, value_t rhs)
{
  const string &name=val_names[rhs];
  if(!name.length())
    lhs << "<unknown>";
  else
    lhs << name;
  return lhs;
};
namespace value {
    struct obj_t
    {
      string name;
      date_t dob;
      int girls;
      double weight;
    };

    void to_json(json& j, const obj_t& p) {
      j = json{
        {"name", p.name},
        {"dob", p.dob},
        {"girls", p.girls},
        {"weight", p.weight},
      };
    }

    void from_json(const json& j, obj_t& p) {
      j.at("name").get_to(p.name);
      j.at("dob").get_to(p.dob);
      j.at("girls").get_to(p.girls);
      j.at("weight").get_to(p.weight);
    }
};
template<typename val_t>
ostream &operator<<(ostream &lhs, const vector<val_t>&rhs)
{
  const char *lead="";
  lhs << "{(" << rhs.size() << "):";
  for( auto val : rhs ) {
    lhs << lead << val;
    lead=", ";
  };
  lhs << "}";
  return lhs;
};
int xmain(vector<string> &args)
{
  bool doit=true;
  setenv("TZ","GMT",true);
  if(true)
  {
    string istr1("2020/04/29 19:07:00 GMT");
    date_t date1(istr1);
    string ostr1=lexical_cast<string>(date1);

    string istr2("2020/04/29 29:07:00 GMT");
    date_t date2(istr2);
    string ostr2=lexical_cast<string>(date2);
    
  };
  if(doit)
  {
    date_t test_am("3/21/2020 4:05:06 AM");
    date_t test_pm("3/21/2020 4:05:06 PM");

    string string_am=lexical_cast<string>(test_am);
    string string_pm(lexical_cast<string>(test_pm));

    xassert(string_am=="2020-03-21 04:05:06 GMT");
    xassert(string_pm=="2020-03-21 16:05:06 GMT");
  };
  if(doit)
  {
    date_t epoch1("1970-01-01T00:00:00");
    date_t epoch2("01/01/1970 00:00:00");
    xassert(epoch1==epoch2);
    check_date(epoch1);
    check_date(epoch2);
    check_date("1968-12-05T00:01:59");
    check_date("1971-01-01T00:00:00");
    check_date("2070-01-01T00:00:00");
    check_date("01/01/2070 12:34:56");
  };
  if(doit)
  {
    date_t date1("1970-01-01T00:00:00");
    date_t date2;
    xassert(!date_t::diff(date1,date2));
  };
  if(doit)
  {
    json js;
    date_t date1("1968-12-05T04:05:06");
    js["a.base"]=date1;
    //cout << js.at("a.base").type() << endl;
    date1 += 1;
    js["b.+1s."]=date1;
    date1 -= 2;
    js["c.-1s."]=date1;
    js["dcopy."]=date1;
    ostringstream text;
    text << setw(4) << js;
    string str=text.str();
    js=json::parse(str);
    for( auto j : js )
    {
      //cout << "j:" << left << setw(10) << j.type() << setw(4) << j << endl;
    };
  };
  
  if(true || doit)
  {
    using namespace value;
    string str;
    {
      map<string,obj_t> objs;
      objs["rich"].name="Rich";
      objs["rich"].dob=date_t("1968-12-05T04:05:06");
      objs["rich"].girls=17;

      objs["ian"].name="Ian";
      objs["ian"].dob=date_t("1972-11-04T04:05:06");
      objs["ian"].girls=7;

      objs["capt"].name="CaptKickass";
      objs["capt"].dob=date_t("1966-11-04T04:05:06");
      objs["capt"].girls=20;

      json list;
      list=objs;
      str=list.dump(5);
    }
  };
  return 0;
};
