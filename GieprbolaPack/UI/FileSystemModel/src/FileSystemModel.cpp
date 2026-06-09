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
    watch_current_path();

    connect(&watcher_, &QFileSystemWatcher::directoryChanged, this, [this](const QString&) {
            refresh();
        }
    );
}

void FileSystemModel::refresh()
{
    const DirectoryLoadResult result = directory_loader_.load(current_path_);

    if (!result.success) {
        status_text_ = result.error_text;
        emit stats_changed();
        return;
    }

    beginResetModel();

    files_ = result.files;

    update_stats();
    update_selection_stats();

    endResetModel();

    watch_current_path();
}

void FileSystemModel::go_parent()
{
    std::filesystem::path path(current_path_.toStdWString());

    if (path.has_parent_path()) {
        set_current_path(QString::fromStdWString(path.parent_path().wstring()));
    }
}

QString FileSystemModel::select_folder(const QString& current_path)
{
    return QFileDialog::getExistingDirectory(
        nullptr,
        tr("Select folder"),
        current_path
    );
}

void FileSystemModel::watch_current_path()
{
    const QStringList watched = watcher_.directories();

    if (!watched.empty()) {
        watcher_.removePaths(watched);
    }

    if (std::filesystem::is_directory(current_path_.toStdWString())) {
        watcher_.addPath(current_path_);
    }
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
    case FileIconRole: {
        return icon_path(file);
    }

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

        return format_size(file.size);
    }

    case FileDateRole: {
        const auto sctp =
            std::chrono::time_point_cast<
            std::chrono::system_clock::duration>(file.last_write_time - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now());
        const std::time_t cftime = std::chrono::system_clock::to_time_t(sctp);

        return QDateTime::fromSecsSinceEpoch(cftime).toString("dd.MM.yyyy HH:mm");
    }

    case FileCheckedRole: {
        const QString path = QString::fromStdWString(file.full_path.wstring());
        return selection_.is_selected(path);
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

    selection_.set_selected(path, checked);

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
        { FileIconRole, "fileIcon" },
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

    const DirectoryLoadResult result = directory_loader_.load(path);

    if (!result.success) {
        status_text_ = result.error_text;
        emit stats_changed();
        return;
    }

    beginResetModel();

    selection_.clear();

    files_ = result.files;
    current_path_ = path;

    update_stats();
    update_selection_stats();

    endResetModel();

    watch_current_path();

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
    return selection_.selected_paths();
}

void FileSystemModel::clear_selection()
{
    if (selection_.empty()) {
        return;
    }

    selection_.clear();

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

QString FileSystemModel::selected_size_text() const
{
    return format_size(selected_size_);
}

void FileSystemModel::update_selection_stats()
{
    selected_folder_count_ = 0;
    selected_file_count_ = 0;
    selected_size_ = 0;

    std::unordered_set<std::filesystem::path> counted_files;

    for (const auto& file : files_) {
        const QString path = QString::fromStdWString(file.full_path.wstring());

        if (!selection_.is_selected(path)) {
            continue;
        }

        if (file.is_directory) {
            ++selected_folder_count_;

            try {
                for (const auto& entry : std::filesystem::recursive_directory_iterator(file.full_path)) {
                    if (!entry.is_regular_file()) {
                        continue;
                    }

                    if (counted_files.insert(entry.path()).second) {
                        ++selected_file_count_;
                        selected_size_ += entry.file_size();
                    }
                }
            }
            catch (...) {
            }
        }
        else {
            if (counted_files.insert(file.full_path).second) {
                ++selected_file_count_;
                selected_size_ += file.size;
            }
        }
    }

    emit selection_changed();
}

void FileSystemModel::create_archive(const QString& archive_path, int archive_type, int compression_mode)
{
    qDebug() << "create_archive:";
    qDebug() << "archive_path =" << archive_path;
    qDebug() << "archive_type =" << archive_type;
    qDebug() << "compression_mode =" << compression_mode;
    qDebug() << "selected =" << selected_paths();

    const QStringList selected = selected_paths();

    if (selected.empty()) {
        status_text_ = tr("No files selected");
        emit stats_changed();
        return;
    }

    std::vector<ArchiveFile> files;
    files.reserve(selected.size());

    try {
        for (const QString& path : selected) {
            const std::filesystem::path fs_path = path.toStdWString();

            qDebug() << "selected path =" << path;

            if (std::filesystem::is_regular_file(fs_path)) {
                files.emplace_back(fs_path, fs_path.filename().generic_string());
                continue;
            }

            if (std::filesystem::is_directory(fs_path)) {
                for (const auto& entry : std::filesystem::recursive_directory_iterator(fs_path)) {
                    if (!entry.is_regular_file()) {
                        continue;
                    }

                    const std::filesystem::path relative_path =
                        fs_path.filename() / std::filesystem::relative(entry.path(), fs_path);

                    files.emplace_back(entry.path(), relative_path.generic_string());
                }
            }
        }

        qDebug() << "files count =" << files.size();

        for (const auto& file : files) {
            qDebug() << "file =" << QString::fromStdWString(file.source_path)
                << "archive name =" << QString::fromStdString(file.archive_name);
        }

        if (files.empty()) {
            status_text_ = tr("No files or folders selected");
            emit stats_changed();
            return;
        }

        const ArchiveType type = static_cast<ArchiveType>(archive_type);
        const CompressionMode mode = static_cast<CompressionMode>(compression_mode);

        auto writer = ArchiveFactory::create_writer(type);

        if (!writer) {
            throw std::runtime_error("Unsupported archive type");
        }

        std::filesystem::path fs_archive_path = archive_path.toStdWString();
        const std::filesystem::path current_fs_path = current_path_.toStdWString();

        if (fs_archive_path.is_relative()) {
            fs_archive_path = current_fs_path / fs_archive_path;
        }

        qDebug() << "final archive path =" << QString::fromStdWString(fs_archive_path.wstring());

        writer->create(fs_archive_path, files, mode);

        status_text_ = tr("Archive created");
        clear_selection();
        refresh();
    }
    catch (const std::exception& error) {
        qDebug() << "create_archive error =" << error.what();
        status_text_ = QString::fromUtf8(error.what());
    }

    emit stats_changed();
}

void FileSystemModel::delete_selected()
{
    const QStringList selected = selected_paths();

    if (selected.empty()) {
        status_text_ = tr("No files selected");
        emit stats_changed();
        return;
    }

    int deleted_count = 0;

    try {
        for (const QString& path : selected) {
            const std::filesystem::path fs_path = path.toStdWString();

            if (!std::filesystem::exists(fs_path)) {
                continue;
            }

            if (std::filesystem::is_directory(fs_path)) {
                std::filesystem::remove_all(fs_path);
            }
            else {
                std::filesystem::remove(fs_path);
            }

            ++deleted_count;
        }

        clear_selection();
        refresh();

        status_text_ = tr("Deleted: %1").arg(deleted_count);
    }
    catch (const std::exception& error) {
        status_text_ = QString::fromUtf8(error.what());
    }

    emit stats_changed();
}

QString FileSystemModel::icon_path(const FileInfo& file) const
{
    if (file.is_directory) {
        return "qrc:/qt/qml/gieprbolapack/Resources/Extensions/default_folder.svg";
    }

    static const QHash<QString, QString> icons{
        {".c",    "qrc:/qt/qml/gieprbolapack/Resources/Extensions/file_type_c.svg"},
        {".cpp",  "qrc:/qt/qml/gieprbolapack/Resources/Extensions/file_type_cpp.svg"},
        {".h",    "qrc:/qt/qml/gieprbolapack/Resources/Extensions/file_type_cheader.svg"},
        {".hpp",  "qrc:/qt/qml/gieprbolapack/Resources/Extensions/file_type_cppheader.svg"},

        {".exe",  "qrc:/qt/qml/gieprbolapack/Resources/Extensions/exe.svg"},
        {".dll",  "qrc:/qt/qml/gieprbolapack/Resources/Extensions/dll.svg"},

        {".zip",  "qrc:/qt/qml/gieprbolapack/Resources/Extensions/file_type_zip.svg"},
        {".rar",  "qrc:/qt/qml/gieprbolapack/Resources/Extensions/file_type_zip.svg"},
        {".7z",   "qrc:/qt/qml/gieprbolapack/Resources/Extensions/file_type_zip.svg"},
        {".tar",  "qrc:/qt/qml/gieprbolapack/Resources/Extensions/file_type_zip.svg"},
        {".gz",   "qrc:/qt/qml/gieprbolapack/Resources/Extensions/file_type_zip.svg"},

        {".png",  "qrc:/qt/qml/gieprbolapack/Resources/Extensions/file_type_image.svg"},
        {".jpg",  "qrc:/qt/qml/gieprbolapack/Resources/Extensions/file_type_image.svg"},
        {".jpeg", "qrc:/qt/qml/gieprbolapack/Resources/Extensions/file_type_image.svg"},
        {".bmp",  "qrc:/qt/qml/gieprbolapack/Resources/Extensions/file_type_image.svg"},
        {".gif",  "qrc:/qt/qml/gieprbolapack/Resources/Extensions/file_type_image.svg"},
        {".webp", "qrc:/qt/qml/gieprbolapack/Resources/Extensions/file_type_image.svg"},

        {".pdf",  "qrc:/qt/qml/gieprbolapack/Resources/Extensions/file_type_pdf.svg"},
        {".json", "qrc:/qt/qml/gieprbolapack/Resources/Extensions/file_type_json.svg"},
        {".txt", "qrc:/qt/qml/gieprbolapack/Resources/Extensions/file_type_text.svg"},
        {".py", "qrc:/qt/qml/gieprbolapack/Resources/Extensions/file_type_python.svg"},
        {".java", "qrc:/qt/qml/gieprbolapack/Resources/Extensions/file_type_java.svg"},
        {".docx", "qrc:/qt/qml/gieprbolapack/Resources/Extensions/file_type_word.svg"},
        {".pptx", "qrc:/qt/qml/gieprbolapack/Resources/Extensions/file_type_powerpoint.svg"},
        {".xlsx", "qrc:/qt/qml/gieprbolapack/Resources/Extensions/file_type_excel.svg"},

        {".hex", "qrc:/qt/qml/gieprbolapack/Resources/Extensions/file_type_binary.svg"},
        {".bin", "qrc:/qt/qml/gieprbolapack/Resources/Extensions/file_type_binary.svg"},
    };

    const QString extension = QString::fromStdWString(file.full_path.extension().wstring()).toLower();

    return icons.value(extension, "qrc:/qt/qml/gieprbolapack/Resources/Extensions/default_file.svg");
}