# Use the Ubuntu 22.04 base image
FROM ubuntu:22.04

# Install necessary build tools and dependencies for Django
RUN apt-get update && apt-get install -y \
    build-essential \
    python3 \
    python3-pip \
    libpq-dev \
    clang \
    pkg-config \
    libgmp3-dev \
    && rm -rf /var/lib/apt/lists/*

# Install Python dependencies for the Django project
WORKDIR /usr/src/app
COPY ./requirements.txt .
RUN pip3 install -r requirements.txt


COPY ./c_scripts/encryption/src /usr/bin/encryption/src
COPY ./c_scripts/compression/src /usr/bin/compression/src

WORKDIR /usr/bin/encryption/src
RUN make
RUN mv keygen /usr/bin
RUN mv encrypt /usr/bin
RUN mv decrypt /usr/bin

WORKDIR /usr/bin/compression/src
RUN make
RUN mv encode /usr/bin
RUN mv decode /usr/bin

# Step 3: Set up the Django project
WORKDIR /usr/src/app
COPY  ./compressme /usr/src/app/

# Expose the port for the Django application
EXPOSE 8000

# Command to run the Django development server
CMD ["python3", "manage.py", "runserver", "0.0.0.0:8000"]
