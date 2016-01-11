/*
 * File:   Utility.h
 * Author: jake
 *
 * Created on September 13, 2013, 12:26 PM
 */

#ifndef UTILITY_H
#define	UTILITY_H
//#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/generator_iterator.hpp>
#include <list>

typedef boost::minstd_rand base_generator_type;
typedef boost::random::uniform_real_distribution<> distribution_real;
typedef boost::variate_generator<base_generator_type&, distribution_real> gen_type_real;

class Utility
{
public:
    static double randomDouble(double min, double max);
    static int randomInt(int min, int max);
    static bool tossACoin();
    static void setSeed(int seed);
    static std::list<int> randomIntList(int number, int min, int max);

private:
    Utility();
    static boost::random::mt19937 gen;
    static base_generator_type generator;

};

#endif	/* UTILITY_H */

