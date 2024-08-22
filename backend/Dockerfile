# Use the Ubuntu 22.04 base image
FROM ubuntu:22.04

# Install necessary build tools and dependencies for Django
RUN apt-get update && apt-get install -y \
    build-essential \
    clang \
    make \
    libgmp3-dev \
    python3 \
    python3-pip \
    libpq-dev \
    && rm -rf /var/lib/apt/lists/*

# Install Python dependencies for the Django project
WORKDIR /usr/src/app
COPY ./requirements.txt .
RUN pip3 install -r requirements.txt

# Step 1: Build the encryption binaries
WORKDIR /usr/src/encryption
COPY ./c_scripts/encryption/src/ /usr/src/encryption/
RUN make all
RUN cp encrypt /usr/local/bin/
RUN cp decrypt /usr/local/bin/
RUN cp keygen /usr/local/bin/
RUN make clean

# Step 2: Build the compression binaries
WORKDIR /usr/src/compression
COPY  ./c_scripts/compression/src/ /usr/src/compression/
RUN make all
RUN cp encode /usr/local/bin/
RUN cp decode /usr/local/bin/
RUN make clean

# Step 3: Set up the Django project
WORKDIR /usr/src/app
COPY  ./compressme /usr/src/app/

# Expose the port for the Django application
EXPOSE 8000

# Command to run the Django development server
CMD ["python3", "manage.py", "runserver", "0.0.0.0:8000"]
