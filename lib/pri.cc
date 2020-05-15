#if 0
#include <bittrex/pri.hh>
#include <db.hh>

bittrex::pri_t::~pri_t(){};
//   bittrex::pri_t::pri_t( const val_t& num, const sym_t sym )
//     : num(num), den(den)
//   {
//     xcheckin();
//     xassert(this->cur()!=this->sym());
//   };
//   bittrex::pri_t::data_t::data_t( const val_t& num, const val_t& den )
//     : num(num), den(den)
//   {
//   };
bittrex::pri_t::pri_t( const val_t& num, const val_t& den )
  : num(num), den(den)
{
  xcheckin();
  xexpose(this->cur());
  xexpose(this->sym());
  xassert(this->cur()!=this->sym());
};

bittrex::pri_t bittrex::pri_t::avg(pri_t lhs, pri_t rhs)
{
  if ( lhs.cur()==rhs.sym() && lhs.sym()==rhs.cur())
    lhs=lhs.invert();

  if(lhs.cur()!=rhs.cur() || lhs.sym()!=rhs.sym())
    xthrowre("to average prices, they must have the same cur and sym");

  return pri_t(lhs.sym(),lhs.cur(),(lhs.val()+rhs.val())/2);
};
bittrex::pri_t bittrex::pri_t::invert() const
{
  return pri_t(val2,val1);
};
int bittrex::pri_t::cmp( const pri_t& lhs, const pri_t& rhs )
{
  auto lval = lhs.val();
  auto rval = rhs.val();
  return util::cmp( lhs.val(), rhs.val() );
};
ostream& bittrex::pri_t::stream( ostream& lhs ) const
{
  lhs << "{" << val() << cur() << " => 1" << sym() << "}";
  return lhs;
};
#endif
