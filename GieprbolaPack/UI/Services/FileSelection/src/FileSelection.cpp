#include "../include/FileSelection.hpp"


bool FileSelection::is_selected(const QString& path) const
{
    return selected_paths_.contains(path);
}

void FileSelection::set_selected(const QString& path,bool selected)
{
    if (selected) {
        selected_paths_.insert(path);
    }
    else {
        selected_paths_.remove(path);
    }
}

void FileSelection::toggle_selected(const QString& path)
{
    if (selected_paths_.contains(path)) {
        selected_paths_.remove(path);
    }
    else {
        selected_paths_.insert(path);
    }
}

void FileSelection::clear()
{
    selected_paths_.clear();
}

bool FileSelection::empty() const
{
    return selected_paths_.isEmpty();
}

QStringList FileSelection::selected_paths() const
{
    return QStringList(selected_paths_.begin(), selected_paths_.end());
}