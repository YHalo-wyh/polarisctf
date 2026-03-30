#!/bin/bash
     
echo $FLAG > /flag
chmod 400 /flag
FLAG="flag{not_here}"

# DO NOT DELETE
service apache2 start
service ssh start
sleep infinity;