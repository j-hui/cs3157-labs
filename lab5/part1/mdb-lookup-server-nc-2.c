#!/bin/sh
while true; do
    read -p "port number: " p || break
    test -n "$p" && ./mdb-lookup-server-nc-1 "$p" &
done
