#pragma once
#include <QStringList>
#include <QSet>

class FileSelection
{
public:
    bool is_selected(const QString& path) const;
    void set_selected(const QString& path, bool selected);
    void toggle_selected(const QString& path);
    void clear();
    bool empty() const;
    QStringList selected_paths() const;

private:
    QSet<QString> selected_paths_;
};