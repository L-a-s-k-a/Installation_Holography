#include "../Inc/motorDriver.h"
#include "../Inc/interrupt.h"

Motor_Characteristics motorCharacter;
Angular_Parameters angular;

/** 
 * @brief Функция настройки параметров привода
 * 
 * @param rpmMax  Максимальное количсество оборотов/мин привода
 * @param freqMax Максимальное значение управляющей частоты, подаваемой 
 *                на вход блока управления приводом
 * @param rpsMax  Максимальное количество оборотов/сек привода
 * @param mode    Режим управления (вычислений), в котором предпочтиетельнее работать
 *                  Этот параметр может быть настроен на одно из следующих значений:
 *                  @arg REV_PER_MIN: Управление в оборотах в минуту
 *                  @arg REV_PER_SEC: Управление в оборотах в секунду
 *                  @arg RAD_PER_MIN: Управление в радианах в минуту
 *                  @arg RAD_PER_SEC: Управление в радианах в секунду
 * 
 * @retval Ничего не возращает
*/
void Motor_Init(float rpmMax, float freqMax, float rpsMax, Calc_Mode mode)
{
    // motorCharacter->FCPU = 180000000.0;
    // motorCharacter->PPS = pulsePerSecond;
    // motorCharacter->freqMax = freqMax;
    // motorCharacter->revPSMax = rpsMax;
    // motorCharacter->revPMMax = rpmMax;
    // motorCharacter->radPMMax = rpmMax * 2 * M_PI;
    // motorCharacter->radPSMax = rpsMax * 2 * M_PI;
    motorCharacter.FCPU = 180000000.0;
    motorCharacter.oneRPS = 10000.0;
    motorCharacter.freqMax = freqMax;
    motorCharacter.revPSMax = rpsMax;
    motorCharacter.revPMMax = rpmMax;
    motorCharacter.radPMMax = rpmMax * 2 * M_PI;
    motorCharacter.radPSMax = rpsMax * 2 * M_PI;
    switch (mode)
    {
    case REV_PER_MIN:
        motorCharacter.divisionFactor = (motorCharacter.FCPU * motorCharacter.revPMMax) / motorCharacter.freqMax;
        // motorCharacter->divisionFactor = (motorCharacter->FCPU * motorCharacter->revPMMax) / motorCharacter->freqMax;
        break;
    case REV_PER_SEC:
        motorCharacter.divisionFactor = (motorCharacter.FCPU * motorCharacter.revPSMax) / motorCharacter.freqMax;
        // motorCharacter->divisionFactor = (motorCharacter->FCPU * motorCharacter->revPSMax) / motorCharacter->freqMax;
        break;
    case RAD_PER_MIN:
        motorCharacter.divisionFactor = (motorCharacter.FCPU * motorCharacter.radPMMax) / motorCharacter.freqMax;
        // motorCharacter->divisionFactor = (motorCharacter->FCPU * motorCharacter->radPMMax) / motorCharacter->freqMax;
        break;
    case RAD_PER_SEC:
        motorCharacter.divisionFactor = (motorCharacter.FCPU * motorCharacter.radPSMax) / motorCharacter.freqMax;
        // motorCharacter->divisionFactor = (motorCharacter->FCPU * motorCharacter->radPSMax) / motorCharacter->freqMax;
        break;
    }
}

/** 
 * @brief Функция пересчёта заданного значения угловой скорости в количетсво уровней ARR МК
 * 
 * @param value Значение скорости
 * 
 * @retval Возвращает уоличество уровней ARR
*/
float Speed_Transformation_In_ARR(float value)
{
    return motorCharacter.divisionFactor / value;
}

/** 
 * @brief Функция для управления углом поворота привода. При вызове заадётся угол,
 * на который необходимо повернуть и угловую скорость, с которой должен происходить поворот.
 * 
 * @param taskDegree Задание угла 
 * @param taskAngularVelocity Задание угловой скорости
 * @param arrTIM Адрес таймера
 * 
 * @retval Ничего не возвращает
*/
void Set_Number_Of_Revolutions(float taskDegree, float taskAngularVelocity, TIM_TypeDef *arrTIM)
{
    motorCharacter.freq = motorCharacter.FCPU / Speed_Transformation_In_ARR(taskAngularVelocity); //Получение частоты из заданной угловой скорости
    calculatePulseARR = motorCharacter.freq; //Передача значения частоты в прерывание, через глобальную внешнюю переменную
    angular.calculateDegree = (motorCharacter.freq  * 360.0) / 10000.0; //Вычисление колчества градусов в секунду, приходящихся на заданную скорсть
    angular.timeForRevolution = taskDegree / angular.calculateDegree; //Вычисление количества времени, необходимого для поворота вала двигателя на заданный угол
    if (FLAG_Revolution < angular.timeForRevolution) //Если пройденное время не превышает расчётного времени, то запускается фунцкия задания скорости
    {
        Set_Smooth_Speed(taskAngularVelocity, 5.0 arrTIM);
    }
    else //Иначе двигатель останавливается
    {
        Set_Smooth_Speed(0, 5.0, arrTIM);
    }
}

/** 
 * @brief Функция управления скоростью. Если разница между текущим значением скорости
 *        и заданным больше step (шаг увеличения/уменьшения скорости), то будет 
 *        происходитьплавное увеличение скорости, аналогично происходит работа в 
 *        обратную сторну. Если передаваемое значение скорости 0, то на ARR таймера передаётся 0,
 *        таймер перестаёт генерировать сигнал и двигатель останавливается.
 * 
 * @param speed   Задание скорости вращения
 * @param step    Максимально возможное изменение скорсти, превышение данного значения 
 *                запустит алгоритм плавного увеличения/уменьшения скорости
 * @param arrTIM  Адрес таймера
 * 
 * @retval Ничего не возвращает
*/
void Set_Smooth_Speed(float speed, float step, TIM_TypeDef *arrTIM)
{
    static SSS_Parameters param;
    param.step = step;
    param.setSpeed = param.previousValue;
    param.divStep = abs(speed - param.previousValue) / param.step;
    if (speed > param.previousValue + param.step)
    {
        for (uint8_t i = 0; i < param.divStep; i++)
        {
            delay(60);
            param.setSpeed += param.step;
            arrTIM->ARR = Speed_Transformation_In_ARR(param.setSpeed);
        }
    }
    else if (speed < param.previousValue - param.step)
    {
        for (uint8_t i = 0; i < param.divStep; i++)
        {
            delay(60);
            param.setSpeed -= param.step;
            arrTIM->ARR = Speed_Transformation_In_ARR(param.setSpeed);
        }
    }
    else if (speed == 0)
    {
        arrTIM->ARR = 0;
    }
    else
    {
        arrTIM->ARR = Speed_Transformation_In_ARR(speed);
    }
    param.setSpeed = 0;
    param.previousValue = speed;
    delay(5); //Задержка обязательна, если её не будет, вычисления будут производиться не корректно
}