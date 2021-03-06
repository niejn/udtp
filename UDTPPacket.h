/*******************************************
*General UDTP Packet. Easy place to hold all the
*enums.
*******************************************/
#ifndef __UDTP_PACKET
#define __UDTP_PACKET
#include "UDTPPeer.h"
class UDTP;
class UDTPPeer;

enum TransferType{
    Push = 0x21,
    Pull = 0x22
};
enum PacketType{
    Packet = 0x00,
    Handshake = 0x01,
    Header = 0x02,
    Path = 0x03,
    Chunk = 0x04,
    Acknowledge = 0x05
};

// TODO: Suggest renaming to PacketCode
enum ResponseCode{
    /* Generic packet codes */
    ResponseStart = 0xD9,
    ResponseNone = 0xF0,  /*Every response code starts out with none. Tihs means the request is unfulfilled.*/
    ResponseApproved = 0xF1,
    ResponseRejected = 0xF2,
    ResponseRetry = 0xF7, /*Just in case server is not ready to provide something, he will ask client to retry the packet again at a different time*/
    ResponseCriticalError = 0xF8, /*This will make the client FORCE quit the client with a error message.*/
    ResponseConfirm = 0xD8, /*Another approval message! This is usually sent from the peer to the HOST*/

    /* TODO: move these to file packet code */
    ResponseFileNotFound = 0xF3,
    ResponseFileExistsAlready = 0xF4,


    /* TODO: move this to path packet code */
    ResponseListingFound= 0xF5,
    ResponseInvalidPath = 0xF6,

};

struct UDTPPacketHeader {
    unsigned short packetSize;
    PacketType packetType;
};

class UDTPPacket{
    public:
        /*There is no UDTPPacket constructor, since we will never use it!*/
        ResponseCode get_response_code() { return _responseCode; };
        bool set_response_code(ResponseCode responseCode) { _responseCode = responseCode; return true;};

        PacketType get_packet_type() { return _header.packetType; };
        bool set_packet_type(PacketType packetType) { _header.packetType = packetType; return true;};

        unsigned int get_socket_id() { return _socketID;};
        bool set_socket_id(unsigned int socketID) { _socketID = socketID;};

        unsigned int get_peer_id() { return _peerID;};
        bool set_peer_id(unsigned int peerID) { _peerID = peerID;};
        bool set_unique_id(unsigned int newID){
            _uniqueID = newID;
        }
        unsigned int get_unique_id(){
            return _uniqueID;
        }




        UDTP* udtp() { return _myUDTP;};
        bool set_udtp(UDTP* myUDTP){
            _myUDTP = myUDTP;
            return true;
        }
        UDTPPeer* peer(){
            return _myPeer;
        }
        bool set_peer(UDTPPeer* myPeer){
            _myPeer = myPeer;
            return true;
        }


        bool verify_unique_id(){
            return  _myPeer->get_unique_id() == get_unique_id();
        }

        /* JH - Integrated UDTPData into generic packet structure. */
        unsigned short get_packet_size() { return _header.packetSize;};
        virtual bool set_raw_buffer(char* raw){ _raw = raw; };
        virtual char& write_to_buffer() {return  *_raw;};

        /* JH - Pure virtual function that derived packets must impliment */
        char* get_raw_buffer() { return _raw;};
        virtual bool pack() = 0; /*Modifies _raw*/
        virtual bool unpack() = 0; /*Modifies _raw*/
        virtual bool respond() = 0; /*Modifies the variables taken from the values of _raw*/

    protected:
        /*These can be transmitted*/
        UDTPPacketHeader _header;
        ResponseCode _responseCode; /*Not every packet uses a response code, like Chunk or Whine -- since those do not need a response.*/
        UDTP* _myUDTP;
        UDTPPeer* _myPeer;
        /*Local variables! They will NOT transmit through the network!*/
        unsigned int _peerID; /*It's numerical location in the peer's list*/
        unsigned int _uniqueID;
        unsigned int _socketID; /*Optional holder*/ /*Since indexes and sockets are different entirely on each system! These are local!*/

        char* _raw; /*This is the holder for raw data*/


};
#endif


