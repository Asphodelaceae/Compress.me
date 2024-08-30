# Use the Ubuntu 22.04 base image
FROM ubuntu:22.04

# Install necessary build tools and dependencies for Django
RUN apt-get update && apt-get install -y \
    build-essential \
    python3 \
    python3-pip \
    libpq-dev \
    && rm -rf /var/lib/apt/lists/*

# Install Python dependencies for the Django project
WORKDIR /usr/src/app
COPY ./requirements.txt .
RUN pip3 install -r requirements.txt


COPY ./c_scripts/encryption/binaries /usr/bin/
COPY ./c_scripts/compression/binaries /usr/bin/

# Step 3: Set up the Django project
WORKDIR /usr/src/app
COPY  ./compressme /usr/src/app/

# Expose the port for the Django application
EXPOSE 8000

# Command to run the Django development server
CMD ["python3", "manage.py", "runserver", "0.0.0.0:8000"]
