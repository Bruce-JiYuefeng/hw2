#include "clothing.h"
#include "util.h"
#include <sstream>

using namespace std;

Clothing::Clothing(const std::string category, const std::string name, double price, int qty, const std::string size, const std::string brand) : 
    Product(category, name, price, qty), size_(size), brand_(brand) {}

Clothing::~Clothing() {}

set<string> Clothing::keywords() const {
    set<string> keyWords = parseStringToWords(name_);
    set<string> brandWords = parseStringToWords(brand_);
    keyWords.insert(brandWords.begin(), brandWords.end());
    return keyWords;
}

std::string Clothing::displayString() const {
    std::ostringstream stream;
    stream << name_ << "\nSize: " << size_ << " Brand: " << brand_ << "\n" << price_ << " " << qty_ << " left.";
    return stream.str();
}


void Clothing::dump(ostream& os) const {
    os << category_ << "\n" << name_ << "\n" << price_ << "\n" << qty_ << "\n" << size_ << "\n" << brand_ << endl;
}
