docker rm `docker ps -a | awk '{if(NR==2) print }'`
docker rmi imglab:
