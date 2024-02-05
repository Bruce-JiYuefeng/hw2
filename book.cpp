#include "book.h"
#include "util.h"
#include <sstream>

using namespace std;

Book::Book(const std::string category, const std::string name, double price, int qty, const std::string isbn, const std::string author) : 
    Product(category, name, price, qty), isbn_(isbn), author_(author) {}

Book::~Book() {}

set<string> Book::keywords() const {
    set<string> keyWords = parseStringToWords(name_);
    set<string> authorWords = parseStringToWords(author_);
    keyWords.insert(isbn_);
    keyWords.insert(authorWords.begin(), authorWords.end());
    return keyWords;
}

std::string Book::displayString() const {
    std::ostringstream stream;
    stream << name_ << "\nAuthor: " << author_ << " ISBN: " << isbn_ << "\n" << price_ << " " << qty_ << " left.";
    return stream.str();
}


void Book::dump(ostream& os) const {
    os << category_ << "\n" << name_ << "\n" << price_ << "\n" << qty_ << "\n" << isbn_ << "\n" << author_ << endl;
}
