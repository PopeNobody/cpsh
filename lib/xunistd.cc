#include "util.hh"
#include "stdarg.h"
#include "fcntl.h"
#include <vector>
#include <sys/mman.h>

using util::fd_t;
void *util::xmmap(void *addr, size_t len, int prot, int flags, fd_t fd, off_t off)
{
  void *res=mmap(addr,len,prot,flags,fd,off);
  if(res==(void*)-1)
    pexit("mmap");
  return res;
};
int util::xmunmap(void *addr, size_t length){
  int res=munmap(addr,length);
  if(res<0)
    pexit("munmap");
  return res;
};
off_t util::xlseek(fd_t fd, off_t offset, int whence)
{
  off_t res=lseek(fd,offset,whence);
  if(res<0)
    pexit("lseek");
  return res;
};

fd_t util::xopen(const char *pathname, int flags, mode_t mode)
{
  fd_t res=open(pathname,flags,mode);
  if(res<0)
    pexit("open",pathname);
  return res;
};
pid_t util::xfork()
{
  pid_t res = fork();
  if ( res < 0 )
    pexit( "fork" );
  return res;
};
size_t util::xread( int fd, char *buf, size_t len )
{
  ssize_t res=read(fd,buf,len);
  if(res<0)
    pexit( "read" );
  return res;
};
size_t util::xwrite( int fd, const char *buf, size_t len )
{
  ssize_t res=write(fd,buf,len);
  if(res<0)
    pexit("write");
  return res;
};
void util::xpipe( int ( &fds )[ 2 ] )
{
  if ( pipe( fds ) < 0 )
    pexit( "pipe" );
};
void util::xdup2( int ofd, int nfd )
{
  if ( dup2( ofd, nfd ) < 0 )
    pexit( "dup2" );
};
void util::xclose( int fd )
{
  if ( close( fd ) < 0 )
    pexit( "close" );
};
sighandler_t util::xsignal( int sig, sighandler_t hand )
{
  hand = signal( sig, hand );
  if ( hand == SIG_ERR )
    pexit( "signal" );
  return hand;
};
void util::xexeclp( const char* file, const char* arg, ... )
{
  vector< char* > argv;
  argv.push_back( (char*)arg );
  va_list vl;
  va_start( vl, arg );
  while ( char* next = va_arg( vl, char* ) )
    argv.push_back( next );
  argv.push_back( nullptr );
  va_end( vl );
  execvp( file, argv.data() );
  pexit( "exec", file );
};
