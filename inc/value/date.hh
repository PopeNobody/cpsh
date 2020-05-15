#ifndef date_hh
#define date_hh date_hh

#include <fmt.hh>
#include <iostream>
#include <util.hh>
namespace value
{
  using std::ostream;
  using std::istream;
  class date_t : public fmt::streamable_tag
  {
    class data_t {
      time_t val;
      string src;

      public:
      data_t(time_t val);
      time_t get() const;
      void set(time_t val);
    } data;
    public:
    static time_t time_from_string(const string &str);
    explicit date_t(time_t val=0);
    explicit date_t(const string &val);
    date_t(const date_t &rhs);
    date_t &operator=(const date_t &rhs);
    void utime(time_t time);
    time_t utime() const
    {
      return data.get();
    };
    static time_t loc_to_gmt();
    explicit operator string() const;
    ostream &stream(ostream &lhs) const;
    static time_t diff( const time_t &date1, const time_t &date2 )
    {
      return date2-date1;
    };
    static time_t diff( const date_t &date1, const date_t &date2 )
    {
      return diff(date1.data.get(),date2.data.get());
    };
    date_t &operator-=(time_t secs)
    {
      utime(data.get()-secs);
      return *this;
    };
    date_t &operator+=(time_t secs)
    {
      utime(data.get()+secs);
      return *this;
    };
    string strftime(const string &fmt) const;
    static time_t cmp(const date_t &date1, const date_t &date2)
    {
      return util::cmp(date1.utime(),date2.utime());
    };
  };
  inline time_t operator-(const date_t &lhs, date_t rhs)
  {
    return lhs.utime()-rhs.utime();
  };
  inline date_t operator+(const date_t &lhs, time_t rhs)
  {
    date_t res(lhs);
    res+=rhs;
    return res;
  };
  inline date_t operator-(const date_t &lhs, time_t rhs)
  {
    date_t res(lhs);
    res-=rhs;
    return res;
  };
  inline date_t operator+(time_t lhs, const date_t &rhs)
  {
    date_t res(rhs);
    res+=lhs;
    return res;
  };
  istream &operator>>(istream &lhs, date_t &date);
};

#endif
