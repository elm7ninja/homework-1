module;                                                                         // Global fragment (not part of the module)
  // Empty




/***********************************************************************************************************************************
**  Module Regression.CheckResults Interface
**
***********************************************************************************************************************************/
export module Regression.CheckResults;                                     // Primary Module Interface Definition
import std;


export namespace Regression
{
  struct CheckResults
  {
    enum class ReportingPolicy{ FAILED_ONLY, BRIEF, ALL };
    CheckResults( std::ostream & stream = std::clog ) : testResults( stream )
    { testResults << std::unitbuf; } // flush the buffer after each insertion - sometimes having that little bit extra info helps if the program crashes

                                     bool is_true                    ( const std::string & nameOfTest,                     bool      actual );
    template<typename T, typename U> bool is_equal                   ( const std::string & nameOfTest, const T & expected, const U & actual );
    template<typename T, typename U> bool is_not_equal               ( const std::string & nameOfTest, const T & expected, const U & actual );
    template<typename T, typename U> bool is_less_than               ( const std::string & nameOfTest, const T & expected, const U & actual );
    template<typename T, typename U> bool is_less_than_or_equal_to   ( const std::string & nameOfTest, const T & expected, const U & actual );
    template<typename T, typename U> bool is_greater_than            ( const std::string & nameOfTest, const T & expected, const U & actual );
    template<typename T, typename U> bool is_greater_than_or_equal_to( const std::string & nameOfTest, const T & expected, const U & actual );

    template<typename T, typename U >
    constexpr bool equal( T const & lhs,  U const & rhs) noexcept
    {
      if constexpr( std::is_floating_point_v<std::common_type_t<T, U>> ) return std::abs( lhs - rhs ) < EPSILON;
      else                                                               return lhs == rhs;
    }

    unsigned        testCount   = 0;
    unsigned        testsPassed = 0;
    unsigned        name_width  = 55;
    double          EPSILON     = 1e-9;
    ReportingPolicy policy      = ReportingPolicy::BRIEF;
    std::ostream &  testResults;
  };  // export struct CheckResults

  std::ostream & operator<<( std::ostream & stream, const CheckResults & results );
}  // export namespace Regression















// Not exported but reachable
namespace Regression
{
  /***********************************************************************************************************************************
  ** Definitions
  ***********************************************************************************************************************************/
  inline std::ostream & operator<<( std::ostream & stream, const CheckResults & results )
  {
    return stream << std::format( "Summary: {} of {} ({:.5g}%) tests passed",
                                  results.testsPassed, results.testCount,
                                  results.testsPassed * 100.0 / results.testCount );
  }









  inline bool CheckResults::is_true( const std::string & nameOfTest, bool actual )
  {
    return is_equal( nameOfTest, true, actual );
  }









  template<typename T, typename U>
  inline bool CheckResults::is_equal( const std::string & nameOfTest, const T & expected, const U & actual )
  {
    ++testCount;

    if( ! equal(expected, actual) )
    {
      testResults << std::format( " *[FAILED] {:{}}: the expected value is not equal to the actual value, but should be\n"
                                  "    EXP: {{{}}}\n"
                                  "    ACT: {{{}}}\n",
                                  nameOfTest, name_width,
                                  expected, actual );
      return false;
    }

    ++testsPassed;
    if( policy >= ReportingPolicy::BRIEF )
    {
      testResults << std::format( "  [PASSED] {:{}}", nameOfTest, name_width );
      if( policy > ReportingPolicy::BRIEF ) testResults << std::format( ": as expected, the expected and actual values are equal:  {{{}}}", actual );
      testResults << '\n';
    }

    return true;
  }









  template<typename T, typename U>
  inline bool CheckResults::is_not_equal( const std::string & nameOfTest, const T & expected, const U & actual )
  {
    ++testCount;

    if( equal(expected, actual) )
    {
      testResults << std::format( " *[FAILED] {:{}}: the expected value is equal to the actual value, but shouldn't be:  {{{}}}\n",
                                  nameOfTest, name_width,
                                  actual );
      return false;
    }

    ++testsPassed;
    if( policy >= ReportingPolicy::BRIEF )
    {
      testResults << std::format( "  [PASSED] {:{}}", nameOfTest, name_width );
      if( policy > ReportingPolicy::BRIEF ) testResults << std::format( ": as expected, the expected and actual values are not equal\n"
                                                                        "    EXP: {{{}\n"
                                                                        "    ACT: {{{}}}",
                                                                        expected, actual );
      testResults << '\n';
    }

    return true;
  }









  template<typename T, typename U>
  inline bool CheckResults::is_less_than( const std::string & nameOfTest, const T & expected, const U & actual )
  {
    ++testCount;

    if( equal(expected, actual)  ||  !(expected < actual) )       // account for "close enough" floating point numbers before check for inequality
    {
      testResults << std::format( " *[FAILED] {:{}}: the expected value is not less than the actual value, but should be\n"
                                  "    EXP: {{{}}}\n"
                                  "    ACT: {{{}}}\n",
                                  nameOfTest, name_width,
                                  expected, actual );
      return false;
    }

    ++testsPassed;
    if( policy >= ReportingPolicy::BRIEF )
    {
      testResults << std::format( "  [PASSED] {:{}}", nameOfTest, name_width );
      if( policy > ReportingPolicy::BRIEF ) testResults << std::format( ": as expected, the expected value is less than the actual value\n"
                                                                        "    EXP: {{{}}}\n"
                                                                        "    ACT: {{{}}}",
                                                                        expected, actual );
      testResults << '\n';
    }

    return true;
  }









  template<typename T, typename U>
  inline bool CheckResults::is_greater_than( const std::string & nameOfTest, const T & expected, const U & actual )
  {
    ++testCount;

    if( equal(expected, actual)  ||  !(actual < expected) )       // account for "close enough" floating point numbers before check for inequality
    {
      testResults << std::format( " *[FAILED] {:{}}: the expected value is not greater than the actual value, but should be\n"
                                  "   EXP: {{{}}}\n"
                                  "   ACT: {{{}}}\n",
                                  nameOfTest, name_width,
                                  expected, actual );
      return false;
    }

    ++testsPassed;
    if( policy >= ReportingPolicy::BRIEF )
    {
      testResults << std::format( "  [PASSED] {:{}}", nameOfTest, name_width );
      if( policy > ReportingPolicy::BRIEF ) testResults << std::format( ": as expected, the expected value is greater than the actual value\n"
                                                                        "    EXP: {{{}}}\n"
                                                                        "    ACT: {{{}}}",
                                                                        expected, actual );
      testResults << '\n';
    }

    return true;
  }
}    // namespace Regression















/***********************************************************************************************************************************
** (C) Copyright 2025 by Thomas Bettens. All Rights Reserved.
**
** DISCLAIMER: The participating authors at California State University's Computer Science Department have used their best efforts
** in preparing this code. These efforts include the development, research, and testing of the theories and programs to determine
** their effectiveness. The authors make no warranty of any kind, expressed or implied, with regard to these programs or to the
** documentation contained within. The authors shall not be liable in any event for incidental or consequential damages in
** connection with, or arising out of, the furnishing, performance, or use of these libraries and programs.  Distribution without
** written consent from the authors is prohibited.
***********************************************************************************************************************************/

/**************************************************
** Last modified:  09-AUG-2025
** Last Verified:  09-AUG-2025
** Verified with:  MS Visual Studio 2022 Version 17.14.9,  Compiler Version 19.44.35213 (/std:c++latest)
**                 GCC version 15.1.0 (-std=c++23 )
**                 Clang version 21.0.0 (-std=c++23 -stdlib=libc++)
***************************************************/
