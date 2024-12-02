# Base Image
FROM python:3.14.0a2-alpine

WORKDIR /app
# Copy Configuration
COPY ./requirements.txt requirements.txt
RUN pip install --no-cache-dir -r requirements.txt
COPY . .
# Expose Ports
CMD ["python", "app.py"]