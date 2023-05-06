/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtemel <mtemel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 22:16:32 by yasinsensoy       #+#    #+#             */
/*   Updated: 2023/05/06 15:47:24 by mtemel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Channel.hpp"
#include "../headers/Client.hpp"
#include "../headers/Server.hpp"

Server::Server(int argc, char **argv)
{
	appointment(argc, argv);
	std::cout << "Starting IRC server on port " << std::endl;

	socketOperations();
	socketOperations2(argv);

	cap_ls[0] = "NICK";
	cap_ls[1] = "JOIN";
	cap_ls[2] = "QUIT";
	cap_ls[3] = "CAP";
	cap_ls[4] = "KICK";

	this->is_nick_first = 0;
}

Server::~Server(){}

void  Server::appointment(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Arg Error." << std::endl;
		exit(1);
	}
	this->my_port = std::stoi(argv[1]);
	this->my_password = argv[2];
}

void	Server::loop()
{
	std::string message;
	while (1)
	{
		poll(pollfds.begin().base(), pollfds.size(), -1);
		for (size_t i = 0 ; i <  pollfds.size() ; i++)
		{
			if (pollfds[i].revents & POLLIN)
			{
				if (pollfds[i].fd == pollfds[0].fd)
				{
					newClient();
					break ;
				}
				executeCommand(pollfds[i].fd);
			}
		}
	}
}

void	Server::newClient()
{
	buffer.clear();
	int	addr_len = sizeof(address);
	this->new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t *)&addr_len);
	std::cout << "New socket: " << this->new_socket << std::endl;
	if (this->new_socket < 0)
	{
		std::cerr << "Accept failed" << std::endl;
		close(this->new_socket);
		// close(server_fd);
		// exit(1);
	}
	else
	{
		this->pollfds.push_back((pollfd){this->new_socket, POLLIN, 0});

		// Client c(this->new_socket, this->my_port);
		// this->clients_.push_back(c);
		// std::cout << "User connected: " << this->clients_.size() << "." << std::endl;
		// std::cout << "Fds size: " << this->pollfds.size() << "." << std::endl;
	
		std::map<int, std::string>::iterator it;
		it = cap_ls.begin();
		std::string str;
		while (it != cap_ls.end())
		{
			str.append('/' + it->second+"\n");
			++it;
		}
		send(this->new_socket, str.c_str(), str.size(), 0);
		// while (it != cap_ls.end())
		// {
		// 	std::string str = "/";
		// 	str += it->second += "\r\n";
		// 	send(this->new_socket, str.c_str(), str.size(), 0);
		// 	++it;
		// }
	}
	
}