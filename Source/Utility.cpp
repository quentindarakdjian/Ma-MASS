// Copyright AI Environnement 2017


#include <ctime>
#include <list>
#include <vector>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/generator_iterator.hpp>
#include <boost/random/normal_distribution.hpp>

#include "Utility.h"

base_generator_type Utility::generator(std::time(0));

Utility::Utility() {
}

void Utility::setSeed(int seed){
    Utility::generator = base_generator_type(seed);
    Utility::randomDouble(0, 1);
}

double Utility::randomDouble(double min, double max){
    gen_type_real die_gen(Utility::generator, distribution_real(min, max));
    boost::generator_iterator<gen_type_real> die(&die_gen);
    double d = *die++;
    //std::cout << d << std::endl;
    return d;
}

double Utility::randomDouble() {
    return randomDouble(0.0,1.0);
}

bool Utility::randomBool(double chance){
    return randomDouble(0.0,1.0) < chance;
}

int Utility::randomInt(int min, int max){
    boost::random::uniform_int_distribution<> dist(min, max);
    return dist(Utility::generator);
}

std::list<int> Utility::randomIntList(int number, int min, int max){
    std::vector<int> numbers;
    std::list<int> randNumbers;
    for(int i =0; i < number; i++){
        numbers.push_back(i);
    }
    while(!numbers.empty()){
        int i = randomInt(0, numbers.size() -1);
        randNumbers.push_back(numbers[i]);
        numbers.erase(numbers.begin() +i);
    }
    return randNumbers;
}

double Utility::randomDoubleNormal(double mean, double sd){
    boost::normal_distribution<> dist(mean, sd);
    return dist(Utility::generator);
}

double Utility::randomWeibull(double scale, double shape) {
    // Draws a random number from a Weibull distribution,
    // proceeding by inversion of the Weibull cdf, defined as
    // F(x) = 1 - exp(-(x/scale)^shape)
    //double x = randomUniform(0.f,1.f);
    //double y = scale*pow(-log(1.f-random),1.f/shape);

    double random = randomDouble();
    return scale * pow(-log(1.f-random), 1.f / shape);
}

double Utility::probability(double m){
    double result = 0;
    if(m < log(FLT_MAX)){
        result = std::exp (m);
    }
    return result / (1.f + result);
}
