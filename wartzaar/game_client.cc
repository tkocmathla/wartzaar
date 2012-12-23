#include "wartzaar/game_client.h"

#include <cstdio>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace wartzaar {

GameClient::GameClient(const std::string &host, int port)
    : host_(host),
      port_(port) {
  Init();
}

GameClient::~GameClient() {
  try {
    Disconnect();
  }
  catch (std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
  }
}

void GameClient::Init() {
  int result = WSAStartup(0x0202, &wsadata_);
  if (result != NO_ERROR) {
    std::stringstream ss;
    ss << "WSAStartup failed with error code: " << result;
    throw std::runtime_error(ss.str());
  }

  if (wsadata_.wVersion != 0x0202) {
    WSACleanup();
    throw std::runtime_error("Required Winsock DLL version not found!");
  }

  // Define the address family, IP address, and port of the destination server
  target_.sin_family = AF_INET;
  target_.sin_port = htons(port_);
  target_.sin_addr.s_addr = inet_addr(host_.c_str());
}

boolean GameClient::Connect() {
  // Create a socket
  sock_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock_ == INVALID_SOCKET) {
    WSACleanup();
    std::stringstream ss;
    ss << "Socket creation failed with error code: " << WSAGetLastError();
    throw std::runtime_error(ss.str());
  }

  // Connect to the destination server
  int result = connect(sock_, (sockaddr *) &target_, sizeof(target_));
  if (result == SOCKET_ERROR) {
    closesocket(sock_);
    WSACleanup();
    std::stringstream ss;
    ss << "Socket connection failed with error code: " << WSAGetLastError();
    throw std::runtime_error(ss.str());
  }

  return true;
}

void GameClient::Disconnect() {
  int result = closesocket(sock_);
  if (result == SOCKET_ERROR) {
    std::stringstream ss;
    ss << "Socket close failed with error code: " << WSAGetLastError();
    throw std::runtime_error(ss.str());
  }

  WSACleanup();
}

void GameClient::SendGameMessage(const wartzaar::messages::GameMessage &message) {
  std::cout << "Sending message: " << const_cast<wartzaar::messages::GameMessage&>(message).text();

  int result = send(
    sock_,
    const_cast<wartzaar::messages::GameMessage&>(message).text().c_str(),
    const_cast<wartzaar::messages::GameMessage&>(message).text().length(),
    0
  );

  if (result == SOCKET_ERROR) {
    std::stringstream ss;
    ss << "Send failed with error code: " << WSAGetLastError();
    throw std::runtime_error(ss.str());
  }

  // Disable transmission
  // TODO: Put this in Disconnect()?
  //
//  result = shutdown(sock_, SD_SEND);
//  if (result == SOCKET_ERROR) {
//    std::stringstream ss;
//    ss << "Shutdown failed with error code: " << WSAGetLastError();
//    throw std::runtime_error(ss.str());
//  }
}

// Reads a single byte at a time from the socket and accumulates the data in
// a vector. When a "\r\n" is received, ReceiveMessage stops and returns the
// data as a string.
//
std::string GameClient::ReceiveGameMessage() {
  int result;
  char buffer[1];
  std::vector<unsigned char> message;

  do {
    result = recv(sock_, buffer, 1, 0);

    if (result > 0) {
      message.push_back(buffer[0]);

      if (message.size() < 2)
        continue;

      if (message.at(message.size() - 2) == '\r' && message.back() == '\n')
        break;
    }
    else if (result == 0) {
      // TODO: How to handle this gracefully?
      std::cout << "Connection closed by peer." << std::endl;
      break;
    }
    else {
      std::stringstream ss;
      ss << "Receive failed with error code: " << WSAGetLastError();
      throw std::runtime_error(ss.str());
    }
  } while (result > 0);

  return std::string(message.begin(), message.end() - 2);
}

void GameClient::set_game_version(const std::string &game_version) {
  game_version_ = game_version;
}

void GameClient::set_manager_version(const std::string &manager_version) {
  manager_version_ = manager_version;
}

} // namespace wartzaar
