Peer_to_Peer_Communication_Using_Flooding
=========================================

Used C language and worked on solaris cluster

Problem:
Peer-to-peer based computing has been widely used at all levels such as to realize user-level
applications, kernel-owned applications such as servers, and inter-connection between network routers
(hardware). Broadcasting data is a common method of communication between peer-to-peer applications.
Flooding is a simple method of realizing broadcasting. The purpose of this project is to implement message
flooding for communication between peers. We assume a static configuration, i.e., all peers form a network
at the beginning, each peer sets up connection with a few other peers, connections remain unchanged, and
that no peer leaves in the middle.
For a quick overview of the project, visualize a network of peers as an
undirected, non-weighted graph with vertices repres
enting the peers and edges representing communication
links. Let vertex number 1 be called
init
, which is the only peer that generates messages. It generates a pair
of integers called message and TTL and delivers the pair to a fixed peer (vertex number 2). Peer 2 picks up
each pair in order, reduces its TTL by 1, and if it is
not zero, it sends the message and the updated TTL to
every peer connected to it directly. Ot
her peers then do the same, i.e., they pick up each message in order of
its arrival, reduce its TTL by 1, and if
it is still not zero, they send the me
ssage to their neighbors other than
the one who sent the message. If the
initial TTL is large enough, the message
would reach to all peers in the
network; many peers may get the same message multiple
times, however. In this
project, peers are threads
of a multi-threaded program, all running the same procedure. Each peer owns a queue and its neighbors
send a message to it by putting it in its queue. 
