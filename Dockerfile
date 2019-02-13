FROM ubuntu as build
RUN apt-get update && apt-get install -y make gcc libmnl-dev
ADD . /code
WORKDIR /code
RUN make

FROM ubuntu
RUN apt-get update && apt-get install -y libmnl0 iproute2
COPY --from=build /code/nltest /bin/nltest
RUN mkdir -p /deps && ldd /bin/nltest | grep "=> /" | awk '{print $3}' | xargs -I '{}' cp -v '{}' /deps
CMD /bin/nltest
