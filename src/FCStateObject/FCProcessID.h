#ifndef FC_PROCESS_ID_H
#define FC_PROCESS_ID_H

//! идентификаторы процессов
enum FCProcessID 
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

#endif // FC_PROCESS_ID_H