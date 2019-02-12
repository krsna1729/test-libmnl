[![Build Status](https://dev.azure.com/krsna1729/krsna1729/_apis/build/status/krsna1729.test-libmnl?branchName=master)](https://dev.azure.com/krsna1729/krsna1729/_build/latest?definitionId=1&branchName=master)

```docker
build:
        docker build -t testmnl .
run: 
        docker run --rm -itd --name testmnl --cap-add NET_ADMIN testmnl
exec:
        docker exec testmnl ip link set mtu 1200 dev eth0
logs:
        docker logs testmnl
stop:
        docker stop testmnl
```
