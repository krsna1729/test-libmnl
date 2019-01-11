FROM ubuntu:18.10 as build
RUN apt-get update && apt-get install -y make gcc libmnl-dev iproute2
ADD . /code
WORKDIR /code
RUN make

FROM ubuntu:18.10
RUN apt-get update && apt-get install -y libmnl0 iproute2
COPY --from=build /code/nltest /bin/nltest
CMD /bin/nltest
