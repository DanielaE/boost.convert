// Boost.Convert
// Copyright (c) 2009-2014 Vladimir Batov.
//
// Many thanks to 
// *) Andrzej Krzemienski for helping great deal to partition responsibilities and to ultimately pave
//    the way for the std::tr2::optional deployment;
// *) Edward Diener the Boost Review Manager for helping with the converters' design, his continuous
//    involvement, technical and administrative help, guidance and advice;
// *) Kevlin Henney and Dave Abrahams for their ['lexical_cast]-related insights and explanations;
// *) Rob Stewart and Alex Hagen-Zanker for making sure the performance tests work as they should.
// 
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_HPP
#define BOOST_CONVERT_HPP

#include <boost/convert/detail/is.hpp>
#include <boost/convert/detail/adapter.hpp>

namespace boost
{
    struct throw_on_failure_type { enum type {};};
    throw_on_failure_type::type const throw_on_failure = throw_on_failure_type::type(0);

    /// @brief The main Boost.Convert deployment interface
    /// @details This is the Boost.Convert main interface. For example,
    /// @code
    ///    boost::cnv::cstringstream cnv;
    ///
    ///    int    i1 = boost::convert<int>(" 12", cnv(std::skipws)).value();
    ///    int    i2 = boost::convert<int>("uhm", cnv).value_or(-1);
    ///    string s1 = boost::convert<string>(123.456, cnv)).value();
    ///    string s2 = boost::convert<string>(123.456, cnv)).value_or("failed");
    /// @endcode

    template<typename TypeOut, typename TypeIn, typename Converter>
    typename boost::enable_if<cnv::is_converter<Converter, TypeIn, TypeOut>, boost::optional<TypeOut> >::type
    convert(TypeIn const& value_in, Converter const& converter)
    {
            boost::optional<TypeOut> result;
            converter(value_in, result);
            return result;
    }

    template<typename TypeOut, typename TypeIn, typename Converter>
    typename boost::enable_if<cnv::is_converter<Converter, TypeIn, TypeOut>, TypeOut>::type
    convert(TypeIn const& value_in, Converter const& converter, throw_on_failure_type::type)
    {
        return boost::convert<TypeOut>(value_in, converter).value();
    }

    template<typename TypeOut, typename TypeIn, typename Converter, typename FallbackValue>
    typename boost::enable_if_c<cnv::is_converter<Converter, TypeIn, TypeOut>::value &&
                                boost::is_convertible<FallbackValue, TypeOut>::value,
    TypeOut>::type
    convert(TypeIn const& value_in, Converter const& converter, FallbackValue const& fallback)
    {
        return boost::convert<TypeOut>(value_in, converter).value_or(fallback);
    }

    template<typename TypeOut, typename TypeIn, typename Converter, typename FallbackFunc>
    typename boost::enable_if_c<cnv::is_converter<Converter, TypeIn, TypeOut>::value &&
                                cnv::is_func<FallbackFunc, TypeOut>::value,
    TypeOut>::type
    convert(TypeIn const& value_in, Converter const& converter, FallbackFunc const& fallback)
    {
        return boost::convert<TypeOut>(value_in, converter).value_or(fallback);
    }

    /// @brief The main Boost.Convert deployment interface with algorithms
    /// @details This the Boost.Convert main deployment interface with algorithms. For example,
    /// @code
    ///    boost::array<char const*, 3> strs = {{ " 5", "0XF", "not an int" }};
    ///    std::vector<int>             ints;
    ///    boost::cnv::cstringstream     cnv;
    ///
    ///    cnv(std::hex)(std::skipws);
    ///
    ///    std::transform(
    ///        strings.begin(),
    ///        strings.end(),
    ///        std::back_inserter(integers),
    ///        boost::convert<int>(cnv).value_or(INT_MAX));
    /// @endcode

    template<typename TypeOut, typename TypeIn, typename Converter>
    typename boost::enable_if<cnv::is_converter<Converter, TypeIn, TypeOut>, typename boost::cnv::adapter<TypeOut, Converter> >::type
    convert(Converter const& cnv)
    {
        return boost::cnv::adapter<TypeOut, Converter>(cnv);
    }
}

#endif // BOOST_CONVERT_HPP
