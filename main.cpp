#include <iostream>
#include <stdio.h>

#include "tcp/server.h"
#include "tcp/client.h"

#include <string>
#include <cstring>

void log_error(std::string msg);
void start_server(uint16_t port);
void start_client(std::string address, uint16_t port);


void log_error(std::string msg)
{
	std::cout << "TCP: " << msg << std::endl;
}

void start_server(uint16_t port)
{
	std::cout << "MAIN: Starting server on port " << port << std::endl;

	tcp::Server *server = new tcp::Server();
	server->set_log_function(log_error);

	bool success = server->start(port);

	if (!success)
	{
		std::cout << "MAIN: Could not start server" << std::endl;
		return;
	}

	tcp::Packet packet;
	while (true)
	{
		while (server->get_next_packet(packet))
		{
			if (packet.type == tcp::PacketType::Connect)
			{
				std::string client_address = server->get_client_address(packet.sender);
				std::cout << "MAIN: Client connected from " << client_address << std::endl;
			}
			else if (packet.type == tcp::PacketType::Disconnect)
			{
				std::cout << "MAIN: Client disconnected" << std::endl;
			}
			else if (packet.type == tcp::PacketType::Message)
			{
				std::string data = std::string(packet.data, packet.length);

				std::cout << "MAIN: Client send a message: '" << data << "'" << std::endl;

				if (data == "hi") {
					std::cout << "MAIN: sending 'hello'" << std::endl;
					server->send(packet.sender, "hello", 5);
				}
				if (data == "stop")
				{
					std::cout << "MAIN: Stopping" << std::endl;
					server->stop();
					return;
				}

				delete [] packet.data;
			}
		}
	}
}
void start_client(std::string address, uint16_t port)
{
	std::cout << "MAIN: Connecting to " << address << ":" << port << std::endl;

	tcp::Client *client = new tcp::Client();
	client->set_log_function(log_error);

	bool success = client->connect(address.c_str(), port);

	if (!success) {
		std::cout << "MAIN: Connection failed" << std::endl;
		return;
	}

	tcp::Packet packet;
	while (true)
	{
		while (client->get_next_packet(packet))
		{
			if (packet.type == tcp::PacketType::Connect)
			{
				std::cout << "MAIN: Connected to server" << std::endl;
				std::cout << "MAIN: sending 'hi'" << std::endl;
				client->send("hi", 2);
			}
			else if (packet.type == tcp::PacketType::Disconnect)
			{
				std::cout << "MAIN: Disconnected" << std::endl;
			}
			else if (packet.type == tcp::PacketType::Message)
			{
				std::string data = std::string(packet.data, packet.length);

				std::cout << "MAIN: Server send a message: '" << data << "'" << std::endl;

				if (data == "hello") {
					std::cout << "MAIN: sending 'stop' and stopping" << std::endl;
					client->send("stop", 4);
					client->disconnect();
					return;
				}
				if (data == "stop")
				{
					std::cout << "MAIN: Stopping" << std::endl;
					client->disconnect();
					return;
				}

				delete [] packet.data;
			}
		}
	}
}

int main()
{
	bool is_client = true;
	std::string address = "127.0.0.1";
	uint16_t port = 7357;


	std::string input;
	std::cout << "Start client or server (c/s): ";
	std::getline(std::cin, input);

	if (input == "s")
	{
		is_client = false;
	}

	if (is_client)
	{
		std::cout << "Server address and port (127.0.0.1:7357): ";
		std::getline(std::cin, input);

		if (input.length() != 0)
		{
			try
			{
				address = input.substr(0, input.find(":"));
				port = (uint16_t)std::stoi(input.substr(input.find(":") + 1, input.length()));
			}
			catch (std::invalid_argument)
			{
				std::cout << "Invalid address" << std::endl;
				return EXIT_FAILURE;
			}
		}
	}
	else
	{
		std::cout << "Server port (7357): ";
		std::getline(std::cin, input);

		if (input.length() != 0)
		{
			try
			{
				port = (uint16_t)std::stoi(input);
			}
			catch (std::invalid_argument)
			{
				std::cout << "Invalid port: " << std::endl;
				return EXIT_FAILURE;
			}
		}
	}


	if (is_client)
	{
		start_client(address, port);
	}
	else
	{
		start_server(port);
	}

	std::cout << "End" << std::endl;
	std::cin.get();

	return EXIT_SUCCESS;
}
