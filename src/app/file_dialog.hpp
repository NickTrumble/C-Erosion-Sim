#pragma once
#include <optional>
#include <filesystem>

enum class FileDialogMode {
    Save,
    Load
};

class FileDialog{
    public:
        static std::optional<std::filesystem::path> chooseSavePath();
        static std::optional<std::filesystem::path> chooseLoadPath();
        static std::optional<std::filesystem::path> choosePath(FileDialogMode mode);     
};