#ifndef CARMEN_CPP_MAP_DEFINITIONS_H
#define CARMEN_CPP_MAP_DEFINITIONS_H

#include "genericmap.h"

namespace mre {

  class CharCell {
  public:
    CharCell(char v=-1) { set(v); }
    CharCell(const CharCell& x) { *this = x; }
    void set(char v) { val = v; }
  
    operator char() const { return val;}
    CharCell& operator= (const char& v)  { val = v; return *this;}
  
    char val;
  };


  class IntCell {
  public:
    IntCell(int v=-1) { set(v); }
    IntCell(const IntCell& x) { *this = x; }
    void set(int v) { val = v; }
  
    operator int() const { return val;}
    IntCell& operator= (const int& v)  { val = v; return *this;}
  
    int val;
  };

  class FloatCell {
  public:
    FloatCell(float v = -1.0) { set(v); }
    FloatCell(const FloatCell& x) { *this = x; }
    void set(float v) { val = v; }

    operator float() const { return val;}
    FloatCell& operator= (const float& v)  { val = v; return *this;}
  
    float val;
  };

  class DoubleCell {
  public:
    DoubleCell(double v = -1.0) { set(v); }
    DoubleCell(const DoubleCell& x) { *this = x; }
    void set(double v) { val = v; }

    operator double() const { return val;}
    DoubleCell& operator= (const double& v)  { val = v; return *this;}
  
    double val;
  };

  class RefProbCell {
  public:
    RefProbCell(int hits=0, int misses=0) { set(hits, misses); }
    RefProbCell(const RefProbCell& x) {  *this = x;  }
  
    void set(int hits, int misses) { 
      this->hits = hits;  
      this->misses = misses; 
      this->val = -1; 
      if (hits+misses > 0) {
        val = ((float)hits)/((float) (hits+misses));
      }
      updated = false;
    }
	   
    void hit() { 
      hits++;
      updated = true;
    }

    void miss() { 
      misses++;
      updated = true;
      val = ((float)hits)/((float) (hits+misses));
    }

    operator float() {  
      if (updated) {
        val = ((float)hits)/((float) (hits+misses));
        updated = false;
      }
      return val;  
    }
    operator double() {      
      if (updated) {
        val = ((float)hits)/((float) (hits+misses));
        updated = false;
      }
      return val;  
    }
  
    int hits;
    int misses;
    float val;
    bool updated;
  };


  class TypeCell {

  public:
    
    enum CellTypes {UNKNOWN=-1, FREE, OCCUPIED, FLAT, ROUGH, BOUNDARY};

  public:
    TypeCell(char v=UNKNOWN) { set(v); }
    TypeCell(const TypeCell& x) { *this = x; }
    void set(char v) { val = v; }
  
    bool initialized() const { return !(val == UNKNOWN); }
    operator char() const { return val;}
    TypeCell& operator= (const char& v)  { val = v; return *this;}
  
    char val;
  };


  typedef GenericMap<CharCell>     CharMap;
  typedef GenericMap<IntCell>      IntMap;
  typedef GenericMap<FloatCell>    FloatMap;
  typedef GenericMap<DoubleCell>   DoubleMap;
  typedef GenericMap<RefProbCell>  RefProbMap;
  typedef GenericMap<TypeCell>     TypeMap;

}

#endif
