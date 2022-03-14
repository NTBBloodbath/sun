#include <fstream>
#include <iostream>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        // TODO: help function
        return 1;
    }

    std::ifstream file("my-file.txt");
    std::string output;

    if (!file)
        return -1;

    std::ostringstream ss;
    ss << file.rdbuf();
    output = ss.str();

    std::cout << output;
    return 0;
}
