/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhidani <mhidani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 20:00:48 by mhidani           #+#    #+#             */
/*   Updated: 2026/05/07 20:00:50 by mhidani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/Server.hpp"

int main(int argc, char** argv) {
	int exitCode = 0;

	if (argc != 2) {
		std::cerr << "error try enter: ./bin/webserv server.conf" << std::endl;
		return 1;
	}

	exitCode = Server::start(argv[1]);

	return exitCode;
}
