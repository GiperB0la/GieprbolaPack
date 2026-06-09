#pragma once
#include <filesystem>
#include <vector>

#include <QAbstractListModel>
#include <QDateTime>
#include <QSet>
#include <QFileSystemWatcher>
#include <QFileDialog>

#include "../../../Core/FileScanner/include/FileScanner.hpp"
#include "../../../Core/ArchiveEditor/Archive/include/ArchiveFactory.hpp"

#include "../../Services/FileSelection/include/FileSelection.hpp"
#include "../../Services/FileDirectoryLoader/include/FileDirectoryLoader.hpp"

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

        Q_PROPERTY(QString selectedSizeText
            READ selected_size_text
            NOTIFY selection_changed)

public:
    static FileSystemModel& instance();

private:
    explicit FileSystemModel(QObject* parent = nullptr);

public:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
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
    QString selected_size_text() const;

    Q_INVOKABLE void refresh();
    Q_INVOKABLE void go_parent();
    Q_INVOKABLE QStringList selected_paths() const;
    Q_INVOKABLE QString select_folder(const QString& current_path);

    Q_INVOKABLE void clear_selection();
    Q_INVOKABLE void set_checked(int row, bool checked);
    Q_INVOKABLE void toggle_checked(int row);
    Q_INVOKABLE void create_archive(const QString& archive_path, int archive_type, int compression_mode);
    Q_INVOKABLE void delete_selected();

signals:
    void current_path_changed();
    void stats_changed();
    void selection_changed();

private:
    void watch_current_path();
    void update_stats();
    void update_selection_stats();

    QString format_size(uint64_t bytes) const;
    QString icon_path(const FileInfo& file) const;

public:
    enum Roles {
        FileIconRole = Qt::UserRole + 1,
        FileNameRole,
        FileTypeRole,
        FileSizeRole,
        FileDateRole,
        FileCheckedRole,
        FileFullPathRole
    };

private:
    QFileSystemWatcher watcher_;

    QString current_path_;

    std::vector<FileInfo> files_;

    FileSelection selection_;
    FileDirectoryLoader directory_loader_;

    int folder_count_ = 0;
    int file_count_ = 0;
    uint64_t total_size_ = 0;

    QString status_text_ = tr("Done");

    int selected_folder_count_ = 0;
    int selected_file_count_ = 0;
    uint64_t selected_size_ = 0;
};