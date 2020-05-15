#ifndef pri_hh
#define pri_hh pri_hh

#include <bittrex/ops.hh>
#include <fmt.hh>
#include <bittrex/val.hh>

#if 0
namespace bittrex
{
  class pri_t : public fmt::streamable_tag
  {
    val_t num;
    val_t den;
    public: 
    pri_t()
    {
    };
    static pri_t avg(pri_t lhs, pri_t rhs);
    pri_t invert() const;
    const sym_t &sym() const
    {
      return num.sym();
    };
    const sym_t &cur() const
    {
      return den.sym();
    };
    double val() const
    {
      return num.val()/den.val();
    };
    pri_t( const val_t &num, const val_t &den );
    ~pri_t();
    ostream &stream(ostream &lhs) const;
    static int cmp(const pri_t &lhs, const pri_t &rhs);
    pri_t &operator+=(const pri_t &rhs);
  };
  inline pri_t operator+(const pri_t &lhs, const pri_t &rhs)
  {
    pri_t tmp(lhs);
    tmp+=rhs;
    return tmp;
  };
  inline pri_t operator/(const val_t &lhs, const val_t &rhs)
  {
    return pri_t(lhs,rhs);
  };
  inline bool operator== (const pri_t &lhs, const pri_t &rhs)
  {
    return pri_t::cmp(lhs,rhs)==0;
  };
  inline bool operator!= (const pri_t &lhs, const pri_t &rhs)
  {
    return pri_t::cmp(lhs,rhs)!=0;
  };
  inline bool operator<= (const pri_t &lhs, const pri_t &rhs)
  {
    return pri_t::cmp(lhs,rhs)<=0;
  };
  inline bool operator>= (const pri_t &lhs, const pri_t &rhs)
  {
    return pri_t::cmp(lhs,rhs)>=0;
  };
  inline bool operator< (const pri_t &lhs, const pri_t &rhs)
  {
    return pri_t::cmp(lhs,rhs)<0;
  };
  inline bool operator>  (const pri_t &lhs, const pri_t &rhs)
  {
    return pri_t::cmp(lhs,rhs)>0;
  };
}
#endif

#endif
