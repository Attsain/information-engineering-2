#include <iostream>
#include "Invoice.h"

using namespace std;

int main()
{
    Invoice inv(1234567890, 9876543210);
    inv.add_item(Item("M3 screw", 0.37, 'A', 100));
    inv.add_item(Item("2mm drill", 2.54, 'B', 2));
    cout << inv << endl;

    return 0;
}
