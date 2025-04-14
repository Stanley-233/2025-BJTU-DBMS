#include "include/CommandLine.h"
using namespace std;

int main() {
    auto app = CommandLine::getInstance();
    app.start();
}