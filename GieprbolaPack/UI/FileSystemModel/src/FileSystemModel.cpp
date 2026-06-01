#include "../include/FileSystemModel.hpp"


FileSystemModel& FileSystemModel::instance()
{
    static FileSystemModel model;
    return model;
}

FileSystemModel::FileSystemModel(QObject* parent)
    : QAbstractListModel(parent)
{
    const auto path = FileScanner::instance().get_root_path();
    current_path_ = QString::fromStdWString(path.wstring());
    files_ = FileScanner::instance().scan(path);
    update_stats();
}

int FileSystemModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return static_cast<int>(files_.size());
}

QVariant FileSystemModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return {};
    }

    const FileInfo& file = files_[index.row()];

    switch (role)
    {
    case FileNameRole: {
        return QString::fromStdWString(
            file.relative_path.wstring()
        );
    }

    case FileTypeRole: {
        if (file.is_directory) {
            return tr("Folder");
        }

        return QString::fromStdWString(
            file.full_path.extension().wstring()
        );
    }

    case FileSizeRole: {
        if (file.is_directory) {
            return "";
        }

        return QString::number(file.size);
    }

    case FileDateRole: {
        const auto sctp =
            std::chrono::time_point_cast<
            std::chrono::system_clock::duration
            >(
                file.last_write_time
                - std::filesystem::file_time_type::clock::now()
                + std::chrono::system_clock::now()
            );

        const std::time_t cftime = std::chrono::system_clock::to_time_t(sctp);

        return QDateTime::fromSecsSinceEpoch(cftime).toString("dd.MM.yyyy HH:mm");
    }

    case FileCheckedRole: {
        const QString path = QString::fromStdWString(file.full_path.wstring());
        return selected_paths_.contains(path);
    }

    case FileFullPathRole: {
        return QString::fromStdWString(file.full_path.wstring());
    }

    default:
        return {};
    }
}

bool FileSystemModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid()) {
        return false;
    }

    if (role != FileCheckedRole) {
        return false;
    }

    const FileInfo& file = files_[index.row()];
    const QString path = QString::fromStdWString(file.full_path.wstring());

    const bool checked = value.toBool();

    if (checked) {
        selected_paths_.insert(path);
    }
    else {
        selected_paths_.remove(path);
    }

    update_selection_stats();

    emit dataChanged(index, index, { FileCheckedRole });

    return true;
}

Qt::ItemFlags FileSystemModel::flags(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    return Qt::ItemIsEnabled |
        Qt::ItemIsSelectable |
        Qt::ItemIsUserCheckable;
}

QHash<int, QByteArray> FileSystemModel::roleNames() const
{
    return {
        { FileNameRole, "fileName" },
        { FileTypeRole, "fileType" },
        { FileSizeRole, "fileSize" },
        { FileDateRole, "fileDate" },
        { FileCheckedRole, "fileChecked" },
        { FileFullPathRole, "fileFullPath" }
    };
}

QString FileSystemModel::current_path() const
{
    return current_path_;
}

void FileSystemModel::set_current_path(const QString& path)
{
    if (current_path_ == path) {
        return;
    }

    const auto fs_path = std::filesystem::path(path.toStdWString());

    if (!std::filesystem::exists(fs_path)) {
        status_text_ = tr("Path not found");
        emit stats_changed();
        return;
    }

    if (!std::filesystem::is_directory(fs_path)) {
        status_text_ = tr("It's not a folder");
        emit stats_changed();
        return;
    }

    beginResetModel();

    selected_paths_.clear();

    update_selection_stats();

    files_ = FileScanner::instance().scan(fs_path);
    current_path_ = path;
    update_stats();

    endResetModel();

    emit current_path_changed();
}

int FileSystemModel::folder_count() const
{
    return folder_count_;
}

int FileSystemModel::file_count() const
{
    return file_count_;
}

QString FileSystemModel::total_size_text() const
{
    return format_size(total_size_);
}

QString FileSystemModel::status_text() const
{
    return status_text_;
}

void FileSystemModel::update_stats()
{
    folder_count_ = 0;
    file_count_ = 0;
    total_size_ = 0;

    for (const auto& file : files_) {
        if (file.is_directory) {
            ++folder_count_;
        }
        else {
            ++file_count_;
            total_size_ += file.size;
        }
    }

    status_text_ = tr("Done");

    emit stats_changed();
}

QString FileSystemModel::format_size(uint64_t bytes) const
{
    constexpr double KB = 1024.0;
    constexpr double MB = KB * 1024.0;
    constexpr double GB = MB * 1024.0;

    if (bytes >= GB) {
        return QString::number(bytes / GB, 'f', 2) + " GB";
    }

    if (bytes >= MB) {
        return QString::number(bytes / MB, 'f', 2) + " MB";
    }

    if (bytes >= KB) {
        return QString::number(bytes / KB, 'f', 2) + " KB";
    }

    return QString::number(bytes) + " B";
}

QStringList FileSystemModel::selected_paths() const
{
    return QStringList(selected_paths_.begin(), selected_paths_.end());
}

void FileSystemModel::clear_selection()
{
    if (selected_paths_.isEmpty()) {
        return;
    }

    selected_paths_.clear();

    update_selection_stats();

    if (!files_.empty()) {
        emit dataChanged(
            index(0, 0),
            index(static_cast<int>(files_.size()) - 1, 0),
            { FileCheckedRole }
        );
    }
}

void FileSystemModel::set_checked(int row, bool checked)
{
    if (row < 0 || row >= static_cast<int>(files_.size())) {
        return;
    }

    setData(index(row, 0), checked, FileCheckedRole);
}

void FileSystemModel::toggle_checked(int row)
{
    if (row < 0 || row >= static_cast<int>(files_.size())) {
        return;
    }

    const QModelIndex model_index = index(row, 0);
    const bool checked = data(model_index, FileCheckedRole).toBool();

    setData(model_index, !checked, FileCheckedRole);
}

int FileSystemModel::selected_folder_count() const
{
    return selected_folder_count_;
}

int FileSystemModel::selected_file_count() const
{
    return selected_file_count_;
}

void FileSystemModel::update_selection_stats()
{
    selected_folder_count_ = 0;
    selected_file_count_ = 0;

    for (const auto& file : files_) {
        const QString path = QString::fromStdWString(file.full_path.wstring());

        if (!selected_paths_.contains(path)) {
            continue;
        }

        if (file.is_directory) {
            ++selected_folder_count_;
        }
        else {
            ++selected_file_count_;
        }
    }

    emit selection_changed();
}