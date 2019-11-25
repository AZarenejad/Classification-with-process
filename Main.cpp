#include "EnsembleClassifier.hpp"




int main(int argc, char* argv[]){
    EnsembleClassifier ensemble_classifier(argv[1],argv[2]);
    ensemble_classifier.open_weight_vectors_directory_set_file_names();
    ensemble_classifier.create_name_pipe();
    ensemble_classifier.create_linear_classifiers_pipes();
    ensemble_classifier.fork_voter();
    ensemble_classifier.fork_linear_classifiers();
    ensemble_classifier.write_on_classifiers_pipes_their_proper_filenames();
    ensemble_classifier.wait_for_linears();
    cout<<"finish waiting"<<endl;
    ensemble_classifier.wait_for_voter();
    ensemble_classifier.get_string_from_voter();
    ensemble_classifier.calc_print_accuracy();
    
}
