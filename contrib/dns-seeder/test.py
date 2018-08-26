import socket

seeders = [
    'bwkseed.mempool.pw',
    'bwkseed1.faccrypto.com',
    'bwkseed2.faccrypto.com',
    'bwkseed3.faccrypto.com',
    'bwkseed4.faccrypto.com',
    'bwkseed5.faccrypto.com',
    'bwkseed1.faccrypto.site',
    'bwkseed2.faccrypto.site',
    'bwkseed3.faccrypto.site',
    'bwkseed4.faccrypto.site',
    'bwkseed5.faccrypto.site'
]

for seeder in seeders:
    try:
        ais = socket.getaddrinfo(seeder, 0)
    except socket.gaierror:
        ais = []
    
    # Prevent duplicates, need to update to check
    # for ports, can have multiple nodes on 1 ip.
    addrs = []
    for a in ais:
        addr = a[4][0]
        if addrs.count(addr) == 0:
            addrs.append(addr)
    
    print(seeder + ' = ' + str(len(addrs)))
