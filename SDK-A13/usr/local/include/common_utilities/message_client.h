/**
  \brief This file contains MessageClient class declaration. MessageClient class is intended
    for message based server/client interprocess communication using PF_UNIX sockets (see MessageServer for server side implementation).
  \author Yuriy Lisovyy
  */
#ifndef MESSAGECLIENT_H
#define MESSAGECLIENT_H

#include <vector>
#include <cstring>
#include <string>

namespace pocketbook
{
namespace utilities
{

/**
  \brief Class implements message client. It intended for sending messages to MessageServer (running at the same computer)
    and optionaly waiting for responce
    Message communication is reliable and doesn't reorder messages
  */
class MessageClient
{
public:
    MessageClient();
    /**
      \brief Initialize client
      \param remote_socket_name name for server side PF_UNIX socket
      \param remote_socket_name_len length of remote_socket_name
      \param max_message_size maximum message size allowed by user protocol
      \return 0 on success
      */
    int Init(const char * remote_socket_name, size_t remote_socket_name_len, size_t max_message_size);

    /**
      \brief send message to server
      \param data message data to send
      \param data_size length of data
      */
    int SendMessage(const char* data, size_t data_size);

    /**
      \brief wait for server responce for the message. Implementation is not guarantee neither that server respond to the specific message
        nor that server send responce at all
      \param data buffer to receive message data
      */
    int WaitResponce(std::vector<char>* data, int timeout_ms);

    // thread safe function
    int WaitResponceNoRead(int timeout_ms);

    int getSocket() const { return socket_; }
    ~MessageClient();

private:
    int socket_;
    size_t max_message_size_;
    std::vector<char> remote_socket_name_;
    size_t remote_socket_name_len_;
};

} // namespace utilities
} // namespace pocketbook

#endif // MESSAGECLIENT_H
