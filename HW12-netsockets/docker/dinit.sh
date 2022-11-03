#!/bin/sh

if [ ! -f .env ]; then
	echo S_UID=$(id -u) >> .env
	echo S_GID=$(id -g) >> .env
fi

docker-compose build

