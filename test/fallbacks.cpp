// Boost.Convert test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"
#include <boost/function.hpp>

namespace { namespace local
{
    bool    called_functor_int;
    bool called_functor_double;
    bool   called_function_int;
    bool  called_function_long;
}}

struct    functor_int { int    operator()() const { local::   called_functor_int = true; return INT_MAX; }};
struct functor_double { double operator()() const { local::called_functor_double = true; return INT_MAX; }};

int   function_int () { local:: called_function_int = true; return INT_MAX; }
long function_long () { local::called_function_long = true; return INT_MAX; }

static
void
fallback_function()
{
    boost::cnv::cstringstream cnv;

    int i01 = boost::convert<int>("uhm", cnv).value_or(functor_int());
    int i02 = boost::convert<int>("uhm", cnv).value_or(functor_double());
    int i03 = boost::convert<int>("uhm", cnv).value_or(function_int);
    int i04 = boost::convert<int>("uhm", cnv).value_or(function_long);

    BOOST_TEST(local::   called_functor_int && i01 == INT_MAX);
    BOOST_TEST(local::called_functor_double && i02 == INT_MAX);
    BOOST_TEST(local::  called_function_int && i03 == INT_MAX);
    BOOST_TEST(local:: called_function_long && i04 == INT_MAX);

    local::   called_functor_int = false;
    local::called_functor_double = false;
    local::  called_function_int = false;
    local:: called_function_long = false;

    int i11 = boost::convert<int>("uhm", cnv, functor_int());
    int i12 = boost::convert<int>("uhm", cnv, functor_double());
    int i13 = boost::convert<int>("uhm", cnv, function_int);
    int i14 = boost::convert<int>("uhm", cnv, function_long);

    BOOST_TEST(local::   called_functor_int && i11 == INT_MAX);
    BOOST_TEST(local::called_functor_double && i12 == INT_MAX);
    BOOST_TEST(local::  called_function_int && i13 == INT_MAX);
    BOOST_TEST(local:: called_function_long && i14 == INT_MAX);

    try
    {
        int i14 = boost::convert<int>("uhm", cnv, boost::throw_on_failure);
        BOOST_TEST(0);
    }
    catch (boost::bad_optional_access const&)
    {
    }
}

void
test::cnv::fallbacks()
{
    fallback_function();
}
