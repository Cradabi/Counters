#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <atomic>
#include <tuple>

void processFile(const QString& filePath,
                 std::vector<std::tuple<int, uint32_t, uint32_t>>& errors,
                 std::atomic<int>& errorCount) {
    std::ifstream file(filePath.toStdString(), std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file: " << filePath.toStdString() << std::endl;
        return;
    }

    std::vector<uint32_t> lastValues(32, 0);
    std::vector<bool> isFirstRead(32, true);

    uint32_t value = 0;
    char buffer[sizeof(uint32_t)];
    int channel = 0;
    while (file.read(buffer, sizeof(buffer))) {
        std::copy(buffer, buffer + sizeof(buffer), reinterpret_cast<char*>(&value));
        if (isFirstRead[channel]) {
            lastValues[channel] = value;
            isFirstRead[channel] = false;
        } else {
            uint32_t expected = 0;
            if (lastValues[channel] == 0xFFFFFFFF) {
                expected = 0;
            } else {
                expected = lastValues[channel] + 1;
            }
            if (value != expected) {
                errors.emplace_back(channel, expected, value);
                errorCount++;
            }
            lastValues[channel] = value;
        }
        channel = (channel + 1) % 32;
    }

    file.close();
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <directory_path>" << std::endl;
        return 1;
    }

    QString dirPath = QString::fromLocal8Bit(argv[1]);
//    QString dirPath = "data_raw_32_rand_ch_offs_break";
    QDir dir(dirPath);
    QStringList filePaths = dir.entryList(QDir::Files);

    if (filePaths.size() != 8) {
        std::cerr << "Error: Directory should contain exactly 8 files." << std::endl;
        return 1;
    }

    std::vector<std::vector<std::tuple<int, uint32_t, uint32_t>>> allErrors(8);
    std::vector<std::atomic<int>> errorCounts(8);
    std::vector<std::thread> threads;

    for (int i = 0; i < 8; ++i) {
        QString fullPath = dir.filePath(filePaths[i]);
        threads.emplace_back(processFile, fullPath, std::ref(allErrors[i]), std::ref(errorCounts[i]));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    for (int i = 0; i < 8; ++i) {
        QString fileName = QFileInfo(filePaths[i]).fileName();
        for (const auto& error : allErrors[i]) {
            int channel = std::get<0>(error);
            uint32_t expected = std::get<1>(error);
            uint32_t actual = std::get<2>(error);
            std::cout << "File: " << fileName.toStdString() << ", Channel: " << channel
                      << ", Expected: " << expected << ", Actual: " << actual << std::endl;
        }
        std::cout << "File: " << fileName.toStdString() << ", Total errors: " << errorCounts[i] << std::endl;
    }

    return 0;
}
