#ifndef SQLITE_H
#define SQLITE_H
/**
  \brief This file contains wrapper for sqlite3 object. Its handle busy error code.
  If one of the function returns SQLITE_BUSY error code it means that something wrong
  (db too overloaded with transactions) because in normal work it should never happens

  \author Yuriy Lisovyy
  */

#include "sqlite3.h"
#include <string>
#include <memory>
#include <functional>

namespace pocketbook
{
namespace utilities
{

class Statement;

class SqliteError {
public:
    SqliteError(const std::string& message):msg_(message)
    {
    }
    std::string GetMessage() { return msg_; }
private:
    std::string msg_;
};

/**
  \brief Wrapper on sqlite3 object. Most function is analog of corresponding sqlite3_* functions
  exept it handle SQLITE_BUSY error code
  */
class Sqlite
{
private:
    sqlite3 * dbHandle_;        // Underlying sqlite3* object
    void (*corrupt_handler_)(void);
    std::function<void()> corrupt_handler_ex_;
    // Non copiable
    Sqlite(const Sqlite&);
    Sqlite& operator=(const Sqlite&);

public:
    Sqlite():dbHandle_(NULL),corrupt_handler_(NULL)
    {
    }

    /**
      \brief open connection to database (equivalent to sqlite3_open function)
      */
    int open(const std::string& fileName);

    /**
      \brief open connection to database (equivalent to sqlite3_open function)
      */
    int open_no_create(const std::string& fileName);

    /**
      \brief close database connection (equivalent to sqlite3_close function)
      */
    int close(void);

    /**
      \brief execute query (equivalent to sqlite3_exec function with last 3 parameters set to NULL)
      */
    int exec(const std::string& query);

    /**
      \brief prepare statement for execution (equivalent to sqlite3_prepare_v2 function
      */
    Statement* prepare(const std::string& query);

    // Throw SqliteError exception on fail
    std::shared_ptr<Statement> prepare_v2(const std::string& query);

    std::string GetLastErrorMsg();

    /**
      \brief get underlying database handle
      */
    sqlite3* getHandle()
    {
        return dbHandle_;
    }

    long long LastInsertedRow();

    void SetCorruptHandler(void (*handler)(void)) { corrupt_handler_ex_ = handler; }
    void SetCorruptHandlerEx(std::function<void()> handler) { corrupt_handler_ex_ = handler;}

    ~Sqlite()
    {
        if (dbHandle_)
        {
            close();
        }
    }
};

/**
  \brief Wrapper on sqlite3_stmt object. Most function is analog of corresponding sqlite3_* functions
  exept it handle SQLITE_BUSY error code
  */
class Statement
{
    sqlite3_stmt* stmt_ = nullptr;    // Underlying statement
    std::function<void()> corrupt_handler_;

    // Create this object through Sqlite::prepare function
    Statement(const Statement&);

    // Non copiable
    Statement& operator=(const Statement&);
    Statement(sqlite3_stmt* stmt, std::function<void()> corrupt_handler):stmt_(stmt), corrupt_handler_(corrupt_handler){}

    Statement& bind_all_impl(int /*index*/) {
        return *this;
    }

    template<typename T, typename... Params> Statement& bind_all_impl(int index, T&& value, Params&&... parameters)
    {
        int err = bind(index, std::forward<T>(value));
        if (err) throw SqliteError("Failed to bind value");

        return bind_all_impl(index + 1, parameters...);
    }

public:
    friend class Sqlite;

    // new interface with exceptions

    template<typename... Params> Statement& bind_all(Params&&... parameters)
    {
        return bind_all_impl(1, parameters...);
    }

    void run();

    // Bind analugues to sqlite3_bind* functions
    int bind(int index, sqlite3_int64 value);

    int bind(int index, const std::string& value);

    int bind(int index, const char* value);

    int bind(int index, double value);

    int bind(int index, int value);

    int bind(int index, const char* value, int size);
    int bind_binary(int index, const char* value, int size);
    struct Binary {
        std::string data_;
        Binary(const std::string& data):data_(data){}
    };
    int bind(int index, const Binary &value);


    int bind_null(int index);

    // Analugues to sqlite3_colmn* function
    sqlite_int64 column_int64(int index);

    const char* column_text(int index);
    std::string column_binary(int index);

    double column_double(int index);

    int column_int(int index);

    bool is_null_columnt(int index);
    /// \brief Analogue to sqlite3_step function
    int step();

    /// \brief Analogue to sqlite3_reset function
    int reset();

    /// \brief Analogue to sqlite3_clear_bindings function
    int clear_bindings();

    /// \brief Analogue to sqlite3_finalize function. No other function can be called on statement object after the call
    int finalize();

    /**
      \brief Destructor finalize object if it not still finalized
      */
    ~Statement()
    {
        if (stmt_)
        {
            finalize();
        }
    }
};

/**
  \brief Contains transaction functionality. Automatically make rollback on destruction if commit was not called
  */
class Transaction
{
private:
    Sqlite & conn_;
    enum State
    {
        Created,
        Active,
        Finished
    } state_;

    // Create throw acquire() function
    Transaction(Sqlite & conn):conn_(conn), state_(Created)
    {

    }

    int init();

public:

    /**
      \brief Create and start new transaction
      \param conn database connection to start transaction on
      \return transaction object on success or NULL if failed
      */
    static Transaction * acquire(Sqlite & conn);

    /**
      \brief Commit transaction. After the successfull call subsequental
        commit/rollback for this object will take no effent
      \return 0 on succeed, SQLITE error code otherwise
      */
    int commit();

    /**
      \brief Rollback transaction. After the successfull call subsequental
        commit/rollback for this object will take no effent
      \return 0 on succeed, SQLITE error code otherwise
      */
    int rollback();

    /**
      \brief Rollback transaction if no commit/rollback fuctions was called before destruction
      */
    ~Transaction();
};

}   // namespace BookDb
}   // namespace pocketbook

#endif // SQLITE_H
