#include <iostream>
#include <vector>
#include <string>
#include <numeric>


class complex{
    float real;
    float imaginary;
public:
    complex(float realPart = 0.0, float imaginaryPart = 0.0) : real(realPart), imaginary(imaginaryPart){}

    float getReal(){
        return real;
    }
    float getIm(){
        return imaginary;
    }
    void set_real(float realPart){
        real = realPart;
    }
    void set_im(float imaginaryPart){
        imaginary = imaginaryPart;
    }
    void print() {
        std::cout << real;
        if (imaginary >= 0){
            std::cout << "+" << imaginary << "i";
        }else{
            std::cout << imaginary << "i";
        }
    }
    complex add(complex& other){
        complex result(*this);
        result.real += other.real;
        result.imaginary += other.imaginary;
        return result;
    }
};

class Student {

    std::string name_;
    std::string surname_;
    int albumnumber_;
    std::vector<float> grades;

public:

    void set_name(std::string name){
        name_ = name;
    }
    std::string name() {
        return name_;
    }

    void set_albumnumber(int albumnumber){
        if (albumnumber >= 10000 && albumnumber <= 999999){
            albumnumber_ = albumnumber;
        }else{
            std::cout << "Invalid Album number. Entry was not created" ;
        }
    }

    void set_surname(std::string surname){
        surname_ = surname;
    }
    std::string surname() {
        return surname_;
    }

    bool add_grade(float grade) {
        if (grade >= 2.0 && grade <= 5.0) {
            // The grade is valid; let's add it and return true
            grades.push_back(grade);
            return true;
        }
        // The grade is invalid; let's return false
        return false;
    }


    void student_summary()
    {
        std::cout << name_ << "" << surname_ ;
        for(auto grade : grades)
        {
            std::cout <<" " << grade;
        }
    }

    float mean_grade() {
        float sum = std::accumulate(grades.begin(), grades.end(), 0.0f);
        return sum / grades.size();
    }

    bool passed(std::vector<float> grades){
        int count = 0;
        for(float grade : grades){
            if (grade == 2.0){
                count ++;
            }
            if (count > 1){
                return false;
            }
        }
        return true;
    }
};

int main()
{
    complex a(1.0, -2.0); // creates 1-2i
    complex b(3.14); // creates 3.14

    b.set_im(-5);

    complex c = a.add(b);

    c.print(); // prints 4.14-7i
    return 0;
}
