#!/bin/bash
resize -s 5 150 > /dev/null
function verify_ipaddr()
{
	local  addr=$1
		local  stat=1

		if [[ $addr =~ ^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$ ]]; then
			OIFS=$IFS
				IFS='.'
				addr=($addr)
				IFS=$OIFS
				[[ ${addr[0]} -le 255 && ${addr[1]} -le 255 \
				&& ${addr[2]} -le 255 && ${addr[3]} -le 255 ]]
				stat=$?
		fi
		return $stat
}
echo "Enter the IP Address of the game server."
echo "Please make sure the server is reachable over the network from this machine";
read ipa;

while true
do
	if verify_ipaddr $ipa; then echo $ipa > address ; break;fi
	echo "Doesn't look like an ip address. Please re enter";
	read ipa;
done
echo "Starting the client "
#./client $ipa 0 > logs/client0.log &
#./client $ipa 0 & 2>&1 >logs/client.log

sleep 1
