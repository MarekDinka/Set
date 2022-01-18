#include <exception>
#include <string>

class SomethingWentBadException : public std::exception {
public:
    const char* what () const throw () {
        return "Something happened";
    }
};

class WrongTypeException : public std::exception {
public:
    const char* what() const noexcept override
    {
        return "Wrong type!";
    }
};

class TypeNotAcceptedException : public std::exception {
public:
    const char* what() const noexcept override
    {
        return "Type not accepted by set!";
    }
};

class OccupiedSpaceException : public std::exception {
public:
    const char* what() const noexcept override
    {
        return "Next is not empty!";
    }
};

class IndexOutOfRangeException : public std::exception {
public:
    const char* what() const noexcept override
    {
        return "Index is out of range!";
    }
};

class ValueNotFoundException : public std::exception {
public:
    const char* what() const noexcept override
    {
        return "Value not found!";
    }
};

class EmptySetException : public std::exception {
public:
    const char* what() const noexcept override
    {
        return "Set is empty!";
    }
};