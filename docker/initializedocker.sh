docker pull zbirenbaum/sealgui
docker build . --tag imglab

docker run -it -e DISPLAY=host.docker.internal:0 \
  --mount type=bind,source="$(pwd)"/../,target=/dlibSealGUI \
  --env="_X11_NO_MITSHM=1" \
  --volume=/tmp/.X11-unix:/tmp/.X11-unix:rw \
  imglab:latest ../sealbuild_docker
