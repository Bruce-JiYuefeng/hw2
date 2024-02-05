#include "movie.h"
#include "util.h"
#include <sstream>

using namespace std;

Movie::Movie(const std::string category, const std::string name, double price, int qty, const std::string genre, const std::string rating) : 
    Product(category, name, price, qty), genre_(genre), rating_(rating) {}

Movie::~Movie() {}

set<string> Movie::keywords() const {
    set<string> keyWords = parseStringToWords(name_);
    keyWords.insert(genre_); // Genre is used as a whole keyword, not split
    return keyWords;
}

std::string Movie::displayString() const {
    std::ostringstream stream;
    stream << name_ << "\nGenre: " << genre_ << " Rating: " << rating_ << "\n" << price_ << " " << qty_ << " left.";
    return stream.str();
}


void Movie::dump(ostream& os) const {
    os << category_ << "\n" << name_ << "\n" << price_ << "\n" << qty_ << "\n" << genre_ << "\n" << rating_ << endl;
}
