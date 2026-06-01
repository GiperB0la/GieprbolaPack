#pragma once
#include <filesystem>
#include <vector>

#include <QAbstractListModel>
#include <QDateTime>
#include <QSet>

#include "../../../Core/FileScanner/include/FileScanner.hpp"

class FileSystemModel : public QAbstractListModel
{
    Q_OBJECT

        Q_PROPERTY(QString currentPath
            READ current_path
            WRITE set_current_path
            NOTIFY current_path_changed)

        Q_PROPERTY(int folderCount
            READ folder_count
            NOTIFY stats_changed)

        Q_PROPERTY(int fileCount
            READ file_count
            NOTIFY stats_changed)

        Q_PROPERTY(QString totalSizeText
            READ total_size_text
            NOTIFY stats_changed)

        Q_PROPERTY(QString statusText
            READ status_text
            NOTIFY stats_changed)

        Q_PROPERTY(int selectedFolderCount
            READ selected_folder_count
            NOTIFY selection_changed)

        Q_PROPERTY(int selectedFileCount
            READ selected_file_count
            NOTIFY selection_changed)

public:
    static FileSystemModel& instance();

private:
    explicit FileSystemModel(QObject* parent = nullptr);

public:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

public:
    QString current_path() const;
    void set_current_path(const QString& path);

    int folder_count() const;
    int file_count() const;

    QString total_size_text() const;
    QString status_text() const;

    int selected_folder_count() const;
    int selected_file_count() const;

    Q_INVOKABLE QStringList selected_paths() const;
    Q_INVOKABLE void clear_selection();
    Q_INVOKABLE void set_checked(int row, bool checked);
    Q_INVOKABLE void toggle_checked(int row);

signals:
    void current_path_changed();
    void stats_changed();
    void selection_changed();

private:
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

private:
    void update_stats();
    QString format_size(uint64_t bytes) const;
    void update_selection_stats();

public:
    enum Roles {
        FileNameRole = Qt::UserRole + 1,
        FileTypeRole,
        FileSizeRole,
        FileDateRole,
        FileCheckedRole,
        FileFullPathRole
    };

private:
    QString current_path_;

    std::vector<FileInfo> files_;

    int folder_count_ = 0;
    int file_count_ = 0;

    uint64_t total_size_ = 0;

    QString status_text_ = tr("Done");

    QSet<QString> selected_paths_;

    int selected_folder_count_ = 0;
    int selected_file_count_ = 0;
};