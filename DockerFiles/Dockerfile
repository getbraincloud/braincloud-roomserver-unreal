FROM gcr.io/distroless/cc-debian10:nonroot

COPY --chown=nonroot:nonroot ./ /home/nonroot/server
WORKDIR /home/nonroot/server

# Ports (This does nothing, documentation purpose only)
EXPOSE 7777/tcp
EXPOSE 7777/udp

# Execute
ENTRYPOINT ["/home/nonroot/server/DedicatedDemo/Binaries/Linux/DedicatedDemoServer", "DedicatedDemo"]
CMD []