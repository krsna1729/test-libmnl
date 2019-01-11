docker rm testmnl -f
docker build -t testmnl .
docker run -itd --name testmnl --cap-add NET_ADMIN testmnl
