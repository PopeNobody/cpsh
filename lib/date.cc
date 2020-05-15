#include <value/date.hh>
#include <json.hh>
#include <iostream>
#include <dbg.hh>
#include <util.hh>

using namespace std;
using namespace util;

#undef xcheckin
#define xcheckin()

namespace value {
  date_t adate(1);
  date_t::data_t::data_t(time_t val)
    :val(val)
  {
    xcheckin();
  };
  time_t date_t::data_t::get() const {
    xcheckin();
    return  val;
  };
  void date_t::utime(time_t val)
  {
    xcheckin();
    data.set(val);
  };
  void date_t::data_t::set(time_t time)
  {
    xcheckin();
    val=time;
  };
  date_t &date_t::operator=(date_t const&rhs)
  {
    xcheckin();
    data.set(rhs.utime());
    return *this;
  };
  date_t::date_t(time_t val)
    : data(val)
  {
    xcheckin();
  };
  date_t::date_t(const string &str)
    : data(time_from_string(str))
  {
    xcheckin();
  };
  date_t::date_t(date_t const&rhs)
    :data(rhs.utime())
  {
    xcheckin();
  };
  ostream &date_t::stream(ostream &lhs) const
  {
    xcheckin();
    lhs << (string)(*this);
    return lhs;
  };
  istream &operator>>(istream &lhs, date_t &date)
  {
    xcheckin();
    //xcheckin();
    string str;
    lhs >> str;
    date=date_t(str);
    return lhs;
  };
  time_t date_t::loc_to_gmt()
  {
    time_t zero=86400;
    tm gm_time(*gmtime(&zero));
    tm lo_time(*localtime(&zero));
    return mktime(&gm_time)-mktime(&lo_time);
  };
  string date_t::strftime(const string &str) const
  {
    time_t val=data.get();
    tm btime = *gmtime(&val);
    char buf[1024];
    size_t res=::strftime( buf, sizeof(buf)-1, str.c_str(), &btime);
    return string(buf,buf+res);
  };
  date_t::operator string() const
  {
    return strftime("%Y-%m-%d %H:%M:%S %Z");
  };
  struct xtm : public tm
  {
    xtm()
    {
      memset(this,0,sizeof(*this));
    };
  };
  inline ostream &operator<<(ostream &lhs, const xtm &rhs);
  time_t finish_time(xtm &tm, const char *rest)
  {
    while(isspace(*rest))
      ++rest;
    if( 
        (rest[0]>='A' && rest[0]<='Z' )
        &&
        (rest[1]>='A' && rest[1]<='Z' )
        &&
        (rest[2]>='A' && rest[2]<='Z' )
      )
    {
      tm.tm_zone=rest;
    };
    time_t ret=mktime(&tm);
    return ret;
  }
  static vector<string> date_fmts = {
    "%m-%d-%Y",
    "%m/%d/%Y",
    "%d-%m-%Y",
    "%d/%m/%Y",
    "%Y-%m-%d",
    "%Y/%m/%d",
  };
  static vector<string> seps = {
    "T",
    " ",
    ""
  };
  static vector<string> time_fmts = {
    "%I:%M:%S %p",
    "%I:%M %p",
    "%H:%M:%S",
    "%H:%M",
    ""
  };
  inline ostream &operator<<(ostream &lhs, const xtm &rhs)
  {
    lhs << "{ ";
    lhs << rhs.tm_year+1900;
    lhs << "-";
    lhs << rhs.tm_mon+1;
    lhs << "-";
    lhs << rhs.tm_mday;
    lhs << " ";
    lhs << rhs.tm_hour;
    lhs << ":";
    lhs << rhs.tm_min;
    lhs << ":";
    lhs << rhs.tm_sec;
    lhs << " ";
    lhs << (rhs.tm_zone?rhs.tm_zone:"(null)");
    lhs << " }";
    return lhs;
  };
  time_t date_t::time_from_string(const string &str)
  {
    xcheckin();
    xtm a_time;
    const char *pos=str.c_str();
    const char *res=0;
    for( auto const &dfmt : date_fmts )
    {
      for( auto const &tfmt : time_fmts )
      {
        for( auto const &sep : seps )
        {
          string fmt=dfmt+sep+tfmt;
    //      cout << "fmt=" << fmt << endl;
          res=strptime( pos, fmt.c_str(), &a_time);
          if(res) {
            return finish_time(a_time, res);
          };
        };
      };
    };
    xthrowre( "failed to parse date: " << str );
  };
};
