#include "mydatastore.h"
#include "util.h"
#include <iostream>
#include <algorithm>

MyDataStore::MyDataStore() {}

MyDataStore::~MyDataStore() {
    std::set<Product*> uniqueProducts;
    for (std::map<std::string, std::set<Product*>>::iterator pair = keywordToProductMap.begin(); pair != keywordToProductMap.end(); ++pair) {
        for (std::set<Product*>::iterator prod = pair->second.begin(); prod != pair->second.end(); ++prod) {
            uniqueProducts.insert(*prod);
        }
    }

    // delete each product
    for (std::set<Product*>::iterator prod = uniqueProducts.begin(); prod != uniqueProducts.end(); ++prod) {
        delete *prod;
    }

    // Clean up users
    for (std::map<std::string, User*>::iterator pair = usernameToUserMap.begin(); pair != usernameToUserMap.end(); ++pair) {
        delete pair->second;
    }

    keywordToProductMap.clear();
    usernameToUserMap.clear();
    userCarts.clear();
}


void MyDataStore::addProduct(Product* p) {
    std::set<std::string> keywords = p->keywords();
    for (const std::string& key : keywords) {
        keywordToProductMap[key].insert(p);
    }
}

void MyDataStore::addUser(User* u) {
    usernameToUserMap[u->getName()] = u;
}

std::vector<Product*> MyDataStore::search(std::vector<std::string>& terms, int type) {
    std::set<Product*> results;
    if (type == 0) { // AND search
        for (std::vector<std::string>::iterator term = terms.begin(); term != terms.end(); ++term) {
            std::set<Product*> termResults = keywordToProductMap[*term];
            if (results.empty()) {
                results = termResults;
            } else {
                results = setIntersection(results, termResults);
            }
        }
    } else if (type == 1) { // OR search
        for (std::vector<std::string>::iterator term = terms.begin(); term != terms.end(); ++term) {
            std::set<Product*> termResults = keywordToProductMap[*term];
            results = setUnion(results, termResults);
        }
    }
    return std::vector<Product*>(results.begin(), results.end());
}


void MyDataStore::dump(std::ostream& ofile) {
    ofile << "<products>" << std::endl;
    // Explicitly declare the type of 'pair'
    for (std::map<std::string, std::set<Product*>>::const_iterator pair = keywordToProductMap.begin(); pair != keywordToProductMap.end(); ++pair) {
        // Explicitly declare the type of 'prod'
        for (std::set<Product*>::const_iterator prod = pair->second.begin(); prod != pair->second.end(); ++prod) {
            (*prod)->dump(ofile);
        }
    }
    ofile << "</products>" << std::endl;

    ofile << "<users>" << std::endl;
    // Explicitly declare the type of 'pair'
    for (std::map<std::string, User*>::const_iterator pair = usernameToUserMap.begin(); pair != usernameToUserMap.end(); ++pair) {
        pair->second->dump(ofile);
    }
    ofile << "</users>" << std::endl;
}


void MyDataStore::addToCart(const std::string& username, Product* p) {
    std::map<std::string, User*>::iterator userIt = usernameToUserMap.find(username);
    if(userIt != usernameToUserMap.end()) {
        userCarts[username].push_back(p);
        std::cout << "Added to " << username << "'s cart: " << p->displayString() << std::endl; // For debugging
    }
    else {
        std::cerr << "User not found: " << username << std::endl; // For debugging
    }
}

void MyDataStore::viewCart(const std::string& username) {
    std::map<std::string, std::vector<Product*>>::iterator cartIt = userCarts.find(username);
    if(cartIt == userCarts.end() || cartIt->second.empty()) {
        std::cerr << "Invalid username" << std::endl;
        return;
    }

    std::cout << "User Cart [" << username << "]:" << std::endl;
    int count = 1;
    for(std::vector<Product*>::iterator it = cartIt->second.begin(); it != cartIt->second.end(); ++it) {
        std::cout << count << ". " << (*it)->displayString() << std::endl;
        ++count;
    }
}


void MyDataStore::buyCart(const std::string& username) {
    std::map<std::string, std::vector<Product*>>::iterator cartIt = userCarts.find(username);
    if(cartIt == userCarts.end()) {
        std::cerr << "Invalid username" << std::endl;
        return;
    }

    std::vector<Product*>::iterator it = cartIt->second.begin();
    while(it != cartIt->second.end()) {
        Product* product = *it;
        if(product->getQty() > 0 && usernameToUserMap[username]->getBalance() >= product->getPrice()) {
            usernameToUserMap[username]->deductAmount(product->getPrice());
            product->subtractQty(1);
            it = cartIt->second.erase(it); // Remove the item from the cart once purchased
        }
        else {
            ++it;
        }
    }
}
