/**
  \brief This file contains MessageServer class declaration. MessageServer class is intended
    for message based server/client interprocess communication using PF_UNIX sockets.
  \author Yuriy Lisovyy
  */
#ifndef MESSAGE_SERVER_H_
#define MESSAGE_SERVER_H_

#include <vector>
#include <cstring>
#include <pthread.h>

namespace pocketbook 
{
namespace utilities
{

/**
  \brief Interface incapsulate message from users of MessageServer class.
    User work with arriving messages through this interface
  */
class IMessage
{
public:
    /**
      \brief Get message data
      */
    virtual const std::vector<char>& data() const = 0;
    virtual ~IMessage(){}
};

/**
  \brief Interface for user message handler
  */
class IMessageHandler
{
public:
    /**
        \brief Function to handle message. MessageServer calls the function for each received message.
            Handler responsible for message deletion
        \param message received message
      */
    virtual void HandleMessage(IMessage * message) = 0;
    virtual ~IMessageHandler(){}
};

/**
  \brief Class for server side of interprocess communication. Instance of the class create separate thread to
    listen for arriving messages and call message handler from the thread.
    Message communication is reliable and doesn't reorder messages
  */
class MessageServer
{
public:
    MessageServer():socket_(-1),handler_(NULL), max_message_size_(0), thread_(0), is_running_(false){}

    ~MessageServer();
    /**
      \brief Initialize message server
      \param socket_name name for PF_UNIX socket (should be the same as used by clients). Can contain \0 symbol
      \param socket_name_len length of socket_name param
      \param handler handler for arriving messages (would be called from another thread)
      \param max_message_size maximum message size allowed by user protocol
      \return 0 on succeed
      */
    int Init(const char * socket_name, size_t socket_name_len, IMessageHandler* handler, size_t max_message_size, bool run_thread = true);
    int Deinit();

    int ProcessMessage(int timeout_ms);
    /**
      \brief Send responce message to the client (client determined by original_message parameter)
      \param original_message received message (passed to HandleMessage function)
      \param response_data responce data to be send to client
      \return 0 on succeed
      */
    int SendResponse(IMessage* original_message, const std::vector<char>& response_data);
    int SendResponse(IMessage* original_message, const char* data, size_t data_size);
    int getSocket() { return socket_; }
private:
    static void* ServerThread(void*);
    int socket_;
    IMessageHandler* handler_;
    size_t max_message_size_;
    pthread_t thread_;
    bool is_running_;
};

} // namespace utilities
} // namespace pocketbook

#endif // MESSAGE_SERVER_H_
