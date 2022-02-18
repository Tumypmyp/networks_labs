# Networks labs
Code, docker files and environment setting for Networks course laboratories. 
[![Docker Pulls](https://img.shields.io/docker/pulls/tumypmyp/networks.svg)](https://hub.docker.com/r/tumypmyp/networks/)


# Usage

To run admin container with privileges for tshark.
```
docker run --name admin --cap-add=NET_RAW --cap-add=NET_ADMIN -ti tumypmyp/networks
```

To run user
```
docker run --name user -ti tumypmyp/networks
```
