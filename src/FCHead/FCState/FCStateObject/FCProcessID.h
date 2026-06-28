#ifndef FC_PROCESS_ID_H
#define FC_PROCESS_ID_H

//! идентификаторы процессов
enum FCProcessID 
{   
    // id - внутрених обьектов
    NoID = 10,            // нет ID  

    // id - обьектов работающие с очередью сообщений
    HeadID = 100, // головка 
        MixerID = 110, // миксер
        LaserScanerID = 120, // сканер 
        ConditionerID = 130, // кондиционер
        IndicatorID = 140, // индикатор 
        DisplayID = 150, // дисплей
            FrameID, // фрейм
            WindowID, // окно
        StepDirMotor = 160, // шаговый двигатель
        FeedersManager = 170, // податчик     
};

#endif // FC_PROCESS_ID_H