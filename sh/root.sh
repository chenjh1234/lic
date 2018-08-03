route add -net 224.0.0.0 netmask 240.0.0.0 dev eth1
/etc/init.d/iptables stop
/etc/init.d/iptables status
