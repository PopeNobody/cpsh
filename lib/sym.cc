#include <bittrex/sym.hh>
#include <string.h>
#include <dbg.hh>

std::array<char,8> bittrex::mk_array(const string &str)
{
  array<char,8> res;
  for(int i=0;i<res.size();i++)
  {
    if(i<str.size())
      res[i]=str[i];
    else
      res[i]=0;
  };
  return res;
};
bittrex::sym_t::data_t::data_t(const std::array<char,8> &rhs)
  : txt(rhs)
{
};
bittrex::sym_t::sym_t()
  :data(mk_array(""))
{
};
bittrex::sym_t::sym_t(const char *c_str)
  :data(mk_array(c_str))
{
};
bittrex::sym_t::sym_t(const string &str)
  :data(mk_array(str))
{
};
size_t bittrex::sym_t::size()const
{
  return strlen(&data.txt[0]);
};
size_t bittrex::sym_t::length()const
{
  return strlen(&data.txt[0]);
};
ostream &bittrex::sym_t::stream(ostream &lhs) const
{
  return lhs << (&data.txt[0]);
};
int bittrex::sym_t::cmp( const bittrex::sym_t& lhs, const bittrex::sym_t &rhs )
{
  return strcmp(&lhs.data.txt[0],&rhs.data.txt[0]);
};
