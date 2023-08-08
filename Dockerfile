FROM alpine:latest
RUN apk --no-cache add --virtual .build-dependencies \
  alpine-sdk autoconf automake git

WORKDIR /root
COPY . ./
RUN git clean -fdx
RUN ./autogen.sh
RUN ./configure --prefix= CFLAGS=-static
RUN make install-strip

FROM scratch
COPY --from=0 /bin/advent /advent

ENV WORKSPACE="/root"
ENV HOME="${WORKSPACE}"
WORKDIR "${WORKSPACE}"
ENTRYPOINT ["/advent"]
