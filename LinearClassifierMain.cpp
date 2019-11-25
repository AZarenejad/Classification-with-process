#include <iostream>
#include "LinearClassifier.hpp"
using namespace std;


int main(int argc, char const *argv[])
{
    LinearClassifier linear_classifier(atoi(argv[1]));
    linear_classifier.calc_scores();
    linear_classifier.send_class_of_each_sample_to_voter();
    exit(1);
}
