#ifndef ops_hh
#define ops_hh

#include <util.hh>
#include <dbg.hh>

#if 0
namespace bittrex {
  template<typename val_t> bool operator == (const val_t &lhs, const val_t &rhs) {
    xcheckin();
    return util::cmp(lhs,rhs) == 0; 
  }
  template<typename val_t> bool operator != (const val_t &lhs, const val_t &rhs) {
    xcheckin();
    return util::cmp(lhs,rhs) != 0; 
  }
  template<typename val_t> bool operator <= (const val_t &lhs, const val_t &rhs) {
    xcheckin();
    return util::cmp(lhs,rhs) <= 0; 
  }
  template<typename val_t> bool operator >= (const val_t &lhs, const val_t &rhs) {
    xcheckin();
    return util::cmp(lhs,rhs) >= 0; 
  }
  template<typename val_t> bool operator < (const val_t &lhs, const val_t &rhs) {
    xcheckin();
    return util::cmp(lhs,rhs) < 0; 
  }
  template<typename val_t> bool operator > (const val_t &lhs, const val_t &rhs) {
    xcheckin();
    return util::cmp(lhs,rhs) > 0; 
  }
};
#endif

#endif
