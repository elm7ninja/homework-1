// GCC 15.1.1 workaround for unsupported module : private
// Not a standalone header file
//
// Usage:  Replace "module : private" with #include "private_fragment_patch.inc"
//         <version> must be #included before including this file
#pragma once


#if ! defined( __GLIBCXX__ )                                             // If GNU's libstdc++ library (GCC or Clang)
  module : private;
#else
  #if ( ( __GLIBC__ << 16 ) + __GLIBC_MINOR__ <= ( 2 << 16 ) + 41 )      // ldd 2.39 and gcc 15.1.1  (run "ldd --version" and "gcc --version" at the command line)
    // module : private;    GCC doesn't support private fragments.
    //                      The workaround is to just don't create a private fragment
    //                      The better approach is to create a separate module implementation unit (.cpp) file, but I didn't want the overhead
    // Note:  For GCC all the stuff below is reachable and not encapsulated as a :private fragment
  #else
    #pragma message( "A potentially obsolete C++23 workaround is present.  Either remove the workaround if no longer needed, or update the version number requiring it" )
  #endif
#endif
