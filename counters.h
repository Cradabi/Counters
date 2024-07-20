#pragma once

#include <QString>
#include <vector>
#include <tuple>
#include <atomic>
#include <string>

std::string processDirectory(const QString& dirPath);

void processFile(const QString& filePath,
                 std::vector<std::tuple<int, uint32_t, uint32_t>>& errors,
                 std::atomic<int>& errorCount);
