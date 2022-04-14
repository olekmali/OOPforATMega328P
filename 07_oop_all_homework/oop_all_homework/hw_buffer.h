//* This is a "stub" file that can be replaced by the fully functional circular buffer-based averaging class template *
#ifndef HW_BUFFER2021_H_
#define HW_BUFFER2021_H_

#include <stddef.h>

template <typename Tdata, typename Tsum, typename Tsize, Tsize nLongAv, Tsize nShortAv>
class CircularBuffer
{
public:
                CircularBuffer() : myBack(0), mySize(0) { }

    bool        empty() const   { return(mySize==0);  }

    void        store(const Tdata & value); // TO DO store (push) a new item, forget the oldest if necessary
    void        clear();                    // TO DO discard all (pop all) data quickly
    bool        ready() const;              // TO DO check if at least nShortAv elements are held
    Tdata       LongAverage()  const;       // TO DO average all CircularBuffer elements
    Tdata       ShortAverage() const;       // TO DO average the last nShortAv elements
private:
    Tdata       myArray[nLongAv];
    Tsize       myBack;
    Tsize       mySize;
    // additional variables might be needed for the sake of averaging operation efficiency
};

template <typename Tdata, typename Tsum, typename Tsize, Tsize nLongAv, Tsize nShortAv>
void CircularBuffer<Tdata, Tsum, Tsize, nLongAv, nShortAv>::store(const Tdata & value)
{
    // TO DO
    // ..
}


template <typename Tdata, typename Tsum, typename Tsize, Tsize nLongAv, Tsize nShortAv>
inline void CircularBuffer<Tdata, Tsum, Tsize, nLongAv, nShortAv>::clear()
{
    // TO DO
    // ..
}

template <typename Tdata, typename Tsum, typename Tsize, Tsize nLongAv, Tsize nShortAv>
bool CircularBuffer<Tdata, Tsum, Tsize, nLongAv, nShortAv>::ready() const
{
    bool result = false;
    // TO DO
    // ..
    return(result);
}

template <typename Tdata, typename Tsum, typename Tsize, Tsize nLongAv, Tsize nShortAv>
Tdata CircularBuffer<Tdata, Tsum, Tsize, nLongAv, nShortAv>::LongAverage() const
{
    Tsum sum = 0;
    // TO DO
    // ..
    return( static_cast<Tdata>(sum) );
}

template <typename Tdata, typename Tsum, typename Tsize, Tsize nLongAv, Tsize nShortAv>
Tdata CircularBuffer<Tdata, Tsum, Tsize, nLongAv, nShortAv>::ShortAverage() const
{
    Tsum sum = 0;
    // TO DO
    // ..
    return( static_cast<Tdata>(sum) );
}

#endif
