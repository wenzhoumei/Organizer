#include <sqlite3.h>
#include <string>
#include <iostream>

struct Action {
    std::string action;
    std::string data;

    void print() const {
	std::cout << "action: " << action << std::endl;
	std::cout << "data: " << data << std::endl;
    }
};

enum { DB_BIND_ERROR, DB_EXECUTE_ERROR, DB_OK, DB_NO_ROWS, DB_RESET_ERROR };

struct Extension {
    std::string extension;
    std::string instruction;
    bool isFile;

    void print() const {
	std::cout << "extension: " << extension << std::endl;
	std::cout << "instruction: " << instruction << std::endl;
	std::cout << "isFile: " << isFile << std::endl;
    }
};

class Database {
    public:
	Database(const std::string& filename) {
	    // Open the database
	    int result = sqlite3_open(filename.c_str(), &db_);
	    if (result != SQLITE_OK) {
		// Handle the error
		fprintf(stderr, "Error opening database: %s\n", sqlite3_errmsg(db_));
		sqlite3_close(db_);
		return;
	    }

	    // Prepare the insert action statement
	    result = sqlite3_prepare_v2(db_, "INSERT INTO Actions (Action, Data) VALUES (?, ?)", -1, &insert_action_stmt_, 0);
	    if (result != SQLITE_OK) {
		// Handle the error
		fprintf(stderr, "Error preparing insert action statement: %s\n", sqlite3_errmsg(db_));
		sqlite3_finalize(insert_action_stmt_);
		sqlite3_close(db_);
		return;
	    }

	    // Prepare the update action statement
	    result = sqlite3_prepare_v2(db_, "UPDATE Actions SET Data = ? WHERE Action = ?", -1, &update_action_stmt_, 0);
	    if (result != SQLITE_OK) {
		// Handle the error
		fprintf(stderr, "Error preparing update action statement: %s\n", sqlite3_errmsg(db_));
		sqlite3_finalize(update_action_stmt_);
		sqlite3_close(db_);
		return;
	    }

	    // Prepare the remove action statement
	    result = sqlite3_prepare_v2(db_, "DELETE FROM Actions WHERE Action = ?", -1, &remove_action_stmt_, 0);
	    if (result != SQLITE_OK) {
		// Handle the error
		fprintf(stderr, "Error preparing remove action statement: %s\n", sqlite3_errmsg(db_));
		sqlite3_finalize(remove_action_stmt_);
		sqlite3_close(db_);
		return;
	    }

	    // Prepare the get action statement
	    result = sqlite3_prepare_v2(db_, "SELECT Action, Data FROM Actions WHERE Action = ?", -1, &get_action_stmt_, nullptr);
	    if (result != SQLITE_OK) {
		// Handle the error
		fprintf(stderr, "Error preparing get action statement: %s\n", sqlite3_errmsg(db_));
		sqlite3_finalize(get_action_stmt_);
		sqlite3_close(db_);
		return;
	    }

	    // Prepare the insert extension statement
	    result = sqlite3_prepare_v2(db_, "INSERT INTO Extensions (Extension, Instruction, IsFile) VALUES (?, ?, ?)", -1, &insert_extension_stmt_, 0);
	    if (result != SQLITE_OK) {
		// Handle the error
		fprintf(stderr, "Error preparing insert extension statement: %s\n", sqlite3_errmsg(db_));
		sqlite3_finalize(insert_extension_stmt_);
		sqlite3_close(db_);
		return;
	    }

	    // Prepare the update extension statement
	    result = sqlite3_prepare_v2(db_, "UPDATE Extensions SET Instruction = ?, IsFile = ? WHERE Extension = ?", -1, &update_extension_stmt_, 0);
	    if (result != SQLITE_OK) {
		// Handle the error
		fprintf(stderr, "Error preparing update extension statement: %s\n", sqlite3_errmsg(db_));
		sqlite3_finalize(update_extension_stmt_);
		sqlite3_close(db_);
		return;
	    }

	    // Prepare the remove extension statement
	    result = sqlite3_prepare_v2(db_, "DELETE FROM Extensions WHERE Extension = ?", -1, &remove_extension_stmt_, 0);
	    if (result != SQLITE_OK) {
		// Handle the error
		fprintf(stderr, "Error preparing remove extension statement: %s\n", sqlite3_errmsg(db_));
		sqlite3_finalize(remove_extension_stmt_);
		sqlite3_close(db_);
		return;
	    }

	    // Prepare the get extension statement
	    result = sqlite3_prepare_v2(db_, "SELECT Instruction, IsFile FROM Extensions WHERE Extension = ?", -1, &get_extension_stmt_, nullptr);
	    if (result != SQLITE_OK) {
		// Handle the error
		fprintf(stderr, "Error preparing get extension statement: %s\n", sqlite3_errmsg(db_));
		sqlite3_finalize(get_action_stmt_);
		sqlite3_close(db_);
		return;
	    }
	}

	~Database() {
	    // Finalize the statements
	    sqlite3_finalize(insert_action_stmt_);
	    sqlite3_finalize(update_action_stmt_);
	    sqlite3_finalize(remove_action_stmt_);
	    sqlite3_finalize(get_action_stmt_);
	    sqlite3_finalize(insert_extension_stmt_);
	    sqlite3_finalize(update_extension_stmt_);
	    sqlite3_finalize(remove_extension_stmt_);
	    sqlite3_finalize(get_extension_stmt_);

	    // Close the database
	    sqlite3_close(db_);
	}

	// Insert an action
	int insertAction(const std::string& action, const std::string& data) {
	    // Bind the values
	    int result = sqlite3_bind_text(insert_action_stmt_, 1, action.c_str(), -1, SQLITE_TRANSIENT);
	    if (result != SQLITE_OK) {
		// Handle the error
		fprintf(stderr, "Error binding action: %s\n", sqlite3_errmsg(db_));
		return DB_BIND_ERROR;
	    }
	    result = sqlite3_bind_text(insert_action_stmt_, 2, data.c_str(), -1, SQLITE_TRANSIENT);
	    if (result != SQLITE_OK) {
		// Handle the error
		fprintf(stderr, "Error binding data: %s\n", sqlite3_errmsg(db_));
		return DB_BIND_ERROR;
	    }

	    // Execute the statement
	    result = executeStatement(insert_action_stmt_);
	    if (result != SQLITE_DONE) {
		// Handle the error
		fprintf(stderr, "Error inserting action: %s\n", sqlite3_errmsg(db_));
		return DB_EXECUTE_ERROR;
	    }

	    // Return success
	    return DB_OK;
	}

	// Update an action
	int updateAction(const std::string& action, const std::string& data) {
	    // Bind the values
	    int result = sqlite3_bind_text(update_action_stmt_, 1, data.c_str(), -1, SQLITE_TRANSIENT);
	    if (result != SQLITE_OK) {
		// Handle the error
		fprintf(stderr, "Error binding name: %s\n", sqlite3_errmsg(db_));
		return DB_BIND_ERROR;
	    }
	    result = sqlite3_bind_text(update_action_stmt_, 2, action.c_str(), -1, SQLITE_TRANSIENT);

	    if (result != SQLITE_OK) {
		// Handle the error
		fprintf(stderr, "Error binding action: %s\n", sqlite3_errmsg(db_));
		return DB_BIND_ERROR;
	    }

	    // Execute the statement
	    result = executeStatement(update_action_stmt_);
	    if (result != SQLITE_DONE) {
		// Handle the error
		fprintf(stderr, "Error updating action: %s\n", sqlite3_errmsg(db_));
		return DB_EXECUTE_ERROR;
	    }

	    // Return success
	    return DB_OK;
	}

	// Remove an action
	int removeAction(const std::string& action) {
	    // Bind the values
	    int result = sqlite3_bind_text(remove_action_stmt_, 1, action.c_str(), -1, SQLITE_TRANSIENT);
	    if (result != SQLITE_OK) {
		// Handle the error
		fprintf(stderr, "Error binding action: %s\n", sqlite3_errmsg(db_));
		return DB_BIND_ERROR;
	    }

	    // Execute the statement
	    result = executeStatement(remove_action_stmt_);
	    if (result != SQLITE_DONE) {
		// Handle the error
		fprintf(stderr, "Error deleting action: %s\n", sqlite3_errmsg(db_));
		return DB_EXECUTE_ERROR;
	    }

	    // Return success
	    return DB_OK;
	}

	// Get action
	int getAction(const std::string& action, Action& out) {
	    // Bind the parameter value to the prepared statement
	    int result = sqlite3_bind_text(get_action_stmt_, 1, action.c_str(), -1, SQLITE_TRANSIENT);
	    if (result != SQLITE_OK) {
		// Handle the error
		fprintf(stderr, "Error binding parameter: %s\n", sqlite3_errmsg(db_));
		return DB_BIND_ERROR;
	    }

	    // Execute the prepared statement
	    result = sqlite3_step(get_action_stmt_);
	    if (result == SQLITE_ERROR) {
		// Handle the error
		fprintf(stderr, "Error executing statement: %s\n", sqlite3_errmsg(db_));
		return DB_EXECUTE_ERROR;
	    } else if (result == SQLITE_DONE) {
		// No rows were returned
		return DB_NO_ROWS;
	    }

	    // Retrieve the values from the result set
	    out.action = std::string(reinterpret_cast<const char*>(sqlite3_column_text(get_action_stmt_, 0)));
	    out.data = std::string(reinterpret_cast<const char*>(sqlite3_column_text(get_action_stmt_, 1)));

	    // Reset the prepared statement
	    result = sqlite3_reset(get_action_stmt_);
	    if (result != SQLITE_OK) {
		// Handle the error
		fprintf(stderr, "Error resetting statement: %s\n", sqlite3_errmsg(db_));
		return DB_RESET_ERROR;
	    }

	    return DB_OK;
	}
	// Insert an extension
	int insertExtension(const std::string& extension, const std::string& instruction, bool is_file) {
	    // Bind the values
	    int result = sqlite3_bind_text(insert_extension_stmt_, 1, extension.c_str(), -1, SQLITE_TRANSIENT);
	    if (result != SQLITE_OK) {
		// Handle the error
		fprintf(stderr, "Error binding extension: %s\n", sqlite3_errmsg(db_));
		return DB_BIND_ERROR;
	    }
	    result = sqlite3_bind_text(insert_extension_stmt_, 2, instruction.c_str(), -1, SQLITE_TRANSIENT);
	    if (result != SQLITE_OK) {
		// Handle the error
		fprintf(stderr, "Error binding instruction: %s\n", sqlite3_errmsg(db_));
		return DB_BIND_ERROR;
	    }
	    result = sqlite3_bind_int(insert_extension_stmt_, 3, is_file ? 1 : 0);
	    if (result != SQLITE_OK) {
		// Handle the error
		fprintf(stderr, "Error binding is_file: %s\n", sqlite3_errmsg(db_));
		return DB_BIND_ERROR;
	    }

	    // Execute the statement
	    result = executeStatement(insert_extension_stmt_);
	    if (result != SQLITE_DONE) {
		// Handle the error
		fprintf(stderr, "Error inserting extension: %s\n", sqlite3_errmsg(db_));
		return DB_EXECUTE_ERROR;
	    }

	    // Return success
	    return DB_OK;
	}

	// Update an extension
	int updateExtension(const std::string& extension, const std::string& instruction, bool is_file) {
	    // Bind the values
	    int result = sqlite3_bind_text(update_extension_stmt_, 1, instruction.c_str(), -1, SQLITE_TRANSIENT);
	    if (result != SQLITE_OK) {
		// Handle the error
		fprintf(stderr, "Error binding instruction: %s\n", sqlite3_errmsg(db_));
		return DB_BIND_ERROR;
	    }
	    result = sqlite3_bind_int(update_extension_stmt_, 2, is_file ? 1 : 0);
	    if (result != SQLITE_OK) {
		// Handle the error
		fprintf(stderr, "Error binding is_file: %s\n", sqlite3_errmsg(db_));
		return DB_BIND_ERROR;
	    }
	    result = sqlite3_bind_text(update_extension_stmt_, 3, extension.c_str(), -1, SQLITE_TRANSIENT);
	    if (result != SQLITE_OK) {
		// Handle the error
		fprintf(stderr, "Error binding extension: %s\n", sqlite3_errmsg(db_));
		return DB_BIND_ERROR;
	    }

	    // Execute the statement
	    result = executeStatement(update_extension_stmt_);
	    if (result != SQLITE_DONE) {
		// Handle the error
		fprintf(stderr, "Error updating extension: %s\n", sqlite3_errmsg(db_));
		return DB_EXECUTE_ERROR;
	    }

	    // Return success
	    return DB_OK;
	}

	// Remove an extension
	int removeExtension(const std::string& extension) {
	    // Bind the values
	    int result = sqlite3_bind_text(remove_extension_stmt_, 1, extension.c_str(), -1, SQLITE_TRANSIENT);
	    if (result != SQLITE_OK) {
		// Handle the error
		fprintf(stderr, "Error binding extension: %s\n", sqlite3_errmsg(db_));
		return DB_BIND_ERROR;
	    }

	    // Execute the statement
	    result = executeStatement(remove_extension_stmt_);
	    if (result != SQLITE_DONE) {
		// Handle the error
		fprintf(stderr, "Error deleting extension: %s\n", sqlite3_errmsg(db_));
		return DB_BIND_ERROR;
	    }

	    // Return success
	    return SQLITE_OK;
	}

	int getExtension(const std::string& extension, Extension& out) {
	    // Bind the parameter value to the prepared statement
	    int result = sqlite3_bind_text(get_extension_stmt_, 1, extension.c_str(), -1, SQLITE_TRANSIENT);
	    if (result != SQLITE_OK) {
		// Handle the error
		fprintf(stderr, "Error binding parameter: %s\n", sqlite3_errmsg(db_));
		return DB_BIND_ERROR;
	    }

	    // Execute the prepared statement
	    result = sqlite3_step(get_extension_stmt_);
	    if (result == SQLITE_ERROR) {
		// Handle the error
		fprintf(stderr, "Error executing statement: %s\n", sqlite3_errmsg(db_));
		return DB_BIND_ERROR;
	    } else if (result == SQLITE_DONE) {
		// No rows were returned
		return DB_NO_ROWS;
	    }

	    // Retrieve the values from the result set
	    out.extension = std::string(reinterpret_cast<const char*>(sqlite3_column_text(get_extension_stmt_, 0)));
	    out.instruction = std::string(reinterpret_cast<const char*>(sqlite3_column_text(get_extension_stmt_, 0)));
	    out.isFile = (sqlite3_column_int(get_extension_stmt_, 2) != 1);

	    result = sqlite3_reset(get_extension_stmt_);
	    if (result != SQLITE_OK) {
		// Handle the error
		fprintf(stderr, "Error resetting statement: %s\n", sqlite3_errmsg(db_));
		return DB_RESET_ERROR;
	    }

	    return DB_OK;
	}
    private:
	// Execute a prepared statement and reset it
	int executeStatement(sqlite3_stmt* stmt) {
	    int result = sqlite3_step(stmt);
	    if (result != SQLITE_DONE && result != SQLITE_ROW) {
		// Handle the error
		fprintf(stderr, "Error executing statement: %s\n", sqlite3_errmsg(db_));
		return result;
	    }
	    sqlite3_reset(stmt);
	    return result;
	}

	sqlite3* db_;
	sqlite3_stmt* insert_action_stmt_;
	sqlite3_stmt* update_action_stmt_;
	sqlite3_stmt* remove_action_stmt_;
	sqlite3_stmt* get_action_stmt_;
	sqlite3_stmt* insert_extension_stmt_;
	sqlite3_stmt* update_extension_stmt_;
	sqlite3_stmt* remove_extension_stmt_;
	sqlite3_stmt* get_extension_stmt_;
};
