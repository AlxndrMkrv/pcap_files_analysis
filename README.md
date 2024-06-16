## PCAP File Analysis

### Description:

This project analyzes a set of PCAP files and provides detailed statistics for
each file.

### Given:

* A set of PCAP files (zip archive).

### Functionality:

The program generates the following statistics for each PCAP file:

1. **Total number of packets.**

2. **Total length of packets.**

3. **Distribution of packet lengths (in bytes):**
    * <=64
    * 65 - 255
    * 256 - 511
    * 512 - 1023
    * 1024 - 1518
    * \>= 1519
    
4. **Distribution of packets by L3 protocols:**
    * IPv4
    * IPv6
    * other

5. **Distribution of packets by L4 protocols:**
    * TCP
    * UDP
    * ICMP/ICMPv6
    * other

6. **Number of unique values for the following fields:**
    * Source MAC
    * Destination MAC
    * Source IP
    * Destination IP
    * Source Port
    * Destination Port

7. **Distribution of TCP packets by flags:**
    * SYN
    * SYN+ACK
    * ACK
    * FIN+ACK
    * RST
    * RST+ACK
    * other

8. **Number of packets with valid and invalid checksums for L3 and L4 headers.**

### Implementation Details:

* The program accepts the path to a directory containing PCAP files and outputs 
the statistics for each file to the console in text format.
* The code is written in C++23.
* Modern C++ practices are followed. At least I tried =)
* Uses libpcap for reading PCAP files. No other third-party libraries are used.
* Packet parsing and checksum verification are performed manually.

### Build and Run Instructions:

1. **Clone the repository:**

```sh
git clone git@github.com:AlxndrMkrv/pcap_files_analysis.git
cd pcap_files_analysis
```

2. **Build the project:**

```sh
mkdir build
cd build
cmake ..
make -j
make tests -j
```

3. **Run tests:**

```sh
ctest
```

4. **Prepare pcap files:**

Use your favorite program to unzip `pcap/cpp-dev-task-pcaps.zip` file

5. **Run the application:**

```sh
./pcap_files_analysis <path_to_directory_with_pcap_files>
```

