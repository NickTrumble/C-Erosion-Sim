#include "file_dialog.hpp"

#define NOMINMAX
#include <windows.h>
#include <array>
#include <commdlg.h>

namespace   
{
    wchar_t filter[] =
        L"Terrain files (*.trn)\0*.trn\0"
        L"All files (*.*)\0*.*\0";  
} 


std::optional<std::filesystem::path> FileDialog::choosePath(FileDialogMode mode){
    std::array<wchar_t, 32768> fileName {};

    OPENFILENAMEW dialog {};
    dialog.lStructSize = sizeof(dialog);
    dialog.lpstrFilter = filter;
    dialog.lpstrFile = fileName.data();
    dialog.nMaxFile = static_cast<DWORD>(fileName.size());
    dialog.lpstrDefExt = L"trn";
    dialog.Flags = OFN_PATHMUSTEXIST;

    bool saving = mode == FileDialogMode::Save;

    if (saving) {
        dialog.Flags |= OFN_OVERWRITEPROMPT;
    } else {
        dialog.Flags |= OFN_FILEMUSTEXIST;
    }
    
    bool selected = saving ? GetSaveFileNameW(&dialog) : GetOpenFileNameW(&dialog);

    if (!selected){
        return std::nullopt;
    }

    return std::filesystem::path(fileName.data());
}

std::optional<std::filesystem::path> FileDialog::chooseSavePath(){
    return choosePath(FileDialogMode::Save);
}

std::optional<std::filesystem::path> FileDialog::chooseLoadPath(){
    return choosePath(FileDialogMode::Load);
}