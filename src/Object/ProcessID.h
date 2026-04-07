#ifndef PROCESS_ID_H
#define PROCESS_ID_H

//! идентификаторы процессов
enum ProcessID 
{   
    // id - внутрених обьектов
    NoID = 100,            // нет ID  

    // id - обьектов работающие с очередью сообщений
    HeadID = 1000, // головка 
        MixerID = 1100, // миксер
        LaserScanerID = 1200, // сканер 
        ConditionerID = 1300, // кондиционер
        IndicatorID = 1400, // индикатор 
        DisplayID = 1500, // дисплей
            FrameID, // фрейм
            WindowID, // окно
};

#endif // PROCESS_ID_H