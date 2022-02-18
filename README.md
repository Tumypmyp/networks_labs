# Networks labs
[![Docker Pulls](https://img.shields.io/docker/pulls/tumypmyp/networks.svg)](https://hub.docker.com/r/tumypmyp/networks/)

Code, docker files and environment setting for Networks course laboratories. 


# Usage

To run a admin container with privileges for tshark:
```
docker run --name admin --cap-add=NET_RAW --cap-add=NET_ADMIN -ti tumypmyp/networks
```

To run a user container:
```
docker run --name user -ti tumypmyp/networks
```
