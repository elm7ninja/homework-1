module;                                                                         // Global fragment (not part of the module)
  // Empty




/***********************************************************************************************************************************
**  Module GroceryItem Interface
**
***********************************************************************************************************************************/
export module GroceryItem;                                                      // Primary Module Interface Definition
import std;

export
{
  class GroceryItem
  {
    public:
      // Constructors, assignments, and destructor
      GroceryItem( std::string productName = {},                                // Default and Conversion (from string to GroceryItem) constructor
                   std::string brandName   = {},                                // String parameters intentionally passed by value.  Not perfect, but very very
                   std::string upcCode     = {},                                // good when combined with move semantics.  See https://youtu.be/PNRju6_yn3o
                   double      price       = 0.0 );

      GroceryItem & operator=( GroceryItem const  & rhs   ) &;                  // Assignment operators available only for l-values (that's what the trailing "&" means), and then
      GroceryItem & operator=( GroceryItem       && rhs   ) & noexcept;         // the 'Rule of 5' says if you define one, then you should define them all
      GroceryItem            ( GroceryItem const  & other );                    // OK:  GroceryItem a{"title"},b;  b = a;  (a and b are both l-values, i.e. named objects)
      GroceryItem            ( GroceryItem       && other )   noexcept;         // Error:  GroceryItem{} = a;              (GroceryItem{} is an r-value, i.e., an unnamed temporary object)
     ~GroceryItem            (                            )   noexcept;


      // Accessors
      std::string const & upcCode    () const &;                                // Returns object's state by constant reference for l-value objects and by value for r-value objects
      std::string const & brandName  () const &;                                // The "const &" at the end says these functions will be called for l-value objects and r-value objects
      std::string const & productName() const &;                                // that (listen carefully) haven't been overloaded.
      double              price      () const &;                                //
                                                                                //
      std::string         upcCode    ()       &&;                               // Overloads that return an r-value object's state by value (unsafe to return an r-value's state by reference)
      std::string         brandName  ()       &&;                               // The "&&" at the end says these functions will be called only for r-value objects
      std::string         productName()       &&;                               // Search "lvalue vs rvalue", or see https://www.learncpp.com/cpp-tutorial/value-categories-lvalues-and-rvalues/,
                                                                                // https://www.bing.com/videos/search?q=chono+c%2b%2b+lvalue+vs+rvalue&docid=608038928535204227&mid=6E0B93922619A11969BB6E0B93922619A11969BB&view=detail&FORM=VIRE

      // Modifiers                                                              // Updates object's state and returns a reference to self (enables chaining)
      GroceryItem & upcCode    ( std::string newUpcCode     ) &;                // String parameters intentionally passed by value
      GroceryItem & brandName  ( std::string newBrandName   ) &;                // Modifiers available for l-values only         (The & at the end says these functions will be called only for l-values)
      GroceryItem & productName( std::string newProductName ) &;                // OK:     GroceryItem b; b.price(13.99);        (b is an l-value, i.e. a named object)
      GroceryItem & price      ( double      newPrice       ) &;                // Error:  GroceryItem{}.price(13.99);           (The default constructed GrocerItem is an r-value, i.e., an unnamed temporary object)


      // Relational Operators
      std::weak_ordering operator<=>( GroceryItem const & rhs ) const noexcept;
      bool               operator== ( GroceryItem const & rhs ) const noexcept;

    private:
      std::string _upcCode;                                                     // a 12 or 14-digit international Universal Product Code uniquely identifying this item (Ex: 051600080015, 05017402006207)
      std::string _brandName;                                                   // the product manufacturer's brand name (Ex: Heinz, Boston Market)
      std::string _productName;                                                 // the name of the product (Ex: Heinz Tomato Ketchup - 2 Ct, Boston Market Spaghetti With Meatballs)
      double      _price{ 0.0 };                                                // the cost of the item in US Dollars (Ex:  2.29, 1.19)
  };  // class GroceryItem


  std::ostream & operator<<( std::ostream & stream, GroceryItem const & groceryItem );  // Inserts the grocery item into the stream
  std::istream & operator>>( std::istream & stream, GroceryItem       & groceryItem );  // Extract the grocery item from the stream
}    // export







// Not exported but reachable
/***********************************************************************************************************************************
** Template definitions
***********************************************************************************************************************************/

// Specialize the formatter to accept and process CSUF::CPSC131::GroceryItem typed objects
template<>
struct std::formatter<GroceryItem> : std::formatter<std::string>
{
  // Convert the grocery item object to a string object with the appropriate formatting.  Then format this string
  // object using std::string's format descriptors.  This added flexibility allows the client to specify a field with, left,
  // right, or center justified, etc.
  //
  // Be sure
  // o)  UPC, brand name, and product name are quoted with appropriate embedded quote escaping;
  // o)  price is displayed with
  //     - with a decimal point
  //     - exactly two digits to the right of the decimal point
  //     - inserts the appropriate digit grouping separator
  //     - no monetary symbol (e.g., $)  Someday we'll override the parse function to provide that as a formatting option
  //
  // See https://github.com/TBettens/Data_Structure_Implementation_Examples/blob/latest/Common/Student.cppm
  //     https://en.cppreference.com/w/cpp/utility/format/spec.html
  auto format( const GroceryItem & item, auto & ctx ) const
  {
    ///////////////////////// TO-DO (1) //////////////////////////////

    /////////////////////// END-TO-DO (1) ////////////////////////////
  }
};















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
