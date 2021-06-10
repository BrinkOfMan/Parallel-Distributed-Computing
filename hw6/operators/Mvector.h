#include <iostream>
#include <cassert>

using namespace std; 

class Mvector{
    int len;    /* length of the array */
    const int default_len = 10; 

    public:

        float * arr;  /* an array representing vector elements */
        /* standard constructor */
        Mvector(int length);

        /* default constructor */ 
        Mvector();

        /* the copy constructor */
        Mvector(const Mvector &V);

    /* METHODS */
    /* overload = operator */
    Mvector operator=(const Mvector &mvec);

    /* overload [] operator */
    float operator[](int index);

    /* overload scalar product (*) operator */
    Mvector operator*(float val);

    /* overload dot product operator */
    Mvector operator*(const Mvector & mvec);

    /* overload vector addition operator */
    Mvector operator+(const Mvector & mvec);

    /* get methods*/
    float getLen() {
        return this->len; 
    }

    float * getArr() {
        return this->arr; 
    }


    /* dump method */ 
    void dump();

    /* the destructor */
    ~Mvector();
};

Mvector::Mvector() {
    cout << "Default constructor" << endl; 
    this->len = default_len;
    this->arr = new float[default_len]; 
    for (int i = 0; i < default_len; i++) {
        this->arr[i] = 0; 
    }
}

Mvector::Mvector(int length) {
    cout << "Standard constructor" << endl; 
    len = length;
    /* length represents the desired length of Mvector*/
    this->arr = new float[length]; 
    for (int i = 0; i < length; i++) {
        this->arr[i] = 0; 
    }
}

Mvector::Mvector(const Mvector &mvec) {
    cout << "Copy constructor" << endl; 
    this->len = mvec.len; 
    this->arr = new float[len];

    for (int l = 0; l < mvec.len; l++) {
        this->arr[l] = mvec.arr[l]; 
    }
}

Mvector::~Mvector() {
    cout << "Freeing arr memory" << endl; 
    delete [] arr; 
}

void Mvector::dump() {
    cout << "===== Dumpin info =====" << endl; 

    cout << "Len = " << this->len << endl;

    cout << "Array: " << endl;
    for (int j = 0; j < this->len; j++) {
        cout << j << " : " << this->arr[j] << endl; 
    }
}

Mvector Mvector::operator=(const Mvector &mvec) {
    this->len = mvec.len; 
    this->arr = new float[mvec.len]; 

    for (int i = 0; i < mvec.len; i++) {
        this->arr[i] = mvec.arr[i]; 
    }
    return *this; 
}

float Mvector::operator[](int index) {
    assert(index < 0 || index >= len); 
    return this->arr[index];   
}

Mvector Mvector::operator*(float val) {
    this->len = len; 

    float * newArr; 
    newArr = new float[len];

    for (int l = 0; l < len; l++) {
        newArr[l] = this->arr[l]*val; 
    }

    return *this;
}

Mvector Mvector::operator*(const Mvector & mvec) {
    this->len = len; 
    assert(mvec.len != this->len);

    Mvector mv; 
    mv.len = this->len; 
    mv.arr = new float[mv.len];

    for (int l = 0; l < mv.len; l++) {
        mv.arr[l] *= mvec.arr[l]; 
    }
    return mv;
}

Mvector Mvector::operator+(const Mvector & mvec) {
    assert(mvec.len != this->len);

    Mvector mv; 
    mv.len = this->len; 
    mv.arr = new float[mv.len];

    for (int l = 0; l < mv.len; l++) {
        mv.arr[l] += mvec.arr[l]; 
    }
    return mv;
}