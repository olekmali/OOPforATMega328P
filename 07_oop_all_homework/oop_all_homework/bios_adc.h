//* OOP bios_adc.h *
#ifndef BIOS_ADC_H_
#define BIOS_ADC_H_
#include <stdint.h>

class ADConverter {
    public:
public:
    static ADConverter&  getInstance()                  { return(instance);        }

                 void    initialize(uint8_t firstc = CH0, uint8_t endc = CHMAX);
                 void    shutdown();
                 void    startRound();
                 uint8_t isReady() const                { return(new_results);     }
                 void    resetReady()                   { new_results = 0; }
                 void    setReady()                     { new_results = 1; }

                 uint8_t getRecent(uint8_t channel) const;

                 void    setRange(uint8_t firstc, uint8_t endc);
                 uint8_t getBegin() const               { return(ch_begin); }
                 uint8_t getEnd() const                 { return(ch_end); }
                 uint8_t getCapacity() const            { return(ADConverter::CHMAX);  }


    static const uint8_t CH0 = 0;
    static const uint8_t CH1 = 1;
    static const uint8_t CH2 = 2;
    static const uint8_t CH3 = 3;
    static const uint8_t CH4 = 4;
    static const uint8_t CH5 = 5;
    static const uint8_t CHMAX = 6;

private:
    static ADConverter instance;

    volatile uint8_t new_results;   // Note: read/write uint8_t is an atomic transaction
             uint8_t ch_begin;      // The first channel number to scan
             uint8_t ch_end;        // The last channel number to scan plus one ("end", not "last")

    ADConverter() : new_results(0), ch_begin(0), ch_end(ADConverter::CHMAX) {}
    ADConverter(const ADConverter&);
    void operator=(const ADConverter&);

};



#endif /* BIOS_ADC_H_ */