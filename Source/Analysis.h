#ifndef __Analysis__
#define __Analysis__

#include <stdio.h>
#include <objbase.h>
#include <string.h>
#include <math.h>

#define TRUE 1
#define FALSE 0
#define EPSILON 0.000001

class Matrix
{
private:
    int _rows;
    int _columns;
    double* _data;

public:
    Matrix();
    Matrix(Matrix& Ma);
    Matrix(int row, int column);
    Matrix(int row, int column, double diagonal);
    ~Matrix();
    inline int GetRows() { return _rows; };
    inline int GetColumns() { return _columns; };
    void Set(int row, int column, double value);
    double& Get(int row, int column);
    void Fill(double value);
    void Add(Matrix& a, Matrix& b);
    void Mul(Matrix& a, Matrix& b);
    void Sub(Matrix& a, Matrix& b);
    double& operator()(int row, int column);
    Matrix& operator=(Matrix& NewMatrix);
    Matrix& operator+=(Matrix& Summand);
    Matrix& operator-=(Matrix& Summand);
    Matrix& operator*=(Matrix& Faktor);
    void operator*=(double Scalar);

    void Inverse();
    double Determinant();
    void Solve(Matrix& matrix, Matrix& result);
};

struct Node
{
    struct Node* Next;
    struct Node* Prev;
    void* Data;
};

class List
{
private:
    struct Node* _root;
    struct Node* _current;
    Node* CreateNode(struct Node* previous, struct Node* next, void* data);

public:
    List();
    ~List();
    inline int IsEmpty() { return _root == NULL; };
    inline void* GetItem() { return _current->Data; };
    void Insert(void* item);
    void Add(void* item);
    void Remove();
    int Next();
    int Prev();
    void Reset();
    void Clear();
};

class SupportNode
{
protected:
    double _position;
    Matrix _state;
    Matrix _force;

public:
    virtual int IsSupport() { return TRUE; }
    inline double GetPosition() { return _position; };
    inline Matrix& GetState() { return _state; };
    Matrix& GetForce();
    void SetForce(Matrix& matrix);
    void GetKnownRows(int rows[2]);
    void GetUnknownRows(int rows[2]);
};

class FixedSupportNode : public SupportNode
{
public:
    FixedSupportNode(double StuetzStelle);
    int IsSupport() { return TRUE; }
};

class HingedSupportNode : public SupportNode
{
public:
    HingedSupportNode(double StuetzStelle);
    int IsSupport() { return TRUE; }
};

class FreeSupportNode : public SupportNode
{
public:
    FreeSupportNode(double StuetzStelle);
    int IsSupport() { return FALSE; }
};

class LoadNode
{
protected:
    double _start;
    double _value;
    double _length;
    Matrix _dummy;
    int _isSupportLoad;

public:
    LoadNode();

    inline double GetValue() { return _value; };
    inline double GetStart() { return _start; };
    inline double GetLength() { return _length; };

    void MarkSupportLoad() { _isSupportLoad = TRUE; }

    virtual double GetDistance(double start);
    virtual int IsLoadVector(double start, double length);
    virtual Matrix& GetLoadVector(double delta);
};

class PointLoadNode : public LoadNode
{
private:
    Matrix _vector;

public:
    PointLoadNode(double start, double value);
    double GetDistance(double start);
    int IsLoadVector(double start, double length);
    Matrix& GetLoadVector(double length);
};

class LineadDistributedLoadNode : public LoadNode
{
private:
    Matrix _vector;

public:
    LineadDistributedLoadNode(double start, double length, double value);
    double GetDistance(double start);
    int IsLoadVector(double start, double length);
    Matrix& GetLoadVector(double length);
};

struct Section
{
    double Start;
    double Length;
    double A0;
    double A1;
    double A2;
    double A3;
    double A4;
};


class Beam
{
private:
    double _length;
    double _EI;

public:
    Matrix _startState;
    Matrix _endState;
    List _loads;
    List _sections;
    List _shearForces;
    List _bendingMoments;
    List _displacements;

public:
    Beam();
    ~Beam();

private:
    void InsertSupport(SupportNode* load);
    void InsertLoad(LoadNode* load);
    double Factorial(int value);
    void ComputeSupportForces();
    void SetCurrentSupportForces(Matrix& forces, SupportNode* previous, Matrix& A, Matrix& L);
    double GetDistance(double start, double end, int& isPointLoad);
    void GetLoadVector(Matrix& L, double start, double distance);
    void GetMatrix(Matrix& A, double distance);

public:
    HRESULT __stdcall SetLength(double length);
    HRESULT __stdcall SetEI(double modulusOfElasticity, double momentOfInertia);
    HRESULT __stdcall CreateFixedSupport(double position);
    HRESULT __stdcall CreateHingedSupport(double position);
    HRESULT __stdcall CreateRollerSupport(double position);
    HRESULT __stdcall CreatePointLoad(double position, double value);
    HRESULT __stdcall CreateLinearDistributedLoad(double position, double value, double length);
    HRESULT __stdcall Analyse();
    HRESULT __stdcall GetNextSection(Section* shearForce, Section* bendingMoment, Section* displacement);
};

#endif
