#include "Invoice.h"

Invoice::Invoice(double nipb, double nips){
    if((nipb < 1000000000 || nipb > 9999999999) || (nips < 1000000000 || nips > 9999999999)){
        cout << "error - wrong nip";
    }
    else{
        nipb_ = nipb;
        nips_ = nips;
    }
}

Item::Item(string name, float price, char vat, int amount){
    if(vat == 'A' || vat == 'B' || vat == 'C'){
        vat_ = vat;
        name_ = name;
        price_ = price;
        amount_ = amount;
    }
    else{
        cout << "error - wrong vat!" << endl;
    }
}

void Invoice::add_item(Item i1){
    items_.emplace_back(i1);
}

ostream &operator<<(ostream &str, Invoice &rhs){
    str << endl;
    str << "------------------VAT invoice------------------" << endl;
    str << "===============================================" << endl;
    str << "Seller: " << setprecision(10) << rhs.nips_ << "     "; str << "Buyer: " << setprecision(10) << (rhs.nipb_) << endl;
    str << endl << endl;
    str << "\t \t  c.j. VAT  il.   net   total" << endl;

    int i = 1;
    float total1 = 0;
    float total2 = 0;
    for(auto value : rhs.items_){
        str << i << ". ";
        str << value.name_ << '\t' << "| ";
        str << setprecision(3) << value.price_ << '\t';
        str << value.vat_ << " | ";
        str << value.amount_ << '\t' << "| ";
        str << value.amount_*value.price_ << '\t' << "| ";
        if(value.vat_ == 'A'){
            str << setprecision(4) <<(value.amount_*value.price_)+ 0.23*(value.amount_*value.price_) << endl;
        }
        else if(value.vat_ == 'B'){
            str << setprecision(4) <<(value.amount_*value.price_)+ 0.08*(value.amount_*value.price_) << endl;
        }
        else{
            str << setprecision(4) <<(value.amount_*value.price_) << endl;
        }

        i++;

        total2 += (value.amount_*value.price_);
        if(value.vat_ == 'A'){
            total1 += (value.amount_*value.price_)+ 0.23*(value.amount_*value.price_);
        }
        else if(value.vat_ == 'B'){
            total1 += (value.amount_*value.price_)+ 0.08*(value.amount_*value.price_);
        }
        else{
            total1 += (value.amount_*value.price_);
        }
    }
    str << endl;
    str << "------------------------------------ TOTAL ----" << endl;
    str << "\t \t \t \t   " << setprecision(4) <<  total2 << " | " << total1 << endl;

    return str;
};
