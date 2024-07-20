#include "counters.h"
#include <QCoreApplication>
#include <iostream>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_directory>" << std::endl;
        return 1;
    }

    QString inputDir = argv[1];

    std::string result = processDirectory(inputDir);
    std::cout << result;

    return 0;
}
