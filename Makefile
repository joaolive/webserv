# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mhidani <mhidani@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/23 11:00:12 by joaolive          #+#    #+#              #
#    Updated: 2026/05/07 18:56:53 by mhidani          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Parallel build
UNAME_S := $(shell uname -s)

ifeq ($(MAKELEVEL),0)
	ifeq ($(UNAME_S),Linux)
		NPROC := $(shell nproc)
	else
		NPROC := $(shell sysctl -n hw.ncpu)
	endif
	MAKEFLAGS += -j$(NPROC)
endif

NAME = webserv

# Colors
CC_GREEN    = \033[38;2;80;250;123m
CC_PINK     = \033[38;2;255;121;198m
CC_BLUE     = \033[38;2;139;233;253m
CC_YELLOW   = \033[38;2;241;250;140m
CC_PURPLE   = \033[38;2;189;147;249m
CC_RED      = \033[38;2;255;85;85m
RESET       = \033[0m

# Tools
CC = c++
RM = rm -rf

# Directories
SRC_DIR = src
BIN_DIR = bin
OBJ_DIR = obj
INC_DIR = include

#Flags and Includes
CCFLAGS = -Wall -Wextra -Werror -std=c++98
DEPFLAGS = -MMD -MP
CPPFLAGS = -I$(INC_DIR)

SRC_MAIN = $(addsuffix .cpp, main)
SRC_IO =  $(addsuffix .cpp, ReadFile)
SRC_LEXER =  $(addsuffix .cpp, Lexer)
SRC_MODEL =  $(addsuffix .cpp, Config LocationConfig ServerConfig)
SRC_PARSER = $(addsuffix .cpp, LocationParser Parser ParseUtils ServerParser)
SRC_INFRA_ENG = $(addsuffix .cpp, AcceptHandler ClientHandler ServerEngine)
SRC_INFRA_EXP = $(addsuffix .cpp, IoMonitorException SocketException)
SRC_HTTP = $(addsuffix .cpp, HttpProcessorFactoryImpl HttpRequestParser HttpProcessorImpl)
SRC_ROUTER = $(addsuffix .cpp, HttpRouter)
SRC_HANDLER = $(addsuffix .cpp, GetHandler RequestHandler)
SRC_SHARED = $(addsuffix .cpp, Utils)

# Source files
SRC = \
	$(addprefix $(SRC_DIR)/, $(SRC_MAIN)) \
	$(addprefix $(SRC_DIR)/config/io/, $(SRC_IO)) \
	$(addprefix $(SRC_DIR)/config/lexer/, $(SRC_LEXER)) \
	$(addprefix $(SRC_DIR)/config/model/, $(SRC_MODEL)) \
	$(addprefix $(SRC_DIR)/config/parser/, $(SRC_PARSER)) \
	$(addprefix $(SRC_DIR)/infra/engine/, $(SRC_INFRA_ENG)) \
	$(addprefix $(SRC_DIR)/infra/exception/, $(SRC_INFRA_EXP))
	$(addprefix $(SRC_DIR)/http/model/, $(SRC_HTTP)) \
	$(addprefix $(SRC_DIR)/http/router/, $(SRC_ROUTER)) \
	$(addprefix $(SRC_DIR)/http/handler/, $(SRC_HANDLER)) \
	$(addprefix $(SRC_DIR)/shared/, $(SRC_SHARED))

# Object files
SRC_OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))

# Progress variables
TOTAL := $(words $(SRC_OBJS))
COUNT := 0
BAR_WIDTH := 20

# Dependencies
DEPS = $(SRC_OBJS:.o=.d)

# Target
all: $(BIN_DIR)/$(NAME)

# Link
$(BIN_DIR)/$(NAME): $(SRC_OBJS) | $(BIN_DIR)
	@printf "$(CC_GREEN)$(NAME) linked successfully!$(RESET)\n"
	@$(CC) $(CCFLAGS) $(SRC_OBJS) -o $(BIN_DIR)/$(NAME)

# Include dependency files to track header changes
-include $(DEPS)

# Compile with progress bar
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@$(eval COUNT=$(shell echo $$(($(COUNT)+1))))
	@PERCENT=$$(($(COUNT)*100/$(TOTAL))); \
	FILLED=$$(( $(BAR_WIDTH) * $(COUNT) / $(TOTAL) )); \
	EMPTY=$$(( $(BAR_WIDTH) - $$FILLED )); \
	BAR=$$(printf "█%.0s" $$(seq 1 $$FILLED))$$(printf "░%.0s" $$(seq 1 $$EMPTY)); \
	printf "\r$(CC_BLUE)[%s] %3d%% | compiling $(RESET)%s" "$$BAR" $$PERCENT $<; \
	$(CC) $(CCFLAGS) $(CPPFLAGS) $(DEPFLAGS) -c $< -o $@; \
	if [ $(COUNT) -eq $(TOTAL) ]; then printf "\n"; fi

# Create directories
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# Cleaning rules
clean:
	@$(RM) $(OBJ_DIR)
	@echo "$(CC_YELLOW)🕸️  Object files destroyed!$(RESET)"

fclean: clean
	@$(RM) $(BIN_DIR)
	@echo "$(CC_PURPLE)⚰️  Binary buried!$(RESET)"

re: fclean
	@$(MAKE) all --no-print-directory
	@echo "$(CC_GREEN)🦇  The Project has risen from the dead!$(RESET)"

val: $(BIN_DIR)/$(NAME)
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --vgdb=no $(BIN_DIR)/$(NAME)
	@echo ""
	@echo "$(CC_PINK)⣿⣿⡇⡏⣸⣷⣳⣹⡿⣿⣿⣿⣿⣿⣿⣿⣿⣧⠄⣷⣿⡇⠄⠄⠄⠄⠄⠸$(RESET)"
	@echo "$(CC_PINK)⣿⣿⣿⣧⠿⣿⣿⣿⣿⣿⣿⣿⡿⠿⠻⢿⣿⣿⣦⡸⣿⣷⠄⠄⠄⠄⠄⠄$(RESET)"
	@echo "$(CC_PINK)⣿⣿⣿⡡⠦⣄⡹⣿⣿⣿⣿⠃⢠⣶⣶⣦⡌⢿⣿⣿⣾⣿⡆⠄⠄⠄⠄⠄$(RESET)"
	@echo "$(CC_PINK)⣿⣿⣿⣇⠄⣿⡇⣿⣿⣿⣿⡀⠊⠄⢸⣿⡿⢸⣿⣿⣿⣿⡇⠄⠄⠄⠄⠄$(RESET)"
	@echo "$(CC_PINK)⣿⣿⣿⣿⣝⣋⣠⣿⣿⣿⣿⣧⡈⠒⠚⢛⣡⣾⣿⣿⣿⣿⠇⠄⠄⠄⠄⠄$(RESET)"
	@echo "$(CC_PINK)⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣾⣭⣽⣿⣿⣿⣿⣿⡿⠄⠄⠄⠄⠄⠄$(RESET)"
	@echo "$(CC_PINK)⣿⣿⣿⡇⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠇⠄⠄⠄⠄⠄⠄$(RESET)"
	@echo "$(CC_PINK)⣿⣿⣿⡷⠄⢿⣿⣹⣯⣽⣿⣿⣿⣿⣿⣿⣿⣿⠟⣫⡶⡀⠄⠄⠄⠄⠄⠄$(RESET)"
	@echo "$(CC_PINK)⣿⣿⣿⡟⡀⠘⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠛⣡⣾⣿⡕⠁⠄⠄⠄⠄⠄⠄$(RESET)"
	@echo "$(CC_PINK)⣿⣿⣿⡟⡅⠄⠘⣿⣿⣿⣿⡿⠟⠋⣡⣴⣿⣿⣿⠟⠄⠄⠄⠄⠄⠄⠄⠄$(RESET)"
	@echo "$(CC_PINK)⣿⣿⣿⣷⡇⠄⠄⠘⢯⣍⣡⣤⣶⣿⣿⣿⣿⡿⠃⠄⠄⠄⠄⠄⠄⠄⠄⠄$(RESET)"
	@echo ""
	@echo "$(CC_RED)🕷️  Hunting leaks in VALGRIND mode...$(RESET)"

debug: CCFLAGS += -g3 -fsanitize=address
debug: re
		@echo ""
		@echo "$(CC_PINK)⣿⣿⣿⣿⣿⣿⣿⣿⠿⠛⠋⠉⠁⠄⠄⠈⠙⠻⣿⣿⣿⣿$(RESET)"
		@echo "$(CC_PINK)⣿⣿⣿⣿⣿⣿⠟⠁⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠙⢿⣿$(RESET)"
		@echo "$(CC_PINK)⣿⣿⣿⣿⡿⠃⠄⠄⠄⢀⣀⣀⡀⠄⠄⠄⠄⠄⠄⠄⠈⢿$(RESET)"
		@echo "$(CC_PINK)⣿⣿⣿⡟⠄⠄⠄⠄⠐⢻⣿⣿⣿⣷⡄⠄⠄⠄⠄⠄⠄⠈$(RESET)"
		@echo "$(CC_PINK)⣿⣿⣿⠃⠄⠄⠄⢀⠴⠛⠙⣿⣿⡿⣿⣦⠄⠄⠄⠄⠄⠄$(RESET)"
		@echo "$(CC_PINK)⣿⣿⠃⠄⢠⡖⠉⠄⠄⠄⣠⣿⡏⠄⢹⣿⠄⠄⠄⠄⠄⢠$(RESET)"
		@echo "$(CC_PINK)⣿⠃⠄⠄⢸⣧⣤⣤⣤⢾⣿⣿⡇⠄⠈⢻⡆⠄⠄⠄⠄⣾$(RESET)"
		@echo "$(CC_PINK)⠁⠄⠄⠄⠈⠉⠛⢿⡟⠉⠉⣿⣷⣀⠄⠄⣿⡆⠄⠄⢠⣿$(RESET)"
		@echo "$(CC_PINK)⠄⠄⠄⠄⠄⠄⢠⡿⠿⢿⣷⣿⣿⣿⣿⣿⠿⠃⠄⠄⣸⣿$(RESET)"
		@echo "$(CC_PINK)⠄⠄⠄⠄⠄⢀⡞⠄⠄⠄⠈⣿⣿⣿⡟⠁⠄⠄⠄⠄⣿⣿$(RESET)"
		@echo "$(CC_PINK)⠄⠄⠄⠄⠄⢸⠄⠄⠄⠄⢀⣿⣿⡟⠄⠄⠄⠄⠄⢠⣿⣿$(RESET)"
		@echo "$(CC_PINK)⠄⠄⠄⠄⠄⠘⠄⠄⠄⢀⡼⠛⠉⠄⠄⠄⠄⠄⠄⣼⣿⣿$(RESET)"
		@echo "$(CC_PINK)⠄⠄⠄⠄⠄⡇⠄⠄⢀⠎⠄⠄⠄⠄⠄⠄⠄⠄⠄⠙⢿⣿$(RESET)"
		@echo "$(CC_PINK)⠄⠄⠄⠄⢰⠃⠄⢀⠎⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠙$(RESET)"
		@echo ""
		@echo "$(CC_RED)🕷️  Hunting bugs in DEBUG mode...$(RESET)"


# PHONY Targets
.PHONY: all clean fclean re val debug
