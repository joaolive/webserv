FROM mcr.microsoft.com/vscode/devcontainers/base:ubuntu-22.04

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
	&& apt-get install -y --no-install-recommends \
		build-essential \
		clang \
		cmake \
		make \
		valgrind \
		gdb \
		llvm \
		lldb \
		ca-certificates \
		&& apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/lists/*
