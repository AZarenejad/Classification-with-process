#include "Voter.hpp"
using namespace std;


int main(int argc, char *argv[]){
    // vector<int> v ={2,0,2,2,2,1,1,1,0,0,0};
    // sort(v.begin(),v.end());
    // for (int i=0;i<v.size();i++){
    //     cout<<v[i]<<" ";
    // }
    // cout<<endl;
    Voter voter(argv[1]);
    // cout<< voter.find_element_with_highest_frequency(v)<<endl;
    voter.get_result_from_linears();
    voter.decide_class_for_each_sample();
    voter.send_judgement_result_to_ensemble();
    exit(1);
}
