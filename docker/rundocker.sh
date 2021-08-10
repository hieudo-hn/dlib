docker run -it -e DISPLAY=host.docker.internal:0 \
  --env="_X11_NO_MITSHM=1" \
  --volume=/tmp/.X11-unix:/tmp/.X11-unix:rw \
  build /bin/bash
