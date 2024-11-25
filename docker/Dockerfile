# Base Image
FROM eclipse-mosquitto:latest

# Copy Configuration
COPY ./mosquitto.conf /mosquitto/config/mosquitto.conf

# Expose Ports
EXPOSE 1883 9001

CMD ["/usr/sbin/mosquitto", "-c", "/mosquitto/config/mosquitto.conf"]