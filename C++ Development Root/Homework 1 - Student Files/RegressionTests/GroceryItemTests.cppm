module;                                                                               // Global fragment (not part of the module)
  #include <version>




/***********************************************************************************************************************************
**  Module Regression.GroceryItemTests
**
***********************************************************************************************************************************/
export module Regression.GroceryItemTests;
import std;
import GroceryItem;
import Regression.CheckResults;

export namespace Regression
{
  class GroceryItemTests
  {
    public:
      GroceryItemTests();

    private:
      void construction();
      void io();
      void comparison();
      void copyVsMoveSemantics();

      Regression::CheckResults affirm;
  };
} // export namespace Regression















/***********************************************************************************************************************************
**  Module Regression.GroceryItemTests Implementation
**  Private implementation: Not exported and not reachable
**
***********************************************************************************************************************************/
#include "private_fragement_patch.inc"
// module : private;




namespace Regression
{
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Namespace scoped object gets constructed BEFORE main(...) begins to execute.  It's this construction process that launches all
  // the regress tests.
  //
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  static GroceryItemTests run_grocery_item_tests;




  constexpr auto EPSILON = 1E-4;
  constexpr auto THETA   = 1E-8;

  void GroceryItemTests::construction()
  {
    GroceryItem gItem1,
                gItem2( "grocery item's product name"                                                                  ),
                gItem3( "grocery item's product name",  "grocery item's brand name"                                    ),
                gItem4( "grocery item's product name",  "grocery item's brand name", "grocery item's UPC code"         ),
                gItem5( "grocery item's product name",  "grocery item's brand name", "grocery item's UPC code", 123.79 );

    affirm.is_equal( "Default price", gItem1.price(), 0.0 );

    gItem1.productName( "1-3-5" );
    affirm.is_equal( "Symmetrical Product Name mutator/query", "1-3-5",    gItem1.productName() );

    gItem1.brandName( "21-23-25" );
    affirm.is_equal( "Symmetrical Brand Name mutator/query",   "21-23-25", gItem1.brandName() );

    gItem1.upcCode( "11-13-15" );
    affirm.is_equal( "Symmetrical UPC code mutator/query",     "11-13-15", gItem1.upcCode() );

    gItem1.price( 31.11 );
    affirm.is_equal( "Symmetrical Price mutator/query",        31.11,      gItem1.price() );

    affirm.is_true
    ( "Grocery item construction with arguments",
           gItem2.productName() == "grocery item's product name"
        && gItem3.productName() == "grocery item's product name" && gItem3.brandName() == "grocery item's brand name"
        && gItem4.productName() == "grocery item's product name" && gItem4.brandName() == "grocery item's brand name" && gItem4.upcCode() == "grocery item's UPC code"
        && gItem5.productName() == "grocery item's product name" && gItem5.brandName() == "grocery item's brand name" && gItem5.upcCode() == "grocery item's UPC code" && std::abs(gItem5.price() - 123.79) < EPSILON
      );

    GroceryItem gItem6( gItem5 );
    affirm.is_true
    ("Copy construction",
           gItem6.productName() ==  gItem5.productName()
        && gItem6.brandName()   ==  gItem5.brandName()
        && gItem6.upcCode()     ==  gItem5.upcCode()
        && std::abs(gItem6.price() - gItem5.price()) < EPSILON
    );

    GroceryItem gItem7( std::move(gItem6) );
    affirm.is_equal( "Move construction", gItem5, gItem7 );                                   // uses GroceryItem's operator == and operator <<, so if those doesn't work ...

    gItem6.productName( "" ).brandName( "" ).upcCode( "" ).price( 0 );                        // uses chaining, so if that doesn't work ...
    gItem6 = gItem5;
    affirm.is_true
    ("Copy assignment",
           gItem6.productName() ==  gItem5.productName()
        && gItem6.brandName()   ==  gItem5.brandName()
        && gItem6.upcCode()     ==  gItem5.upcCode()
        && std::abs(gItem6.price() - gItem5.price()) < EPSILON
    );


    gItem7 = std::move( gItem6 );
    affirm.is_equal( "Move assignment", gItem5, gItem7 );                                     // uses GroceryItem's operator == and operator <<, so if those doesn't work ...

  }



  void GroceryItemTests::io()
  {

    { // field and delimiter validation
      GroceryItem const control{ "abcdefg", "1234567", "uvwxyz", 77.5 };
      {
        std::istringstream stream( R"~~("upc", "brand", "product", -10.5  )~~" );
        stream >> std::noskipws;
        GroceryItem        variant;

        stream >> variant;

        affirm.is_equal( "Field validation-1: UPC",              "upc",     variant.upcCode() );
        affirm.is_equal( "Field validation-1: Brand Name",       "brand",   variant.brandName() );
        affirm.is_equal( "Field validation-1: Product Name",     "product", variant.productName() );
        affirm.is_equal( "Field validation-1: Price",            -10.5,     variant.price() );
        affirm.is_true ( "Delimiter validation-1: UPC/Brand",    !stream.fail()  );
        affirm.is_true ( "Delimiter validation-1: Brand/Product",!stream.eof ()  );
        affirm.is_true ( "Delimiter validation-1: Product/Price",!stream.bad ()  );
      }
      {
        std::istringstream stream( R"~~("upc"; "brand", "product", -10.5  )~~" );
        stream >> std::noskipws;
        GroceryItem variant{ control };

        stream >> variant;

        affirm.is_equal( "Field validation-2: Unchanged Grocery Item", control, variant );
        affirm.is_true ( "Delimiter validation-2: UPC/Brand",          stream.fail()  );
        affirm.is_true ( "Delimiter validation-2: Brand/Product",      !stream.eof ()  );
        affirm.is_true ( "Delimiter validation-2: Product/Price",      !stream.bad ()  );
      }
      {
        std::istringstream stream( R"~~("upc", "brand"; "product", -10.5  )~~" );
        stream >> std::noskipws;
        GroceryItem variant{ control };

        stream >> variant;

        affirm.is_equal( "Field validation-3: Unchanged Grocery Item", control, variant );
        affirm.is_true ( "Delimiter validation-3: UPC/Brand",          stream.fail()  );
        affirm.is_true ( "Delimiter validation-3: Brand/Product",      !stream.eof ()  );
        affirm.is_true ( "Delimiter validation-3: Product/Price",      !stream.bad ()  );
      }
      {
        std::istringstream stream( R"~~("upc", "brand", "product"; -10.5  )~~" );
        stream >> std::noskipws;
        GroceryItem variant{ control };

        stream >> variant;

        affirm.is_equal( "Field validation-4: Unchanged Grocery Item", control, variant );
        affirm.is_true ( "Delimiter validation-4: UPC/Brand",          stream.fail()  );
        affirm.is_true ( "Delimiter validation-4: Brand/Product",      !stream.eof ()  );
        affirm.is_true ( "Delimiter validation-4: Product/Price",      !stream.bad ()  );
      }
    }
    { // Input parsing

      std::istringstream stream( R"~~( "00072250018548","Nature's Own","Nature's Own Butter Buns Hotdog - 8 Ct",56.69

                                        "00028000517205", "Nestle"             ,
                                        "Nestle \"Media Crema\" Table Cream"       ,
                                        118.07

                                        "00034000020706"    ,
                                        "York",
                                        "York Peppermint Patties Dark Chocolate Covered Snack Size",
                                        31.57 "00038000570742",
                                        "Kellogg's", "Kellogg's Cereal Krave Chocolate",
                                          65.65

                                        "00014100072331" , "Pepperidge  \"Home Town\"  Farm", "Pepperidge Farm Classic Cookie Favorites", 26.45

                                        "00000000000000", "invalid item" "invalid 7.99

                                        "00000000000000", "incomplete / invalid grocery item"
                                  )~~" );

      stream >> std::noskipws;

      const GroceryItem control{ "abcdefg", "1234567", "uvwxyz", 77.77 };
      GroceryItem        t1, t2, t3, t4, t5, t6(control), t7(control);

      stream >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7;

      affirm.is_equal( "GroceryItem input parsing 1", GroceryItem { "Nature's Own Butter Buns Hotdog - 8 Ct",                     "Nature's Own",                     "00072250018548", 56.69  }, t1 );
      affirm.is_equal( "GroceryItem input parsing 2", GroceryItem { "Nestle \"Media Crema\" Table Cream",                         "Nestle",                           "00028000517205", 118.07 }, t2 );
      affirm.is_equal( "GroceryItem input parsing 3", GroceryItem { "York Peppermint Patties Dark Chocolate Covered Snack Size",  "York",                             "00034000020706", 31.57  }, t3 );
      affirm.is_equal( "GroceryItem input parsing 4", GroceryItem { "Kellogg's Cereal Krave Chocolate",                           "Kellogg's",                        "00038000570742", 65.65  }, t4 );
      affirm.is_equal( "GroceryItem input parsing 5", GroceryItem { "Pepperidge Farm Classic Cookie Favorites",                   "Pepperidge  \"Home Town\"  Farm",  "00014100072331", 26.45  }, t5 );

      affirm.is_equal( "GroceryItem input parsing 6", control, t6 );
      affirm.is_equal( "GroceryItem input parsing 7", control, t7 );
    }
    {  // read what you write (insertion operator)
      GroceryItem gItem1,
                  gItem2( "grocery item's product name" ),
                  gItem3( "grocery item's product name", "grocery item's brand name" ),
                  gItem4( "grocery item's product name", "grocery item's brand name", "grocery item's UPC code" ),
                  gItem5( "grocery item's product name", "grocery item's brand name", "grocery item's UPC code", 123.79 );

      std::stringstream stream;
      stream >> std::noskipws;

      stream << gItem1 << '\n'
             << gItem2 << '\n'
             << gItem3 << '\n'
             << gItem4 << '\n'
             << gItem5;

      GroceryItem t1, t2, t3, t4, t5;
      stream >> t1 >> t2 >> t3 >> t4 >> t5;

      affirm.is_equal( "Symmetrical GroceryItem insertion and extraction 1", gItem1, t1 );
      affirm.is_equal( "Symmetrical GroceryItem insertion and extraction 2", gItem2, t2 );
      affirm.is_equal( "Symmetrical GroceryItem insertion and extraction 3", gItem3, t3 );
      affirm.is_equal( "Symmetrical GroceryItem insertion and extraction 4", gItem4, t4 );
      affirm.is_equal( "Symmetrical GroceryItem insertion and extraction 5", gItem5, t5 );
    }
    {  // read what you write (formatter)
      GroceryItem gItem1,
                  gItem2( "grocery item's product name" ),
                  gItem3( "grocery item's product name", "grocery item's brand name" ),
                  gItem4( "grocery item's product name", "grocery item's brand name", "grocery item's UPC code" ),
                  gItem5( "grocery item's product name", "grocery item's brand name", "grocery item's UPC code", 123.79 );

      std::stringstream stream;
      stream >> std::noskipws;

      std::print( stream, "{}\t{}\v{} {}{}",
                          gItem1,
                          gItem2,
                          gItem3,
                          gItem4,
                          gItem5 );

      GroceryItem t1, t2, t3, t4, t5;
      stream >> t1 >> t2 >> t3 >> t4 >> t5;

      affirm.is_equal( "Symmetrical GroceryItem insertion and extraction 6",  gItem1, t1 );
      affirm.is_equal( "Symmetrical GroceryItem insertion and extraction 7",  gItem2, t2 );
      affirm.is_equal( "Symmetrical GroceryItem insertion and extraction 8",  gItem3, t3 );
      affirm.is_equal( "Symmetrical GroceryItem insertion and extraction 9",  gItem4, t4 );
      affirm.is_equal( "Symmetrical GroceryItem insertion and extraction 10", gItem5, t5 );
    }
  }



  void GroceryItemTests::comparison()
  {
    GroceryItem less( "a1", "a1", "a1", 10.0 ), more(less);

    // Be careful - using affirm.xxx() may hide the class-under-test overloaded operators.  But affirm.is_true() doesn't provide as
    // much information when the test fails.
    affirm.is_equal    ( "Equality test - is equal",                   less, more );
    affirm.is_equal    ( "Equality test - floating point lower limit", less, GroceryItem {"a1", "a1", "a1", less.price() - EPSILON} );
    affirm.is_equal    ( "Equality test - floating point upper limit", less, GroceryItem {"a1", "a1", "a1", less.price() + EPSILON} );

    affirm.is_not_equal( "Inequality Product Name test",               less, GroceryItem {"b1", "a1", "a1", 10.0} );
    affirm.is_not_equal( "Inequality Brand Name test",                 less, GroceryItem {"a1", "b1", "a1", 10.0} );
    affirm.is_not_equal( "Inequality UPC test",                        less, GroceryItem {"a1", "a1", "b1", 10.0} );
    affirm.is_not_equal( "Inequality Price test - lower limit",        less, GroceryItem {"a1", "a1", "a1", less.price() - ( 2 * EPSILON )} );
    affirm.is_not_equal( "Inequality Price test - upper limit",        less, GroceryItem {"a1", "a1", "a1", less.price() + ( 2 * EPSILON )} );


    auto check = [&]()
    {
      // Exercise all relational operators for the class-under-test.
      return less <  more
          && less <= more
          && more >  less
          && more >= less;
    };

    more = {"a1", "a1", "a1", 11.0};
    affirm.is_true( "Relational Price test",        check() );

    more = {"a1", "a2", "a1", 9.0};
    affirm.is_true( "Relational Brand Name test",   check() );

    more = {"a2", "a0", "a1", 9.0};
    affirm.is_true( "Relational Product Name test", check() );

    more = {"a0", "a0", "a2", 9.0};
    affirm.is_true( "Relational UPC code test",     check() );



    // test equality for large floating point numbers
    more = less = { "a1", "a1", "a1", 40000.0 };
    affirm.is_equal    ( "Equality test - large floating point is equal",     less, more );
    affirm.is_not_equal( "Equality test - large floating point is not equal", less, more.price( 40000.000400000001 ) );

    affirm.is_equal    ( "Equality test - large floating point lower limit",  less, more.price( less.price() - ( EPSILON + THETA ) * 2 ) );
    affirm.is_equal    ( "Equality test - large floating point lower limit",  less, more.price( less.price() + ( EPSILON + THETA ) * 2 ) );
  }



  void GroceryItemTests::copyVsMoveSemantics()
  {
    GroceryItem const gItem5( "grocery item's product name",  "grocery item's brand name", "grocery item's UPC code", 123.79 );


    { // UPC Code
      GroceryItem gItem( gItem5 );
      gItem.upcCode();                            // keep an eye on this, make sure it doesn't get optimized away
      affirm.is_equal( "Copy VS Move Semantics - UPC Code Copy", gItem5, gItem );

      std::move( gItem ).upcCode();               // keep an eye on this, make sure it doesn't get optimized away
      affirm.is_not_equal( "Copy VS Move Semantics - UPC Code Move", gItem5, gItem );
    }


    { // Brand Name
      GroceryItem gItem( gItem5 );
      gItem.brandName();                          // keep an eye on this, make sure it doesn't get optimized away
      affirm.is_equal( "Copy VS Move Semantics - Brand Name Copy", gItem5, gItem );

      std::move( gItem ).brandName();             // keep an eye on this, make sure it doesn't get optimized away
      affirm.is_not_equal( "Copy VS Move Semantics - Brand Name Move", gItem5, gItem );
    }


    { // Product Name
      GroceryItem gItem( gItem5 );
      gItem.productName();                        // keep an eye on this, make sure it doesn't get optimized away
      affirm.is_equal( "Copy VS Move Semantics - Product Name Copy", gItem5, gItem );

      std::move( gItem ).productName();           // keep an eye on this, make sure it doesn't get optimized away
      affirm.is_not_equal( "Copy VS Move Semantics - Product Name Move", gItem5, gItem );
    }
  }



  GroceryItemTests::GroceryItemTests()
  {
    // affirm.policy = Regression::CheckResults::ReportingPolicy::ALL;
    // affirm.name_width = 40;

    try
    {
      std::clog << "\n\n\nGroceryItem Regression Test:  Construction\n";
      construction();

      std::clog << "\nGroceryItem Regression Test:  Relational comparisons\n";
      comparison();

      std::clog << "\nGroceryItem Regression Test:  Input/Output\n";
      io ();


      std::clog << "\nGroceryItem Regression Test:  Move Semantics\n";
      copyVsMoveSemantics();


      std::clog << "\n\nGroceryItem Regression Test " << affirm << "\n\n";
    }
    catch( const std::exception & ex )
    {
      std::clog << "FAILURE:  Regression test for \"class GroceryItem\" failed with an unhandled exception. \n\n\n"
                << ex.what() << std::endl;
    }
  }
}  // Regression namespace















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
