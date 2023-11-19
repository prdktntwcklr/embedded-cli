FROM ubuntu:latest

# set working directory
WORKDIR /app

# set timezone
RUN ln -snf /usr/share/zoneinfo/$CONTAINER_TIMEZONE /etc/localtime && \
    echo $CONTAINER_TIMEZONE > /etc/timezone

# update package information and install required packages
RUN apt-get update && \
    apt-get install --no-install-recommends -y build-essential clang-format cmake gcovr git ninja-build python3 python3-pip ruby && \
    pip install --no-cache-dir pre-commit && \
    gem install ceedling

# clean up stale packages
RUN apt-get clean -y && \
    apt-get autoremove --purge -y && \
    rm -rf /var/lib/apt/lists/*
