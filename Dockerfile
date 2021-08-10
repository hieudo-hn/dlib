#syntax=docker/dockerfile:1
FROM imglab:latest
ENV DISPLAY :0
WORKDIR /dlibSealGUI/run
RUN ./imglab
