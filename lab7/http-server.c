#!/usr/bin/env bash

set -euo pipefail

d () { echo "$@" >&2 ; }

if [ "$#" != 4 ] ; then
    echo -n "usage: http-server <server-port> <web-root>" >&2
    echo    " <mdb-lookup-host> <mdb-lookup-port>" >&2
    exit 1
fi

server_port="$1"
web_root="$2"
mdb_lookup_host="$3"
mdb_lookup_port="$4"

if [ "$server_port" -lt 1023 ] || [ 65535 -lt "$server_port" ] ; then
    echo "Server port out of range: $server_port" >&2
    exit 1
fi
if nc -z 0.0.0.0 "$server_port" ; then
    echo "Server port already in use: $server_port" >&2
    exit 1
fi
if [ "$mdb_lookup_port" -lt 1023 ] || [ 65535 -lt "$mdb_lookup_port" ] ; then
    echo "mdb-lookup-server port out of range: $mdb_lookup_port" >&2
    exit 1
fi
if ! nc -z "$mdb_lookup_host" "$mdb_lookup_port" ; then
    echo "Could not connect to $mdb_lookup_host:$mdb_lookup_port" >&2
    exit 1
fi

server_pipe="$$.server.pipe"
mdb_i="$$.mdb-in.pipe"
mdb_o="$$.mdb-out.pipe"

rm -f "$mdb_i" "$mdb_o"
mkfifo "$mdb_i" "$mdb_o"
(cat "$mdb_i" | nc "$mdb_lookup_host" "$mdb_lookup_port" > "$mdb_o") &
exec 3> "$mdb_i"
exec 4< "$mdb_o"
trap "rm -f '$server_pipe' '$mdb_i' '$mdb_o' ; exit 1" INT

while true ; do
    rm -f "$server_pipe"
    mkfifo "$server_pipe"
    meth=""
    uri=""
    ver=""
    cont=""
    state=""

    d "Waiting for connection..."
    nc -l "$server_port" < "$server_pipe" | \
        while [ -z "$cont" ] && read -r l ; do
        if [ -z "$meth" ] ; then
            d "Connection received. Handling..."
            d
            d "    $l"
        fi
        if [ -z "$state" ] ; then
            l="$(echo "$l" | tr '\t' ' ' | tr -d '\r' | tr -s ' ')"
            meth="$(echo "$l" | cut -d ' ' -f 1)"
            uri="$(echo "$l" | cut -d ' ' -f 2)"
            ver="$(echo "$l" | cut -d ' ' -f 3)"

            if [ -n "$(echo "$l" | cut -d ' ' -f 4)" ] ; then
                echo -e "HTTP/1.0 501 Not Implemented\r"
                echo -e "\r"
                echo "<html><body><h1>501 Not Implemented</h1></body></html>"
                cont="yes"
            elif [ "$meth" != "GET" ] ; then
                echo -e "HTTP/1.0 501 Not Implemented\r"
                echo -e "\r"
                echo "<html><body><h1>501 Not Implemented</h1></body></html>"
                cont="yes"
            elif [ "$ver" != "HTTP/1.0" ] && [ "$ver" != "HTTP/1.1" ] ; then
                echo -e "HTTP/1.0 501 Not Implemented\r"
                echo -e "\r"
                echo "<html><body><h1>501 Not Implemented</h1></body></html>"
                cont="yes"
            elif echo "$uri" | grep -q -v '^/' ; then
                echo -e "HTTP/1.0 400 Bad Request\r"
                echo -e "\r"
                echo "<html><body><h1>400 Bad Request</h1></body></html>"
                cont="yes"
            elif echo "$uri" | grep -q '/\.\./' ; then
                echo -e "HTTP/1.0 400 Bad Request\r"
                echo -e "\r"
                echo "<html><body><h1>400 Bad Request</h1></body></html>"
                cont="yes"
            elif echo "$uri" | grep -q '/\.\.$' ; then
                echo -e "HTTP/1.0 400 Bad Request\r"
                echo -e "\r"
                echo "<html><body><h1>400 Bad Request</h1></body></html>"
                cont="yes"
            else
                state="drain"
            fi
        else
            l="$(echo "$l" | tr -d '\r')"
            if [ -z "$l" ] ; then
                case "$uri" in
                "/mdb-lookup"|"/mdb-lookup?"*)
                    echo -e "HTTP/1.0 200 OK\r"
                    echo -e "\r"
                    echo "<html><body><h1>mdb-lookup</h1>"
                    echo "<p>"
                    echo "<form method=GET action=/mdb-lookup>"
                    echo "lookup: <input type=text name=key>"
                    echo "<input type=submit>"
                    echo "</form>"
                    echo "<p>"
                    if [ "$uri" != "/mdb-lookup" ] ; then
                        lookup="${uri}"
                        lookup="${lookup#*\?}"
                        lookup="${lookup#*key=}"
                        lookup="${lookup%%&*}"

                        echo "$lookup" >&3

                        echo "<p><table border>"
                        color=""
                        while read -u 4 -r m ; do
                            [ -z "$m" ] && break
                            case "$color" in
                            yellow)
                                echo -n "<tr><td bgcolor=yellow>"
                                color=""
                                ;;
                            *)
                                echo -n "<tr><td>"
                                color="yellow"
                                ;;
                            esac
                            echo "$m"
                        done

                        echo "</table>"
                    fi
                    echo "</body></html>"
                    ;;
                *)
                    if echo "$uri" | grep -q '/$' ; then
                        uri="${uri}index.html"
                    fi

                    if [ -d "$web_root$uri" ] ; then
                        echo -e "HTTP/1.0 403 Forbidden\r"
                        echo -e "\r"
                        echo "<html><body><h1>403 Forbidden</h1></body></html>"
                        cont="yes"
                    elif ! [ -r "$web_root$uri" ] ; then
                        echo -e "HTTP/1.0 404 Not Found\r"
                        echo -e "\r"
                        echo "<html><body><h1>404 Not Found</h1></body></html>"
                        cont="yes"
                    else
                        echo -e "HTTP/1.0 200 OK\r"
                        echo -e "\r"
                        cat "$web_root$uri"
                    fi
                    ;;
                esac
                cont="yes"
            fi
        fi
    done > "$server_pipe" || true
    d "Finished handling connection."
done
