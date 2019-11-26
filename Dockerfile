FROM cmbant/docker-gcc-build
RUN apt-get update
RUN apt-get install bison -y
WORKDIR /app
ADD . .
CMD /bin/bash