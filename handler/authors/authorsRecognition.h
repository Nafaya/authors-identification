#include <iostream>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <dirent.h>
#include <sys/types.h>
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include <fstream>
#include <chrono>
#include <algorithm>
#include <numeric>
#include <streambuf>
#include <thread>
#include <future>
#include <cmath>
#include <random>
#include <tuple>
using namespace std;
namespace AuthorsRecognition {
struct BookStatistics{
    unsigned long length;
    vector<unsigned long> n_grams_occurrences;
    vector<double> n_grams_frequencies;
    vector<vector<unsigned long>> n_grams_occurrences_patches;
    vector<vector<double>> n_grams_frequencies_patches;
    vector<vector<double>> n_grams_distances;
};
struct Book:BookStatistics{
    string id;
    string title;
    string author;
};
struct Author{
    string name;
    vector<Book> books;
};
vector<string> build_n_grams(const string&, const size_t&);
unordered_map<string, Author> load_authors_data(const string&);
vector<unsigned long> get_n_grams_occurrences(const string&, const vector<string>&);
BookStatistics get_text_statisticts(const string& text, const vector<string>& n_grams, const unsigned long& number_of_patches);
BookStatistics get_text_statisticts(const string& path, const string& alphabet, const vector<string>& n_grams, const unsigned long& number_of_patches, const unsigned long& max_length, const unsigned long& n_grams_distances_amount);
string read_text(const string&);
char* read_text_2(const string&);
string filter_text(string, const string&);
double frequencies_distance(const vector<double>& a, const vector<double>& b);
double frequencies_distance(const BookStatistics&, const BookStatistics&);
double pstatistic_distance(const vector<double>& a, const vector<double>& b);
double pstatistic_distance(const BookStatistics&, const BookStatistics&);
double pstatistic_distance_2(const BookStatistics&, const BookStatistics&);
template< class TBookStatistics >
BookStatistics get_frequencies_ethalon(const vector<TBookStatistics>& v);
template< class TBookStatisticsConstIterator >
BookStatistics get_frequencies_ethalon(const TBookStatisticsConstIterator& il, const TBookStatisticsConstIterator& iu);
template< class TBookStatistics >
BookStatistics get_pstatistic_ethalon(const vector<TBookStatistics>&);
template< class TBookStatisticsConstIterator >
BookStatistics get_pstatistic_ethalon(const TBookStatisticsConstIterator& il, const TBookStatisticsConstIterator& iu);
template< class TBookStatistics >
BookStatistics get_pstatistic_2_ethalon(const vector<TBookStatistics>&);
template< class TBookStatisticsConstIterator >
BookStatistics get_pstatistic_2_ethalon(const TBookStatisticsConstIterator& il, const TBookStatisticsConstIterator& iu);

double calculate_split_deviation(const string& author_name, const BookStatistics& ethalon, const unordered_map<string, vector<AuthorsRecognition::Book>>& authors, double distance_metric(const BookStatistics&, const BookStatistics&));
unordered_map<string, double> calculate_split_deviations(const unordered_map<string, vector<AuthorsRecognition::Book>>& authors, BookStatistics get_ethalon(const vector<Book>& v), double distance_metric(const BookStatistics&, const BookStatistics&));
double calculate_global_split_deviation(const unordered_map<string, vector<AuthorsRecognition::Book>>& authors, BookStatistics get_ethalon(const vector<Book>& v), double distance_metric(const BookStatistics&, const BookStatistics&));

template< class TBookStatistics >
vector<BookStatistics> calculate_grouped_ethalons(const vector<TBookStatistics>& books, const double& max_distance, BookStatistics get_ethalon(const vector<TBookStatistics>& v), double distance_metric(const BookStatistics&, const BookStatistics&));

template< class TBookStatistics >
vector<BookStatistics> calculate_grouped_ethalons2(const vector<TBookStatistics>& books, const double& max_distance, BookStatistics get_ethalon(const vector<TBookStatistics>& v), double distance_metric(const BookStatistics&, const BookStatistics&));

string predict_author(const BookStatistics& stat, const unordered_map<string, vector<BookStatistics>>& grouped_ethelons, double (const BookStatistics&, const BookStatistics&));
double get_n_gram_importance(const unordered_map<string, vector<AuthorsRecognition::Book>>& authors, const long& n_gram_index);
vector<double> get_n_grams_importances(const unordered_map<string, vector<AuthorsRecognition::Book>>& authors);
}
