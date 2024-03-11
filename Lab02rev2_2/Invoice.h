#ifndef INVOICE_H
#define INVOICE_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Item{
public:
    string name_;
    char vat_;
    int amount_;
    float price_;

    Item(string name, float price, char vat, int amount);
};

class Invoice{
public:
    Invoice(double nips, double nipb);
    void add_item(Item i1);

    friend ostream &operator<<(ostream &str, Invoice &rhs);

private:
    double nips_, nipb_;
    vector <Item> items_;
};

#endif // INVOICE_H
