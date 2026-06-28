#include "FCCANBus.h"

const string FCCANBus::scan() 
{ 
    string result = ""; //\n     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\n";
    // char buf[6];
    // for (int row = 0; row < 0x80; row += 0x10) 
    // {
    //     sprintf(buf, "%02x: ", row);
    //     result += buf;

    //     for (int col = 0; col < 16; col++) 
    //     {
    //         int addr = row + col;

    //         if (addr < 0x03 || addr > 0x77) // Исключаем зарезервированные адреса
    //         {
    //             sprintf(buf, "   ");
    //         } 
    //         else 
    //         {
    //             int ret = write(addr << 1, nullptr, 0); // Проверяем наличие устройства
    //             if (ret == 0) 
    //             {
    //                 sprintf(buf, "%02x ", addr);
    //             } 
    //             else 
    //             {
    //                 sprintf(buf, "-- ");
    //             }
    //         }
    //         result += buf;
    //     }
    //     result += "\n";
    // }
    return result;
}

FCCANPacket FCCANBus::receive()
{
    CANMessage message;
    if(!read(message))
    {
        return {0};
    }

    if(message.id == 0x100 && message.len == sizeof(FCCANPacket)) 
    {
        // безопасное копирование данных
        memcpy(&_packet, message.data, sizeof(FCCANPacket));
    }

    return {0};
}

int FCCANBus::send(FCCANPacket packet)
{
    if(packet._id == 0x01) 
    {
        // отправка подтверждения (ACK)
        CANMessage message;
        message.id = 0x300; // heartbeat/ACK ID
        message.len = sizeof(FCCANPacket);
        memcpy(message.data, &packet, message.len);
        return write(message);
    }
    return 0;
}

FCCANPacket& FCCANBus::exchange(FCCANPacket &packet)
{
    if(receive()._id) 
    {
        send(packet);
    }
    
    return _packet;
}

// void on_can_receive() 
// {
//     CANMessage msg;
//     if(can.read(msg)) 
//     {
//         if(msg.id == 0x100 && msg.len == sizeof(MoveCmd)) 
//         {
//             // Безопасное копирование данных
//             MoveCmd cmd;
//             memcpy(&cmd, msg.data, sizeof(MoveCmd));
//             if(cmd.cmd_id == 0x01) 
//             {
//                 printf("Received Move: X=%d, Y=%d, Speed=%d, Pen=%d, Seq=%d\r\n", 
//                        cmd.target_x, cmd.target_y, cmd.speed, cmd.pen_state, cmd.seq_num);
//                 // TODO: Добавить команду в очередь планировщика движения
//                 // planner.add_move(cmd.target_x, cmd.target_y, cmd.speed, cmd.pen_state);
//                 // Отправка подтверждения (ACK)
//                 CANMessage ack_msg;
//                 ack_msg.id = 0x300; // Heartbeat/ACK ID
//                 ack_msg.data[0] = cmd.seq_num; // Эхо номера команды
//                 ack_msg.len = 1;
//                 can.write(ack_msg);
//             }
//         }
//     }
// }
