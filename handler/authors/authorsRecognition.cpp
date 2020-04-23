#include "authorsRecognition.h"
namespace AuthorsRecognition {
vector<string> build_n_grams(const string& alphabet, const size_t& n) {
    if (n==0) return vector<string>({""});
    vector<string> v = build_n_grams(alphabet, n-1);
    vector<string> vres;
    for(const char& c: alphabet) {
        for (const string& str : v){
            vres.push_back(c+str);
        }
    }
    return vres;
}
namespace{
vector<string> get_files_in_directory(const string& path){
    struct dirent *entry;
    vector<string> files;
    DIR *dir = opendir(path.c_str());
    if (dir == NULL) {
        closedir(dir);
        throw std::runtime_error("No such directory");
    }
    while ((entry = readdir(dir)) != NULL) {
        string filename = entry->d_name;
        if (filename == "." || filename == "..") continue;
        files.push_back(filename);
    }
    closedir(dir);
    return files;
}
}
unordered_map<string, Author> load_authors_data(const string& path){
    unordered_map<string, Author> authors;

    //auto start = chrono::steady_clock::now();
    //auto end = chrono::steady_clock::now();
    //auto diff = end - start;
    /*vector<std::thread> threads;
    std::mutex m;
    for (const string& filename : get_files_in_directory(path)) {
        threads.push_back(std::thread([&m,&authors, &filename, &path](){
            rapidjson::Document d;
            //auto start = chrono::steady_clock::now();
            ifstream ifs(path+'/'+filename);
            rapidjson::IStreamWrapper isw(ifs);

            d.ParseStream(isw);
            //auto end = chrono::steady_clock::now();
            //diff += end - start;
            if (d.HasParseError()) throw std::runtime_error("Error while parsing "+path+'/'+filename);
            if (!d.IsArray()) throw std::runtime_error("Wrong format(not an array) "+path+'/'+filename);

            for(rapidjson::Value::ConstValueIterator itr = d.Begin(); itr != d.End(); ++itr) {
                //cout << "hi" << (path+'/'+filename)<< (*itr)["id"].GetString() << "\n" ;
                if ((*itr)["authors"].IsNull()
                    || (*itr)["title"].IsNull()
                    || (*itr)["id"].IsNull()
                    || !itr->HasMember("length")
                    || (*itr)["length"].IsNull()) continue;
                if (!(*itr)["authors"].IsArray()) throw std::runtime_error("Wrong format(not an array) "+path+'/'+filename);
                if ((*itr)["authors"].Size() == 0) throw std::runtime_error("Wrong format(empty array) "+path+'/'+filename);
                string name = (*itr)["authors"][0].GetString();
                m.lock();
                if (authors.find(name) == authors.end()) {
                    authors[name] = Author();
                    authors[name].name = name;
                    authors[name].books = vector<Book>();
                }
                Author& author = authors[name];
                Book book;
                book.id = (*itr)["id"].GetString();
                book.title = (*itr)["title"].GetString();
                book.length = (*itr)["length"].GetInt();
                author.books.push_back(book);
                m.unlock();
            }
        }));
    }
    for(auto& t: threads) t.join();*/

    for (const string& filename : get_files_in_directory(path)) {
        rapidjson::Document d;
        //auto start = chrono::steady_clock::now();
        ifstream ifs(path+'/'+filename);
        rapidjson::IStreamWrapper isw(ifs);

        d.ParseStream(isw);
        //auto end = chrono::steady_clock::now();
        //diff += end - start;
        if (d.HasParseError()) throw std::runtime_error("Error while parsing "+path+'/'+filename);
        if (!d.IsArray()) throw std::runtime_error("Wrong format(not an array) "+path+'/'+filename);

        for(rapidjson::Value::ConstValueIterator itr = d.Begin(); itr != d.End(); ++itr) {
            //cout << "hi" << (path+'/'+filename)<< (*itr)["id"].GetString() << "\n" ;
            if ((*itr)["authors"].IsNull()
                || (*itr)["title"].IsNull()
                || (*itr)["id"].IsNull()
                || !itr->HasMember("length")
                || (*itr)["length"].IsNull()) continue;
            if (!(*itr)["authors"].IsArray()) throw std::runtime_error("Wrong format(not an array) "+path+'/'+filename);
            if ((*itr)["authors"].Size() == 0) throw std::runtime_error("Wrong format(empty array) "+path+'/'+filename);
            string name = (*itr)["authors"][0].GetString();
            if (authors.find(name) == authors.end()) {
                authors[name] = Author();
                authors[name].name = name;
                authors[name].books = vector<Book>();
            }
            Author& author = authors[name];
            Book book;
            book.id = (*itr)["id"].GetString();
            book.title = (*itr)["title"].GetString();
            book.length = (*itr)["length"].GetInt();
            book.author = name;
            author.books.push_back(book);
        }
    }
    //cout << chrono::duration <double, milli> (diff).count() << " ms" << endl;
    return authors;
}
namespace{
    unsigned long countSubstring(const string& str, const string& sub) {
        if (sub.length() == 0) return 0;
        unsigned long count = 0;
        for (size_t offset = str.find(sub); offset != string::npos;
         offset = str.find(sub, offset + sub.length()))
        {
            ++count;
        }
        return count;
    }
}
vector<unsigned long> get_n_grams_occurrences(const string& text, const vector<string>& n_grams){
    vector<unsigned long> res;
    transform(n_grams.begin(), n_grams.end(), back_inserter(res), [&text](const string& n_gram){
        return countSubstring(text, n_gram);
    });
    return res;
}
BookStatistics get_text_statisticts(const string& text, const vector<string>& n_grams, const unsigned long& number_of_patches) {
    BookStatistics statistics = {
    .length = text.length(),
    .n_grams_occurrences = get_n_grams_occurrences(text, n_grams)
    };
    //auto start0 = chrono::steady_clock::now();
    unsigned long total_occurrences = std::accumulate(statistics.n_grams_occurrences.begin(), statistics.n_grams_occurrences.end(), 0);
    transform(statistics.n_grams_occurrences.begin(), statistics.n_grams_occurrences.end(), back_inserter(statistics.n_grams_frequencies), [&total_occurrences](const unsigned long& n){
        return double(n)/total_occurrences;
    });
    //cout << "TTTime: " << chrono::duration <double, milli> (chrono::steady_clock::now()-start0).count() << " ms" << endl << endl;
    //start0 = chrono::steady_clock::now();
    for(const auto& n_gram: n_grams){
        statistics.n_grams_occurrences_patches.push_back(vector<unsigned long>());
        statistics.n_grams_frequencies_patches.push_back(vector<double>());
    }
    //cout << "TTTime: " << chrono::duration <double, milli> (chrono::steady_clock::now()-start0).count() << " ms" << endl << endl;
    //start0 = chrono::steady_clock::now();
    //auto diff0 = chrono::steady_clock::now() - start0;
    unsigned long patch_size = (text.length()+number_of_patches-1)/number_of_patches;
    for (unsigned long p=0;p<number_of_patches;p++){
        //start0 = chrono::steady_clock::now();
        vector<unsigned long> n_grams_occurrences = get_n_grams_occurrences(text.substr(patch_size*p, patch_size*(p+1)-1), n_grams);
        //diff0 += chrono::steady_clock::now() - start0;
        unsigned long total_occurrences = std::accumulate(n_grams_occurrences.begin(), n_grams_occurrences.end(), 0);
        vector<double> n_grams_frequencies;
        transform(n_grams_occurrences.begin(), n_grams_occurrences.end(), back_inserter(n_grams_frequencies), [&total_occurrences](const unsigned long& n){
            return double(n)/total_occurrences;
        });
        for(unsigned long i=0;i<n_grams.size();++i){
            statistics.n_grams_occurrences_patches[i].push_back(n_grams_occurrences[i]);
            statistics.n_grams_frequencies_patches[i].push_back(n_grams_frequencies[i]);
        }
    }
    //cout << "TTTime: " << chrono::duration <double, milli> (diff0).count() << " ms" << endl << endl;
    //start0 = chrono::steady_clock::now();
    for(unsigned long i=0;i<n_grams.size();++i){
        std::sort(statistics.n_grams_frequencies_patches[i].begin(), statistics.n_grams_frequencies_patches[i].end());
    }
    //cout << "TTTime: " << chrono::duration <double, milli> (chrono::steady_clock::now()-start0).count() << " ms" << endl << endl;

    return statistics;
}
string read_text(const string& path){
    ifstream ifs(path);
    std::string text;
    ifs.seekg(0, std::ios::end);
    text.reserve(ifs.tellg());
    ifs.seekg(0, std::ios::beg);

    text.assign((std::istreambuf_iterator<char>(ifs)),
            std::istreambuf_iterator<char>());
    return text;
}
char* read_text_2(const string& path){
    ifstream ifs(path);
    ifs.seekg(0, std::ios::end);
    long length = ifs.tellg();
    char *pch = new char[length];
    ifs.seekg(0, std::ios::beg);
    ifs.read (pch,length);
    return pch;
}
BookStatistics get_text_statisticts(const string& path, const string& allowed_symbols, const vector<string>& n_grams, const unsigned long& number_of_patches, const unsigned long& max_length, const unsigned long& n_grams_distances_amount){
    ifstream ifs(path);
    ifs.seekg(0, std::ios::end);
    long length = ifs.tellg();
    char *ptext = new char[length];
    ifs.seekg(0, std::ios::beg);
    ifs.read(ptext,length);

    BookStatistics statistics = {
    .length = length
    };
    const unsigned long n_gram_length = n_grams[0].length();
    for(const auto& n_gram: n_grams){
        statistics.n_grams_occurrences_patches.push_back(vector<unsigned long>());
        statistics.n_grams_occurrences.push_back(0);
        statistics.n_grams_frequencies_patches.push_back(vector<double>());
        statistics.n_grams_distances.push_back(vector<double>());
    }
    bool skip = false;
    //bool skipat = 0;
    for(char* p=ptext, *preal=ptext, *ptextend = ptext+length; preal!=ptextend; ++preal){
        if(allowed_symbols.find(*preal) == std::string::npos) { skip=true;--length; continue; }
        //if(skip) *(p++) = '-';
        skip=false;
        *(p++) = *preal;
    }
    if (length>max_length) length = max_length;
    unsigned long patch_size = (length-n_gram_length+1+number_of_patches-1)/number_of_patches;
    char* p=ptext, *ptextend = ptext+length-n_gram_length+1;
    long position = 0;
    vector<double> n_grams_last_positions;
    vector<vector<double>> temp_n_grams_distances;
    for(const auto& n_gram : n_grams) {
        n_grams_last_positions.push_back(-1);
        temp_n_grams_distances.push_back(vector<double>());
    }

    for(unsigned long i=0;i<number_of_patches;++i){
        for(unsigned long j=0;j<n_grams.size();++j){
            statistics.n_grams_occurrences_patches[j].push_back(0);
        }
        for(char *ppatchend = p+patch_size;p!=ppatchend;++p, ++position){
            if(*p == '-') continue;
            //cout << std::distance(ptext, p) << ' '<< length<< '\n';
            unsigned long n_gram_ind = 0;
            for(const auto& n_gram : n_grams){
                char *ptemp = p;
                char *arr = (char*)n_gram.c_str();
                char *pend = p+n_gram_length;
                while(ptemp!=pend) if(*arr == *ptemp) {++arr;++ptemp;} else break;
                if(ptemp == pend) {
                    ++statistics.n_grams_occurrences_patches[n_gram_ind].back();
                    /*if(n_grams_last_positions[n_gram_ind] >= 0) {
                        temp_n_grams_distances[n_gram_ind].push_back(position - n_grams_last_positions[n_gram_ind]);
                    }
                    n_grams_last_positions[n_gram_ind] = position;*/
                    //while(p!=pend && p!=ppatchend) p++;
                    //p--;
                    break;
                } else ++n_gram_ind;
            }
        }
        unsigned long total_occurrences_per_patch = 0;
        for(unsigned long j=0;j<n_grams.size();++j){
            total_occurrences_per_patch += statistics.n_grams_occurrences_patches[j].back();
        }
        for(unsigned long j=0;j<n_grams.size();++j){
            statistics.n_grams_frequencies_patches[j].push_back(double(statistics.n_grams_occurrences_patches[j].back())/total_occurrences_per_patch);
            statistics.n_grams_occurrences[j] +=statistics.n_grams_occurrences_patches[j].back();
        }
    }


    unsigned long total_occurrences = std::accumulate(statistics.n_grams_occurrences.begin(), statistics.n_grams_occurrences.end(), 0);

    for(unsigned long j=0;j<n_grams.size();++j){
        statistics.n_grams_frequencies.push_back(double(statistics.n_grams_occurrences[j])/total_occurrences);
    }
    for(unsigned long i=0;i<n_grams.size();++i){
        std::sort(statistics.n_grams_frequencies_patches[i].begin(), statistics.n_grams_frequencies_patches[i].end());
        std::sort(temp_n_grams_distances[i].begin(), temp_n_grams_distances[i].end());
        if (temp_n_grams_distances[i].size() >= n_grams_distances_amount) {
            const long d = temp_n_grams_distances[i].size()/n_grams_distances_amount;
            auto p = temp_n_grams_distances[i].begin();
            for(long j=0; j<n_grams_distances_amount;++j,p+=d ){
                statistics.n_grams_distances[i].push_back(*p);
            }
        }
    }

    delete[] ptext;
    return statistics;
}

string filter_text(string text, const string& allowed_symbols){
    text.erase(std::remove_if(text.begin(), text.end(), [&allowed_symbols](const char& c){
        return allowed_symbols.find(c) == std::string::npos;
    }), text.end());
    return text;
}
double frequencies_distance(const vector<double>& a, const vector<double>& b){
    auto ia = a.begin();
    auto ib = b.begin();
    //const long n = 500000;
    double result = 0;
    while(ia != a.end()){
        const double p1 = *ia;
        const double p2 = *ib;
        //cout << (((p1-p2)<(sqrt(p1*(1-p1)/n)*1.999))?0:1) << ((p1-p2)<(sqrt(p2*(1-p2)/n)*1.999))?0:1 << ' ';
        //result+=((p1-p2)<(sqrt(p1*(1-p1)/n)*2.999))?0:1;
        //result+=((p1-p2)<(sqrt(p2*(1-p2)/n)*2.999))?0:1;
        result+=std::abs((*ia)-(*ib));
        ++ia;
        ++ib;
    }
    return result;
}
double frequencies_distance(const BookStatistics& a, const BookStatistics& b){
    return frequencies_distance(a.n_grams_frequencies, b.n_grams_frequencies);
}
long double factorial(int N)
{
    if(N < 0)
        return 0;
    if (N == 0)
        return 1;
    else
        return N * factorial(N - 1);
}
double pstatistic_distance(const vector<double>& a, const vector<double>& b){
    unsigned long n = a.size();
    unsigned long m = b.size();
    long occurrences[n-1];
    auto ia = a.begin();
    for(long i=0;i<n-1;++i){
        const double& lb = *(ia++);
        const double& ub = *ia;
        //if(lb == ub) {
        //    occurrences[i] = std::count_if(b.begin(), b.end(), [&lb,&ub](double elem) {return elem == ub;});
        //    if(occurrences[i]>0) occurrences[i]--;
        //} else
        occurrences[i] = std::count_if(b.begin(), b.end(), [&lb,&ub](double elem) {return lb < elem && elem <= ub;});
    }
    unsigned long L = 0;
    double result = 0;
    for(long i=0, l=0;i<n-1;l-=occurrences[i++]){
        l=occurrences[i];
        for(long j=i+1;j<n;l+=occurrences[j++]){
            //cout << (double(l)/m)  << ' '<< (double(j-i)/n) << '\n';
            double p = double(l)/m;
            //double pint = double(j-i)/n;
            double pint = p;
            if(std::abs(p-double(j-i)/(n)) <= sqrt(pint*(1-pint)/m)*1.999) ++L;
            //if(std::abs(j-i-l) <= 2) ++L;
        }
    }
    /*for(unsigned long i=0, l=0;i<n-1;++i){
        for(unsigned long j=i+1;j<n;++j){
            const double& lb = a[i];
            const double& ub = b[j];
            unsigned long ll = std::count_if(b.begin(), b.end(), [&lb,&ub](double elem) {return lb < elem && elem <= ub;});
            double p = double(ll)/m;
            if(std::abs(p-double(j-i)/(n+1)) < sqrt(p*(1-p)/m)*1.95) ++L;
        }
    }*/
    //cout << L<< '\n' ;
    double t = 2.0*double(L)/(n*(n-1));
    double tint = 0.9772;

    //return ((0.9772  - t)<(sqrt(tint*(1-tint))*1.999))?0:1;
    return 1-t;
}
double pstatistic_distance(const BookStatistics& a, const BookStatistics& b){
    double sum=0;
    for(auto ia=a.n_grams_frequencies_patches.begin(), ib=b.n_grams_frequencies_patches.begin(), iaend=a.n_grams_frequencies_patches.end();ia!=iaend;++ia, ++ib){
        double d = max(pstatistic_distance(*ia, *ib),pstatistic_distance(*ib, *ia));
        //if(d<0.5) d=0;
        //cout << d << ' ';
        sum += d;
        //cout << (max(pstatistic_distance(*ia, *ib),pstatistic_distance(*ib, *ia))>0.5)?1:0 <<' ';
        //sum += (max(pstatistic_distance(*ia, *ib),pstatistic_distance(*ib, *ia))>0.7)?1:0;
    }
    return sum/a.n_grams_frequencies_patches.size()/2;
}
double pstatistic_distance_2(const BookStatistics& a, const BookStatistics& b){
    double sum=pstatistic_distance(a, b);
    for(auto ia=a.n_grams_distances.begin(), ib=b.n_grams_distances.begin(), iaend=a.n_grams_distances.end();ia!=iaend;++ia, ++ib){
        if (ia->size() == 0 || ib->size() == 0) sum+=2;
        else sum += pstatistic_distance(*ia, *ib)+pstatistic_distance(*ib, *ia);
    }
    return sum;
}
template< class TBookStatisticsConstIterator >
BookStatistics get_frequencies_ethalon(const TBookStatisticsConstIterator& il, const TBookStatisticsConstIterator& iu){
    BookStatistics statistics = get_pstatistic_ethalon(il, iu);
    unsigned long n = il->n_grams_occurrences.size();
    for(unsigned long i=0;i<n;++i){
        statistics.n_grams_occurrences.push_back(0);
    }
    for(TBookStatisticsConstIterator pstat=il;pstat != iu;pstat++){
        for(unsigned long i=0;i<n;++i){
            statistics.n_grams_occurrences[i] += pstat->n_grams_occurrences[i];
        }
    }
    unsigned long total_occurrences = std::accumulate(statistics.n_grams_occurrences.begin(), statistics.n_grams_occurrences.end(), 0);
    transform(statistics.n_grams_occurrences.begin(), statistics.n_grams_occurrences.end(), back_inserter(statistics.n_grams_frequencies), [&total_occurrences](const unsigned long& n){
        return double(n)/total_occurrences;
    });
    return statistics;
}
template< class TBookStatistics >
BookStatistics get_frequencies_ethalon(const vector<TBookStatistics>& v){
    return get_frequencies_ethalon(v.begin(), v.end());
}
template BookStatistics get_frequencies_ethalon<vector<BookStatistics>::const_iterator>(const vector<BookStatistics>::const_iterator& il, const vector<BookStatistics>::const_iterator& iu);
template BookStatistics get_frequencies_ethalon<vector<Book>::const_iterator>(const vector<Book>::const_iterator& il, const vector<Book>::const_iterator& iu);
template BookStatistics get_frequencies_ethalon<BookStatistics>(const vector<BookStatistics>& v);
template BookStatistics get_frequencies_ethalon<Book>(const vector<Book>& v);
template< class TBookStatisticsConstIterator >
BookStatistics get_pstatistic_ethalon(const TBookStatisticsConstIterator& il, const TBookStatisticsConstIterator& iu){
    BookStatistics statistics;
    unsigned long n = il->n_grams_frequencies_patches.size();
    unsigned long m = il->n_grams_frequencies_patches[0].size();
    unsigned long d = std::distance(il, iu);
    for(unsigned long i=0;i<n;++i){
        statistics.n_grams_frequencies_patches.push_back(vector<double>());
    }
    vector<double> temp_vec;
    temp_vec.reserve(d*m);

    for(unsigned long i=0;i<n;++i){
        temp_vec.clear();
        for(TBookStatisticsConstIterator pstat=il;pstat != iu;++pstat){
            temp_vec.insert(temp_vec.end(), pstat->n_grams_frequencies_patches[i].begin(), pstat->n_grams_frequencies_patches[i].end());
        }
        std::sort(temp_vec.begin(), temp_vec.end());
        for(auto [j,p] = std::tuple<long, vector<double>::iterator>{0, temp_vec.begin()+(d/2)}; j<m;++j,p+=d ){
            statistics.n_grams_frequencies_patches[i].push_back(*p);
        }
    }
    /*for(TBookStatisticsConstIterator pstat=il;pstat != iu;++pstat){
        for(unsigned long i=0;i<n;++i){
            statistics.n_grams_frequencies_patches[i].insert(statistics.n_grams_frequencies_patches[i].begin(), pstat->n_grams_frequencies_patches[i].begin(), pstat->n_grams_frequencies_patches[i].end());
        }
    }
    for(unsigned long i=0;i<n;++i){
        std::sort(statistics.n_grams_frequencies_patches[i].begin(), statistics.n_grams_frequencies_patches[i].end());
    }*/
    return statistics;
}
template< class TBookStatistics >
BookStatistics get_pstatistic_ethalon(const vector<TBookStatistics>& v){
    return get_pstatistic_ethalon(v.begin(), v.end());
}
template BookStatistics get_pstatistic_ethalon<vector<BookStatistics>::const_iterator>(const vector<BookStatistics>::const_iterator& il, const vector<BookStatistics>::const_iterator& iu);
template BookStatistics get_pstatistic_ethalon<vector<Book>::const_iterator>(const vector<Book>::const_iterator& il, const vector<Book>::const_iterator& iu);
template BookStatistics get_pstatistic_ethalon<BookStatistics>(const vector<BookStatistics>& v);
template BookStatistics get_pstatistic_ethalon<Book>(const vector<Book>& v);
template< class TBookStatisticsConstIterator >
BookStatistics get_pstatistic_2_ethalon(const TBookStatisticsConstIterator& il, const TBookStatisticsConstIterator& iu){
    BookStatistics statistics = get_pstatistic_ethalon(il, iu);
    unsigned long n = il->n_grams_distances.size();
    unsigned long m = il->n_grams_distances[0].size();
    unsigned long d = std::distance(il, iu);
    for(unsigned long i=0;i<n;++i){
        statistics.n_grams_distances.push_back(vector<double>());
    }
    vector<double> temp_vec;
    temp_vec.reserve(d*m);

    for(unsigned long i=0;i<n;++i){
        temp_vec.clear();
        for(TBookStatisticsConstIterator pstat=il;pstat != iu;++pstat){
            temp_vec.insert(temp_vec.end(), pstat->n_grams_distances[i].begin(), pstat->n_grams_distances[i].end());
        }
        std::sort(temp_vec.begin(), temp_vec.end());
        for(auto [j,p] = std::tuple<long, vector<double>::iterator>{0, temp_vec.begin()+(d/2)}; j<m;++j,p+=d ){
            statistics.n_grams_distances[i].push_back(*p);
        }
    }
    return statistics;
}
template< class TBookStatistics >
BookStatistics get_pstatistic_2_ethalon(const vector<TBookStatistics>& v){
    return get_pstatistic_2_ethalon(v.begin(), v.end());
}
template BookStatistics get_pstatistic_2_ethalon<vector<BookStatistics>::const_iterator>(const vector<BookStatistics>::const_iterator& il, const vector<BookStatistics>::const_iterator& iu);
template BookStatistics get_pstatistic_2_ethalon<vector<Book>::const_iterator>(const vector<Book>::const_iterator& il, const vector<Book>::const_iterator& iu);
template BookStatistics get_pstatistic_2_ethalon<BookStatistics>(const vector<BookStatistics>& v);
template BookStatistics get_pstatistic_2_ethalon<Book>(const vector<Book>& v);

double calculate_split_deviation(const string& author_name, const BookStatistics& ethalon, const unordered_map<string, vector<AuthorsRecognition::Book>>& authors, double distance_metric(const BookStatistics&, const BookStatistics&)){
    //cout << "calculate_split_deviation " << author_name<< '\n' ;
    const vector<Book>& authorBooks = authors.at(author_name);
    const unsigned long n = authorBooks.size();
    unsigned int m = 0;
    vector<double> container;
    //cout << "own distances " ;
    transform(
        authorBooks.begin(),
        authorBooks.end(),
        std::back_inserter(container),
        [&ethalon, &distance_metric](const BookStatistics& stat){
            double d = distance_metric(stat, ethalon);
            //cout << d << " " ;
            return d;
        }
    );
    //cout << '\n';
    for(const auto& element : authors) {
        if(element.first == author_name) continue;
        m+=element.second.size();
        transform(
            element.second.begin(),
            element.second.end(),
            std::back_inserter(container),
            [&ethalon, &distance_metric](const BookStatistics& stat){
                return distance_metric(stat, ethalon);
            }
        );
    }
    vector<double>::iterator a = container.begin();
    vector<double>::iterator b = a+n;
    vector<double>::iterator c = container.end();
    vector<double> probs;
    transform(a,c,std::back_inserter(probs),[&a, &b, &c, &n, &m](const double& p){
        return double(std::count_if(a, b, [&p](const double& _p) {return _p > p;}))/n + double(std::count_if(b, c, [&p](const double& _p) {return _p <= p;}))/m ;
    });
    return container[std::distance(probs.begin(), std::min_element(probs.begin(), probs.end()))];
}

unordered_map<string, double> calculate_split_deviations(const unordered_map<string, vector<AuthorsRecognition::Book>>& authors, BookStatistics get_ethalon(const vector<Book>& v), double distance_metric(const BookStatistics&, const BookStatistics&)){
    unordered_map<string, double> split_deviations;
    unsigned long total_number_of_books = 0;

    vector<std::thread> threads;
    std::mutex m;
    for(const auto& element : authors) {
        threads.push_back(std::thread([&element, &get_ethalon, &m, &split_deviations, &authors, &distance_metric](){
            BookStatistics ethalon = get_ethalon(element.second);
            double d = calculate_split_deviation(element.first, ethalon, authors, distance_metric);
            m.lock();
            split_deviations[element.first] = d;
            m.unlock();
        }));
    }
    for(auto& t: threads) t.join();

    return split_deviations;
}
double calculate_global_split_deviation(const unordered_map<string, vector<AuthorsRecognition::Book>>& authors, BookStatistics get_ethalon(const vector<Book>& v), double distance_metric(const BookStatistics&, const BookStatistics&)){
    const unsigned long n = authors.size();
    unsigned long m = 0;
    vector<double> container;
    container.reserve(n*m);
    for(const auto& [name, books] :  authors) {
        BookStatistics ethalon = get_ethalon(books);
        m+=books.size();
        for(const auto& book : books){
            container.push_back(distance_metric(book, ethalon));
        }
    }
    for(const auto& [name1, books1] :  authors) {
        BookStatistics ethalon = get_ethalon(books1);
        for(const auto& [name2, books2] :  authors) {
            if(name2 == name1) continue;
            for(const auto& book : books2){
                container.push_back(distance_metric(book, ethalon));
            }
        }
    }
    vector<double>::iterator a = container.begin();
    vector<double>::iterator b = a+m;
    vector<double>::iterator c = container.end();
    vector<double> probs;
    transform(a,c,std::back_inserter(probs),[&a, &b, &c, &n, &m](const double& p){
        return double(std::count_if(a, b, [&p](const double& _p) {return _p > p;}))/m + double(std::count_if(b, c, [&p](const double& _p) {return _p <= p;}))/(m*(n-1)) ;
    });
    return container[std::distance(probs.begin(), std::min_element(probs.begin(), probs.end()))];
}


template< class TBookStatistics >
vector<BookStatistics> calculate_grouped_ethalons(const vector<TBookStatistics>& books, const double& max_deviation, BookStatistics get_ethalon(const vector<TBookStatistics>& v), double distance_metric(const BookStatistics&, const BookStatistics&)){
    vector<pair<BookStatistics, vector<TBookStatistics>>> ethalons = {pair<BookStatistics, vector<TBookStatistics>>((get_ethalon(books)), books)};

    //cout << "ethalons[0].first.n_grams_frequencies_patches.size() = " << ethalons[0].first.n_grams_frequencies_patches.size() << '\n' ;
    //for(const auto& y: ethalons[0].first.n_grams_frequencies_patches) {
    //    for(const auto& x: y) cout << x << ' ' ;
    //    cout  << '\n';
    //}
    //for(const auto& book: books) {
    //    cout << "book \n" << distance_metric(book, ethalons[0].first) << '\n' ;
    //    for(const auto& y: book.n_grams_frequencies_patches) {
    //        for(const auto& x: y) cout << x << ' ' ;
    //        cout  << '\n';
    //    }
    //}
    //ethalons[0].second.insert(ethalons[0].second.begin(), books.begin(), books.end());
    //std::copy
    bool ok = true;
    //cout << "calculate_grouped_ethalons "<< max_deviation << '\n';
    while (ok){
        ok=false;
        for(auto itethalon = ethalons.begin(); itethalon != ethalons.end(); ++itethalon){
            auto& [ethalon, bookspatch] = *itethalon;
            if(bookspatch.size()==1) continue;
            //for(const auto& y: ethalon.n_grams_frequencies_patches) {
            //    for(const auto& x: y) cout << x << ' ' ;
            //    cout  << '\n';
            //}
            //cout << "ethalons: "<< ethalons.size()<<"\n";
            //cout << "books: "<< bookspatch.size()<<"\n";
            double own_distances[bookspatch.size()];
            transform(bookspatch.begin(), bookspatch.end(), own_distances, [&ethalon, &distance_metric](const auto& book){ return distance_metric(ethalon, book); });
            //for(const auto&d:own_distances ) cout << d << ' '; cout << '\n';
            const unsigned long max_ind = std::distance(own_distances, std::max_element(own_distances, own_distances+bookspatch.size()));

            if(own_distances[max_ind] > max_deviation){
                //cout << "drop "<< max_ind << '\n';
                ok=true;
                //const unsigned long min_ind = std::distance(own_distances, std::min_element(own_distances, own_distances+bookspatch.size()));
                //const TBookStatistics& minbook = bookspatch[min_ind];
                const TBookStatistics maxbook = bookspatch[max_ind];
                bookspatch.erase(bookspatch.begin()+max_ind);
                ethalon = get_ethalon(bookspatch);

                auto itethalon2 = itethalon;
                for(; itethalon2 != ethalons.end(); ++itethalon2){
                    if (distance_metric(itethalon2->first, maxbook) <= max_deviation) break;
                }
                if (itethalon2 == ethalons.end()) {
                    ethalons.push_back(pair<BookStatistics, vector<TBookStatistics>>(get_ethalon({maxbook}), {maxbook}));
                } else {
                    itethalon2->second.push_back(maxbook);
                    itethalon2->first = get_ethalon(itethalon2->second);
                }
                //cout << "drop 1 " <<bookspatch.size()<< '\n';
                //pair<BookStatistics, vector<TBookStatistics>> ten = ;
                //cout << ten.second.size();
                //cout << "drop 1 " <<bookspatch.size()<< '\n';
                //ethalons.push_back(pair<BookStatistics, vector<TBookStatistics>>(get_ethalon({maxbook}), {maxbook}));
                //cout << "drop 2" << '\n';
                //cout << "drop 4" << '\n';
                //cout << "drop 5" << '\n';
                break;
            }
        }
        //cout << "AAAAAAAAAAA " << '\n';
        if (ok){
            //cout << "here 1" << '\n';
            bool recalc = true;
            while(recalc){
                //cout << "here 2" << '\n';
                recalc = false;
                //cout << "|||||||||||||||||" << '\n';
                //for(const auto& [ethalon, bookspatch]: ethalons){
                //     cout << "||||| " ;
                //    for(const auto& book :bookspatch) {
                //        for(const auto& [ethalon1, bookspatch1]: ethalons){
                //            cout << distance_metric(book, ethalon1) << ' ';
                //        } cout << '\n';
                //    }
                //}
                for(auto p_current_element = ethalons.begin(); p_current_element!=ethalons.end();++p_current_element){
                    if(p_current_element->second.size()) continue;
                    if(recalc) break;
                    for(auto p_book = p_current_element->second.begin(); p_book!=p_current_element->second.end();++p_book){
                        if(recalc) break;
                        double distances[ethalons.size()];
                        transform(ethalons.begin(), ethalons.end(), distances, [&p_book, &distance_metric](const auto& element) { return distance_metric(element.first, *p_book); });
                        auto p_min_element = ethalons.begin()+std::distance(distances, std::min_element(distances, distances+ethalons.size()));
                        if (p_min_element != p_current_element){
                            recalc = true;
                            p_min_element->second.push_back(*p_book);
                            p_min_element->first = get_ethalon(p_min_element->second);
                            p_current_element->second.erase(p_book);
                            p_current_element->first = get_ethalon(p_current_element->second);
                        }
                    }
                }
            }
        }
    }
    vector<BookStatistics> result;
    transform(ethalons.begin(), ethalons.end(), std::back_inserter(result), [](const auto& element){ return element.first; });
    return result;
}
template vector<BookStatistics> calculate_grouped_ethalons<BookStatistics>(const vector<BookStatistics>& books, const double& max_deviation, BookStatistics get_ethalon(const vector<BookStatistics>& v), double distance_metric(const BookStatistics&, const BookStatistics&));
template vector<BookStatistics> calculate_grouped_ethalons<Book>(const vector<Book>& books, const double& max_deviation, BookStatistics get_ethalon(const vector<Book>& v), double distance_metric(const BookStatistics&, const BookStatistics&));


template< class TBookStatistics >
vector<BookStatistics> calculate_grouped_ethalons2(const vector<TBookStatistics>& books, const double& max_deviation, BookStatistics get_ethalon(const vector<TBookStatistics>& v), double distance_metric(const BookStatistics&, const BookStatistics&)){
    vector<pair<BookStatistics, vector<TBookStatistics>>> ethalons;

    for(const auto& book : books) {
        ethalons.push_back(pair<BookStatistics, vector<TBookStatistics>>(book, {book}));
    }
    bool ok = true;
    while (ok){
        if(ethalons.size()<=1) break;
        ok = false;
        double min_distance = std::numeric_limits<double>::infinity();
        auto ethA = ethalons.begin() , ethB = ethA+1;
        for(auto iel = ethalons.begin(), ielend = ethalons.end()-1; iel != ielend;++iel){
            for(auto ier = iel+1, ierend = ethalons.end(); ier!= ierend;++ier){
                const double d = distance_metric(iel->first, ier->first);
                //cout << d << ' ';
                if(min_distance>d){
                    min_distance = d;
                    ethA = iel;
                    ethB = ier;
                }
            }
        }
        //cout << '\n';
        //cout << "min_distance << ' ' << max_deviation - "<< min_distance << ' ' << max_deviation << '\n';
        /*if(min_distance<max_deviation*2){
            ok = true;
            ethA->second.insert(ethA->second.begin(), ethB->second.begin(), ethB->second.end());
            ethA->first = get_ethalon(ethA->second);
            ethalons.erase(ethB);
        }*/
        vector<TBookStatistics> tempvec;
        tempvec.insert(tempvec.end(), ethA->second.begin(), ethA->second.end());
        tempvec.insert(tempvec.end(), ethB->second.begin(), ethB->second.end());
        auto tempethalon = get_ethalon(tempvec);

        double distances[tempvec.size()];
        transform(tempvec.begin(), tempvec.end(), distances, [&tempethalon, &distance_metric](const auto& element){ return distance_metric(tempethalon, element); });
        const auto it = std::max_element(distances, distances+tempvec.size());
        if(*it<=max_deviation){
            ok = true;
            ethA->second.insert(ethA->second.begin(), ethB->second.begin(), ethB->second.end());
            ethA->first = get_ethalon(ethA->second);
            ethalons.erase(ethB);
        }
        //cout << "calculate_grouped_ethalons2 for end\n";
    }
    //cout << "calculate_grouped_ethalons2 end\n";
    vector<BookStatistics> result;
    transform(ethalons.begin(), ethalons.end(), std::back_inserter(result), [](const auto& element){ return element.first; });
    return result;
}
template vector<BookStatistics> calculate_grouped_ethalons2<BookStatistics>(const vector<BookStatistics>& books, const double& max_deviation, BookStatistics get_ethalon(const vector<BookStatistics>& v), double distance_metric(const BookStatistics&, const BookStatistics&));
template vector<BookStatistics> calculate_grouped_ethalons2<Book>(const vector<Book>& books, const double& max_deviation, BookStatistics get_ethalon(const vector<Book>& v), double distance_metric(const BookStatistics&, const BookStatistics&));


string predict_author(const BookStatistics& stat, const unordered_map<string, vector<BookStatistics>>& grouped_ethelons, double distance_metric(const BookStatistics&, const BookStatistics&)){
    unordered_map<string, double> authors_min_distances;
    for(const auto& element: grouped_ethelons){
        double distances[element.second.size()];
        transform(element.second.begin(), element.second.end(), distances, [&stat, &distance_metric](const BookStatistics& stat_ethalon){
            return distance_metric(stat, stat_ethalon);
        });
        authors_min_distances[element.first] = *(std::min_element(distances, distances + element.second.size()));
        //cout << element.first << ':'<< authors_min_distances[element.first] << '\n';
    }
    const auto& i = std::min_element(authors_min_distances.begin(), authors_min_distances.end(), [](const auto& a, const auto& b){
        return a.second<b.second;
    });
    return i->first;
}
double get_n_gram_importance(const unordered_map<string, vector<AuthorsRecognition::Book>>& authors, const long& n_gram_index){
    double importance = 0;
    double A, B, C;
    unsigned long n = 0;
    for(const auto& [name, books]: authors) {
        unsigned long m = books.size();
        n+=m;
        double c =0;
        for(const auto& book : books) {
            const auto f = book.n_grams_frequencies[n_gram_index];
            //cout << f << '\n';
            A += pow(f,2);
            B += f;
            c += f;
        }
        C+=pow(c,2)/m;
    }
    A /=n;
    B = pow(B/n,2);
    C /=n;
    //cout << "A, B, C" << A << " "<< B << " "<< C << '\n';
    return (A-C)/(A-B);
}
vector<double> get_n_grams_importances(const unordered_map<string, vector<AuthorsRecognition::Book>>& authors){
    vector<double> n_grams_importances ;
    const auto n_grams_amount = authors.begin()->second[0].n_grams_frequencies.size();
    for(long i =0;i<n_grams_amount;++i) n_grams_importances.push_back(get_n_gram_importance(authors, i));
    return n_grams_importances;
}
}
