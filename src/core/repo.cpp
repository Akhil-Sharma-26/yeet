#include "../include/core/repo.hpp"
#include "core/index.h"
#include "core/commit.h"
#include "core/branch.h"
#include "utils/filesystem.h"
#include "utils/hash.h"
#include "utils/logger.h"

#include <fstream>
#include <iostream>
#include <ctime>

namespace yeet {

Repository::Repository() : index_(std::make_unique<Index>()),
                          current_branch_(std::make_unique<Branch>()) {
}

Repository::~Repository() = default;

bool Repository::init(const std::filesystem::path& path) {
    try {
        // Check if directory exists
        if (!std::filesystem::exists(path)) {
            if (!std::filesystem::create_directories(path)) {
                Logger::error("Failed to create directory: " + path.string());
                return false;
            }
        }
        
        // Check if already a repository
        if (is_repository(path)) {
            Logger::error("Repository already exists at: " + path.string());
            return false;
        }
        
        repo_path_ = std::filesystem::absolute(path);
        
        // Create .yeet directory structure
        std::filesystem::create_directory(get_yeet_dir());
        std::filesystem::create_directory(get_objects_dir());
        std::filesystem::create_directory(get_refs_dir());
        std::filesystem::create_directory(get_refs_dir() / "heads");
        
        // Create initial HEAD file pointing to main branch
        std::ofstream head_file(get_yeet_dir() / "HEAD");
        head_file << "ref: refs/heads/main";
        head_file.close();
        
        // Create config file
        std::ofstream config_file(get_yeet_dir() / "config");
        config_file << "[core]\n";
        config_file << "    repositoryformatversion = 0\n";
        config_file << "    filemode = true\n";
        config_file << "    bare = false\n";
        config_file.close();
        
        // Create empty index file
        std::ofstream index_file(get_yeet_dir() / "index");
        index_file.close();
        
        // Initialize index
        index_->load(get_yeet_dir() / "index");
        
        // Set current branch to main
        current_branch_->set_name("main");
        
        Logger::info("Initialized empty YEET repository in " + get_yeet_dir().string());
        return true;
    } 
    catch (const std::exception& e) {
        Logger::error("Repository initialization failed: " + std::string(e.what()));
        return false;
    }
}

bool Repository::is_repository(const std::filesystem::path& path) const {
    try {
        return std::filesystem::exists(path / ".yeet") &&
               std::filesystem::exists(path / ".yeet" / "HEAD") &&
               std::filesystem::exists(path / ".yeet" / "objects") &&
               std::filesystem::exists(path / ".yeet" / "refs");
    }
    catch (const std::exception& e) {
        Logger::error("Error checking repository: " + std::string(e.what()));
        return false;
    }
}

bool Repository::add(const std::filesystem::path& file_path) {
    try {
        // Convert to absolute path if needed
        std::filesystem::path abs_path = file_path.is_absolute() ? 
                                         file_path : 
                                         std::filesystem::absolute(file_path);
        
        // Check if file exists
        if (!std::filesystem::exists(abs_path)) {
            Logger::error("File does not exist: " + abs_path.string());
            return false;
        }
        
        // Get file content
        std::string content = utils::read_file(abs_path);
        
        // Calculate hash for object
        std::string hash = utils::sha1(content);
        
        // Store object in objects directory
        std::string object_path = get_objects_dir().string() + "/" + 
                                 hash.substr(0, 2) + "/" + hash.substr(2);
        
        // Create directory if it doesn't exist
        std::filesystem::create_directories(
            get_objects_dir() / hash.substr(0, 2));
        
        // Write compressed content to object file
        utils::write_compressed_file(object_path, content);
        
        // Update index
        std::filesystem::path relative_path = 
            std::filesystem::relative(abs_path, repo_path_);
        
        index_->add_entry(relative_path.string(), hash);
        index_->write(get_yeet_dir() / "index");
        
        Logger::info("Added file: " + relative_path.string());
        return true;
    } 
    catch (const std::exception& e) {
        Logger::error("Error adding file: " + std::string(e.what()));
        return false;
    }
}

std::string Repository::commit(const std::string& message, 
                               const std::string& author,
                               const std::string& email) {
    try {
        // Create commit object
        Commit commit;
        commit.message = message;
        commit.author = author;
        commit.email = email;
        commit.timestamp = std::time(nullptr);
        
        // Get current branch and its current commit
        std::string branch_name = get_current_branch();
        std::string parent_commit;
        
        std::filesystem::path branch_path = get_refs_dir() / "heads" / branch_name;
        if (std::filesystem::exists(branch_path)) {
            parent_commit = utils::read_file(branch_path);
            commit.parent = parent_commit;
        }
        
        // Create tree from index
        std::string tree_hash = index_->create_tree(*this);
        commit.tree = tree_hash;
        
        // Serialize and store commit
        std::string commit_content = commit.serialize();
        std::string commit_hash = utils::sha1(commit_content);
        
        // Store commit object
        std::filesystem::create_directories(
            get_objects_dir() / commit_hash.substr(0, 2));
        
        utils::write_compressed_file(
            get_objects_dir() / commit_hash.substr(0, 2) / commit_hash.substr(2),
            commit_content
        );
        
        // Update branch reference
        utils::write_file(branch_path, commit_hash);
        
        Logger::info("Created commit " + commit_hash.substr(0, 7) + ": " + message);
        return commit_hash;
    } 
    catch (const std::exception& e) {
        Logger::error("Error creating commit: " + std::string(e.what()));
        return "";
    }
}

bool Repository::create_branch(const std::string& name) {
    try {
        // Get current commit
        std::string current_commit = utils::read_file(
            get_refs_dir() / "heads" / get_current_branch()
        );
        
        // Create new branch file pointing to same commit
        utils::write_file(get_refs_dir() / "heads" / name, current_commit);
        
        Logger::info("Created branch: " + name);
        return true;
    } 
    catch (const std::exception& e) {
        Logger::error("Error creating branch: " + std::string(e.what()));
        return false;
    }
}

bool Repository::switch_branch(const std::string& name) {
    try {
        // Check if branch exists
        std::filesystem::path branch_path = get_refs_dir() / "heads" / name;
        if (!std::filesystem::exists(branch_path)) {
            Logger::error("Branch does not exist: " + name);
            return false;
        }
        
        // Update HEAD to point to new branch
        utils::write_file(get_yeet_dir() / "HEAD", "ref: refs/heads/" + name);
        
        // Update working directory
        // (This is simplified - a real implementation would handle changed files,
        // conflicts, etc.)
        
        current_branch_->set_name(name);
        
        Logger::info("Switched to branch: " + name);
        return true;
    } 
    catch (const std::exception& e) {
        Logger::error("Error switching branch: " + std::string(e.what()));
        return false;
    }
}

std::string Repository::get_current_branch() const {
    try {
        // Read HEAD file
        std::string head_content = utils::read_file(get_yeet_dir() / "HEAD");
        
        // Parse ref (format is "ref: refs/heads/branchname")
        if (head_content.substr(0, 5) == "ref: ") {
            std::string ref = head_content.substr(5);
            // Extract branch name from refs/heads/branchname
            return ref.substr(ref.find_last_of('/') + 1);
        }
        
        // If HEAD is a commit hash, we're in detached HEAD state
        return "HEAD";
    } 
    catch (const std::exception& e) {
        Logger::error("Error getting current branch: " + std::string(e.what()));
        return "unknown";
    }
}

std::vector<std::string> Repository::list_branches() const {
    std::vector<std::string> branches;
    
    try {
        // Iterate through refs/heads directory
        for (const auto& entry : std::filesystem::directory_iterator(get_refs_dir() / "heads")) {
            if (entry.is_regular_file()) {
                branches.push_back(entry.path().filename().string());
            }
        }
    } 
    catch (const std::exception& e) {
        Logger::error("Error listing branches: " + std::string(e.what()));
    }
    
    return branches;
}

bool Repository::is_modified(const std::filesystem::path& file_path) const {
    try {
        // Get index entry for file
        std::string relative_path = std::filesystem::relative(file_path, repo_path_).string();
        std::string stored_hash = index_->get_hash(relative_path);
        
        if (stored_hash.empty()) {
            // File not in index
            return false;
        }
        
        // Calculate current hash
        std::string content = utils::read_file(file_path);
        std::string current_hash = utils::sha1(content);
        
        // Compare hashes
        return current_hash != stored_hash;
    } 
    catch (const std::exception& e) {
        Logger::error("Error checking modifications: " + std::string(e.what()));
        return false;
    }
}

bool Repository::is_tracked(const std::filesystem::path& file_path) const {
    std::string relative_path = std::filesystem::relative(file_path, repo_path_).string();
    return !index_->get_hash(relative_path).empty();
}

std::filesystem::path Repository::get_yeet_dir() const {
    return repo_path_ / ".yeet";
}

std::filesystem::path Repository::get_objects_dir() const {
    return get_yeet_dir() / "objects";
}

std::filesystem::path Repository::get_refs_dir() const {
    return get_yeet_dir() / "refs";
}

} // namespace yeet