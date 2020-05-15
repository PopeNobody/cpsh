#define NDEBUG
#include <assert.h>
#define __assert_fail Holy shit !This will Never Parse !Were Doomed!
#ifndef debug_hh
#define debug_hh debug_hh

#include <iostream>
#include <util.hh>

#define nop()
#define xdo( x ) do { x; } while ( 0 )

#define xdbg() << __FILE__ << ":" << __LINE__ << ":" << std::boolalpha
#define xtrace( msg )  xdo( std::cerr xdbg() << msg << std::endl )
#define xexpose( msg ) xtrace( #msg << " => " << msg )
#define xcheckin()     xtrace(__PRETTY_FUNCTION__)
#define xcarp( msg )   xtrace( "warning: " << msg )
#define xthrow( type, msg ) xdo({ ostringstream s; s xdbg() << msg << std::endl; throw type( s.str() );} );
#define xthrowre( msg ) xthrow(runtime_error, msg )
#define xassert( cond ) if(!(cond)) {xthrowre( "assertion failed: " << #cond )}
#define xcroak( msg )  xthrowre( "error: " << msg )

#endif // debug_hh
