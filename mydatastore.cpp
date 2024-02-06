#include "mydatastore.h"
#include "util.h"
#include <iostream>
#include <algorithm>

MyDataStore::MyDataStore() {}

MyDataStore::~MyDataStore() {
    std::set<Product*> uniqueProducts; 
    for (auto& pair : keywordToProductMap) {
        for (Product* prod : pair.second) {
            uniqueProducts.insert(prod); 
        }
    }

    // delete each product
    for (Product* prod : uniqueProducts) {
        delete prod;
    }

    // Clean up users
    for (auto& pair : usernameToUserMap) {
        delete pair.second;
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
        for (auto& term : terms) {
            std::set<Product*> termResults = keywordToProductMap[term];
            if (results.empty()) {
                results = termResults;
            } else {
                results = setIntersection(results, termResults);
            }
        }
    } else if (type == 1) { // OR search
        for (auto& term : terms) {
            std::set<Product*> termResults = keywordToProductMap[term];
            results = setUnion(results, termResults);
        }
    }
    return std::vector<Product*>(results.begin(), results.end());
}

void MyDataStore::dump(std::ostream& ofile) {
    ofile << "<products>" << std::endl;
    for (const auto& pair : keywordToProductMap) {
        for (const auto& prod : pair.second) {
            prod->dump(ofile);
        }
    }
    ofile << "</products>" << std::endl;

    ofile << "<users>" << std::endl;
    for (const auto& pair : usernameToUserMap) {
        pair.second->dump(ofile);
    }
    ofile << "</users>" << std::endl;
}

void MyDataStore::addToCart(const std::string& username, Product* p) {
    auto userIt = usernameToUserMap.find(username);
    if(userIt != usernameToUserMap.end()) {
        userCarts[username].push_back(p);
        std::cout << "Added to cart: " << p->displayString() << " for user: " << username << std::endl; // Debugging log
    }
    else {
        std::cerr << "User not found: " << username << std::endl;
    }
}


void MyDataStore::viewCart(const std::string& username) {
    if(userCarts.find(username) == userCarts.end()) {
        std::cerr << "User not found or cart is empty." << std::endl;
        return;
    }

    std::cout << "User Cart [" << username << "]:" << std::endl;
    int count = 1;
    for(auto* product : userCarts[username]) {
        std::cout << count << ". " << product->displayString() << std::endl;
        count++;
    }
}

void MyDataStore::buyCart(const std::string& username) {
    if(userCarts.find(username) == userCarts.end()) {
        std::cerr << "User not found or cart is empty." << std::endl;
        return;
    }

    auto& cart = userCarts[username];
    auto it = cart.begin();
    while(it != cart.end()) {
        Product* product = *it;
        if(product->getQty() > 0 && usernameToUserMap[username]->getBalance() >= product->getPrice()) {
            usernameToUserMap[username]->deductAmount(product->getPrice());
            product->subtractQty(1);
            it = cart.erase(it); // Remove the item from the cart once purchased
        }
        else {
            ++it;
        }
    }
}