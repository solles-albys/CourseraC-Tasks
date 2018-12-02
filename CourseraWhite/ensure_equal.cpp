#import <string>
#import <iostream>
#import <sstream>

using namespace std;

void EnsureEqual(const string& left, const string& right) {
    if (left != right) {
        std::stringstream ss;
        ss << left << " != " << right;
        throw runtime_error(ss.str());
    }
}