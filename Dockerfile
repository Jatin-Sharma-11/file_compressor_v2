# Use an official Python runtime as a parent image
FROM python:3.8-slim

# Set the working directory in the container
WORKDIR /app/fileCompressor_v_2

# Copy the current directory contents into the container at /app/fileCompressor_v_2
COPY . /app/fileCompressor_v_2

# Ensure the required directories exist
RUN mkdir -p /app/fileCompressor_v_2/uploads /app/fileCompressor_v_2/downloads

# Copy the executables into the working directory
COPY lzwCompression.exe /app/fileCompressor_v_2/
COPY lzwDecompress.exe /app/fileCompressor_v_2/
COPY c.exe /app/fileCompressor_v_2/
COPY d.exe /app/fileCompressor_v_2/

# Install any needed packages specified in requirements.txt
RUN pip install --no-cache-dir -r requirements.txt

# Make port 80 available to the world outside this container
EXPOSE 80

# Define environment variable
ENV NAME World

# Run Gunicorn server when the container launches
CMD ["gunicorn", "--bind", "0.0.0.0:80", "main:app"]
