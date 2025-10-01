#include "utils.h"

uint16_t max_search(uint16_t* array, uint8_t size){
    if(size==0)
        return 0;
    uint16_t max = array[0];
    for(uint8_t i=1;i<size;i++){
        if(array[i]>max)
            max = array[i];
    }
    return max;
}

uint16_t min_search(uint16_t* array, uint8_t size){
    if(size==0)
        return 0;
    uint16_t min = array[0];
    for(uint8_t i=1;i<size;i++){
        if(array[i]<min)
            min = array[i];
    }
    return min;
}

uint16_t get_avg(uint16_t* array, uint8_t size){
    if(size==0)
        return 0;
    uint32_t sum = 0;
    for(uint8_t i=0;i<size;i++){
        sum += array[i];
    }
    return sum/size;
}

int16_t max_search_signed(int16_t* array, uint8_t size){
    if(size==0)
        return 0;
    int16_t max = array[0];
    for(uint8_t i=1;i<size;i++){
        if(array[i]>max)
            max = array[i];
    }
    return max;
}

int16_t min_search_signed(int16_t* array, uint8_t size){
    if(size==0)
        return 0;
    int16_t min = array[0];
    for(uint8_t i=1;i<size;i++){
        if(array[i]<min)
            min = array[i];
    }
    return min;
}

int16_t get_avg_signed(int16_t* array, uint8_t size){
    if(size==0)
        return 0;
    int32_t sum = 0;
    for(uint8_t i=0;i<size;i++){
        sum += array[i];
    }
    return sum/size;
}

uint8_t timer_check(uint32_t interval, uint32_t millis_elapsed){
  if(HAL_GetTick()-(millis_elapsed)>interval){
    return 1;
  }else{
    return 0;
  }
}