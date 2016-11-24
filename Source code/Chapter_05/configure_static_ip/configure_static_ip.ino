void setup() {
  // put your setup code here, to run once:

system("ifconfig eth0 192.168.1.28 netmask 255.255.255.0 up");
system("route add default gw 192.168.1.1 netmask 255.255.255.0");

}

void loop() {
  // put your main code here, to run repeatedly: 
  
}
