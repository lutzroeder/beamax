#include <stdio.h>
#include <iostream>
#include <math.h>
#include "stdafx.h"
#include "Analysis.h"

Matrix::Matrix()
{
    _rows = 0;
    _columns = 0;
    _data = NULL;
}

Matrix::Matrix(int rows, int columns)
{
    _rows = rows;
    _columns = columns;
    _data = new double[rows * columns];
    Fill(0.0);
}

Matrix::Matrix(int rows, int columns, double diagonal)
{
    _rows = rows;
    _columns = columns;
    _data = new double[rows * columns];
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            Set(i, j, (i == j) ? diagonal : 0.0);
        }
    }
}

Matrix::Matrix(Matrix& matrix)
{
    _rows = matrix._rows;
    _columns = matrix._columns;
    int size = _rows * _columns;
    _data = new double[size];
    for (int i = 0; i < size; i++)
    {
        _data[i] = matrix._data[i];
    }
}

Matrix::~Matrix()
{
    if (_data != NULL)
    {
        delete _data;
        _data = NULL;
    }
}

void Matrix::Fill(double value)
{
    int size = _rows * _columns;
    for (int i = 0; i < size; i++)
    {
        _data[i] = value;
    }
}

void Matrix::Set(int row, int column, double value)
{
    _data[row * _columns + column] = value;
}

double& Matrix::Get(int row, int column)
{
    return _data[row * _columns + column];
}

Matrix& Matrix::operator=(Matrix& matrix)
{
    if (_data != NULL)
    {
        delete _data;
        _data = NULL;
    }
    _rows = matrix._rows;
    _columns = matrix._columns;
    int size = _rows * _columns;
    _data = new double[size];
    for (int i = 0; i < size; i++)
    {
        _data[i] = matrix._data[i];
    }
    return *this;
}

double& Matrix::operator() (int row, int column)
{
    return Get(row, column);
}

void Matrix::Add(Matrix& a, Matrix& b)
{
    if (a.GetRows() == b.GetRows() && a.GetColumns() == b.GetColumns())
    {
        if (_data != NULL)
        {
            delete _data;
            _data = NULL;
        }
        _rows = a.GetRows();
        _columns = a.GetColumns();
        _data = new double[_rows * _columns];
        for (int i = 0; i < _rows; i++)
        {
            for (int j = 0; j < _columns; j++)
            {
                Set(i, j, a(i, j) + b(i, j));
            }
        }
    }
}

void Matrix::Sub(Matrix& a, Matrix& b)
{
    if (a.GetRows() == b.GetRows() && a.GetColumns() == b.GetColumns())
    {
        if (_data != NULL)
        {
            delete _data;
            _data = NULL;
        }
        _rows = a.GetRows();
        _columns = a.GetColumns();
        _data = new double[_rows * _columns];
        for (int i = 0; i < _rows; i++)
        {
            for (int j = 0; j < _columns; j++)
            {
                Set(i, j, a(i, j) - b(i, j));
            }
        }
    }
}

void Matrix::Mul(Matrix& a, Matrix& b)
{
    if (a.GetColumns() == b.GetRows())
    {
        _rows = a.GetRows();
        _columns = b.GetColumns();
        int length = a.GetColumns();
        if (_data != NULL)
        {
            delete _data;
            _data = NULL;
        }
        _data = new double[_rows * _columns];
        for (int i = 0; i < _rows; i++)
        {
            for (int j = 0; j < _columns; j++)
            {
                double value = 0;
                for (int k = 0; k < length; k++)
                {
                    value += a(i, k) * b(k, j);
                }
                Set(i, j, value);
            }
        }
    }
}

Matrix& Matrix::operator+=(Matrix& b)
{
    Matrix copy(*this);
    Add(copy, b);
    return *this;
}

Matrix& Matrix::operator-=(Matrix& b)
{
    Matrix copy(*this);
    Sub(copy, b);
    return *this;
}

Matrix& Matrix::operator*=(Matrix& b)
{
    Matrix copy(*this);
    Mul(copy, b);
    return *this;
}

void Matrix::operator*=(double scalar)
{
    for (int i = 0; i < GetRows(); i++)
    {
        for (int j = 0; j < GetColumns(); j++)
        {
            Set(i, j, Get(i, j) * scalar);
        }
    }
}

void Matrix::Solve(Matrix& matrix, Matrix& result)
{
    double d = matrix.Determinant();
    if (fabs(d) > 0.00000000000001)
    {
        Matrix arg(2, 2);
        double d1;
        double d2;
        for (int n = 0; n < 2; n++)
        {
            arg = matrix;
            for (int i = 0; i < 2; i++)
            {
                arg(i, n) = result(i, 0);
            }
            if (n == 0)
            {
                d1 = arg.Determinant();
            }
            else
            {
                d2 = arg.Determinant();
            }
        }

        Set(0, 0, d1 / d);
        Set(1, 0, d2 / d);
    }
}

double Matrix::Determinant()
{
    if (_rows != 2 || _columns != 2)
    {
        throw;
    }
    return Get(0, 0) * Get(1, 1) - Get(1, 0) * Get(0, 1);
}

void Matrix::Inverse()
{
    if (_rows != 2 || _columns != 2)
    {
        throw;
    }
    Matrix result(2, 1);
    Matrix copy(*this);
    Matrix unk1(2, 1);
    Matrix unk2(2, 1);
    result(0, 0) = 1;
    result(1, 0) = 0;
    unk1.Solve(copy, result);
    result(0, 0) = 0;
    result(1, 0) = 1;
    unk2.Solve(copy, result);
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            Set(i, j, j == 0 ? unk1(i, 0) : unk2(i, 0));
        }
    }
}

void List::Add(void* item)
{
    _current = _current->Next = CreateNode(_current, _current->Next, item);
    if (_current->Next != NULL)
    {
        _current->Next->Prev = _current;
    }
}

void List::Remove()
{
    struct Node* current;
    if ((current = _current) != NULL)
    {
        struct Node* prev = _current->Prev;
        struct Node* next = _current->Next;

        if (Prev() != TRUE)
        {
            _root = _current->Next;
            if (Next() == TRUE)
            {
                _root->Prev = NULL;
            }
            else
            {
                _current = NULL;
            }
        }
        else
        {
            prev->Next = next;
            if (next != NULL)
            {
                next->Prev = prev;
            }
        }
        delete current->Data;
        delete current;
    }
}

void List::Clear()
{
    Reset();
    while (!IsEmpty())
    {
        Remove();
    }
}

int List::Prev()
{
    if (_current->Prev == NULL)
    {
        return FALSE;
    }
    _current = _current->Prev;
    return TRUE;
}

void List::Reset()
{
    _current = _root;
}


Node* List::CreateNode(struct Node* previous, struct Node* next, void* data)
{
    struct Node* node = new Node;
    node->Prev = previous;
    node->Next = next;
    node->Data = data;
    return node;
}

void List::Insert(void* item)
{
    if (_root == NULL)
    {
        _current = _root = CreateNode(NULL, NULL, item);
    }
    else
    {
        _current = _root = _root->Prev = CreateNode(NULL, _root, item);
    }
}

List::List()
{
    _root = NULL;
    _current = NULL;
}

List::~List()
{
    Reset();
    while (!IsEmpty())
    {
        Remove();
    }
}

int List::Next()
{
    if (_current->Next == NULL)
    {
        return FALSE;
    }
    else
    {
        _current = _current->Next;
    }
    return TRUE;
}

void SupportNode::SetForce(Matrix& matrix)
{
    for (int i = 0; i < 4; i++)
    {
        _force(i, 0) = matrix(i, 0);
    }
}


Matrix& SupportNode::GetForce()
{
    return _force;
}

void SupportNode::GetKnownRows(int rows[2])
{
    int j = 0;
    for (int i = 0; i < 4; i++)
    {
        if ((fabs(_state(i, 0)) < EPSILON) && (fabs(_state(i, 1)) < EPSILON))
        {
            rows[j] = i;
            j = 1;
        }
    }
}

void SupportNode::GetUnknownRows(int rows[2])
{
    int j = 0;
    for (int i = 0; i < 4; i++)
    {
        if ((fabs(_state(i, 0)) > EPSILON) || (fabs(_state(i, 1)) > EPSILON))
        {
            rows[j] = i;
            j = 1;
        }
    }
}

HingedSupportNode::HingedSupportNode(double position)
{
    _position = position;
    _force = Matrix(4, 1);
    _state = Matrix(4, 2);
    _state(1, 0) = 1;
    _state(3, 1) = 1;
}

FixedSupportNode::FixedSupportNode(double position)
{
    _position = position;
    _force = Matrix(4, 1);
    _state = Matrix(4, 2);
    _state(2, 0) = 1;
    _state(3, 1) = 1;
}

FreeSupportNode::FreeSupportNode(double position)
{
    _position = position;
    _force = Matrix(4, 1);
    _state = Matrix(4, 2);
    _state(0, 0) = 1;
    _state(1, 1) = 1;
}

double LoadNode::GetDistance(double)
{
    return 0.0;
}

Matrix& LoadNode::GetLoadVector(double)
{
    return _dummy;
}

LoadNode::LoadNode()
{
    _isSupportLoad = FALSE;
    _dummy = Matrix(4, 1);
}

int LoadNode::IsLoadVector(double, double)
{
    return FALSE;
}

PointLoadNode::PointLoadNode(double start, double value)
{
    _value = value;
    _start = start;
    _vector = Matrix(4, 1);
}

double PointLoadNode::GetDistance(double start)
{
    if (!_isSupportLoad)
    {
        if (fabs(start - GetStart()) < EPSILON)
        {
            return 0.0;
        }
        if (start - GetStart() < EPSILON)
        {
            return GetStart() - start;
        }
    }
    return (double)-1;
}

Matrix& PointLoadNode::GetLoadVector(double)
{
    if (!_isSupportLoad)
    {
        _vector(3, 0) = GetValue() * (-1.0);
    }
    return _vector;
}

int PointLoadNode::IsLoadVector(double start, double length)
{
    if (!_isSupportLoad && length < EPSILON)
    {
        if (fabs(start - GetStart()) < EPSILON)
        {
            return TRUE;
        }
    }
    return FALSE;
}

LineadDistributedLoadNode::LineadDistributedLoadNode(double start, double length, double value)
{
    _start = start;
    _length = length;
    _value = value;
    _vector = Matrix(4, 1);
}

double LineadDistributedLoadNode::GetDistance(double start)
{
    if (fabs(start - GetStart()) < EPSILON)
    {
        return GetLength();
    }
    double end = GetStart() + GetLength();
    if (fabs(start - end) < EPSILON || start > end)
    {
        return (double)-1;
    }
    if (start < GetStart())
    {
        return GetStart() - start;
    }
    return end - start;
}

Matrix& LineadDistributedLoadNode::GetLoadVector(double length)
{
    double p = length;
    _vector(3, 0) = GetValue() * p * (-1.0);
    p *= length;
    _vector(2, 0) = GetValue() * p * (-1.0) / 2;
    p *= length;
    _vector(1, 0) = GetValue() * p / 6;
    p *= length;
    _vector(0, 0) = GetValue() * p / 24;
    return _vector;
}

int LineadDistributedLoadNode::IsLoadVector(double start, double length)
{
    if (length > EPSILON)
    {
        if (((GetStart() - EPSILON) <= start) && (start <= (GetStart() + GetLength() - EPSILON)))
        {
            return TRUE;
        }
    }
    return FALSE;
}

void Beam::InsertSupport(SupportNode* support)
{
    int Eingefuegt = FALSE;
    _sections.Reset();
    if (_sections.IsEmpty())
    {
        _sections.Insert(support);
        Eingefuegt = TRUE;
    }
    while (!Eingefuegt)
    {
        if (fabs(((SupportNode*)_sections.GetItem())->GetPosition() - support->GetPosition()) < EPSILON) {
            if (_sections.Prev() == FALSE)
                _sections.Insert(support);
            else
                _sections.Add(support);
            _sections.Remove();
            Eingefuegt = TRUE;
        }
        else if (((SupportNode*)_sections.GetItem())->GetPosition() > support->GetPosition()) {
            if (_sections.Prev() == FALSE)
                _sections.Insert(support);
            else
                _sections.Add(support);

            Eingefuegt = TRUE;
        }
        else
        {
            if (_sections.Next() == FALSE) {
                _sections.Add(support);
                Eingefuegt = TRUE;
            }
        }
    }
}

void Beam::InsertLoad(LoadNode* load)
{
    int success = FALSE;
    _loads.Reset();
    if (_loads.IsEmpty())
    {
        _loads.Insert(load);
        success = TRUE;
    }
    while (!success)
    {
        if ((((LoadNode *)_loads.GetItem())->GetStart()) > (load->GetStart()))
        {
            if (_loads.Prev() == FALSE)
            {
                _loads.Insert(load);
            }
            else
            {
                _loads.Add(load);
            }
            success = TRUE;
        }
        else
        {
            if (_loads.Next() == FALSE) {
                _loads.Add(load);
                success = TRUE;
            }
        }
    }
}

void Beam::GetMatrix(Matrix& A, double distance)
{
    for (int i = 0; i < 4; i++)
    {
        double p = 1;
        for (int j = i; j < 4; j++)
        {
            A(i, j) = (i == j) ? 1.0 : p / Factorial(j - i);
            p *= distance;
            if ((i < 2) && (j > 1))
            {
                A(i, j) /= (-1 * _EI);
            }
        }
    }
}

double Beam::GetDistance(double start, double end, int &pointLoad)
{
    double distance = end - start;
    double currentDistance = 0;
    int next = TRUE;
    _loads.Reset();
    if (_loads.IsEmpty())
    {
        next = FALSE;
    }
    while (next)
    {
        currentDistance = ((LoadNode *)_loads.GetItem())->GetDistance(start);
        if (!(fabs(currentDistance + 1) < EPSILON))
        {
            if (currentDistance < distance)
            {
                if (currentDistance < EPSILON)
                {
                    if (pointLoad == TRUE)
                    {
                        distance = 0.0;
                    }
                }
                else
                {
                    distance = currentDistance;
                }
            }
        }
        if (_loads.Next() == FALSE)
        {
            next = FALSE;
        }
    }
    return distance;
}

void Beam::GetLoadVector(Matrix& L, double start, double length)
{
    _loads.Reset();
    if (!_loads.IsEmpty())
    {
        Matrix NewLastVektor(4, 1);
        do
        {
            LoadNode* p = (LoadNode*)_loads.GetItem();
            if (p->IsLoadVector(start, length))
            {
                NewLastVektor = p->GetLoadVector(length);
                NewLastVektor(0, 0) /= _EI;
                NewLastVektor(1, 0) /= _EI;
                L += NewLastVektor;
            }
        }
        while (_loads.Next());
    }
}

void Beam::SetCurrentSupportForces(Matrix& forces, SupportNode* previous, Matrix& A, Matrix& L)
{
    SupportNode* current = (SupportNode*)_sections.GetItem();
    Matrix matrix = Matrix(A);
    Matrix loadVector = L;
    Matrix _z1 = Matrix(2, 2);
    Matrix _l1 = Matrix(2, 1);
    double start = previous->GetPosition();
    double end = current->GetPosition();
    int isPointLoad = FALSE;
    double distance;
    while (start < end)
    {
        distance = GetDistance(start, end, isPointLoad);
        isPointLoad = distance < EPSILON ? FALSE : TRUE;
        if (distance > EPSILON)
        {
            Matrix A(4, 4);
            GetMatrix(A, distance);
            Matrix copy(matrix);
            matrix.Mul(A, copy);
            copy = loadVector;
            loadVector.Mul(A, copy);
        }
        GetLoadVector(loadVector, start, distance);
        start += distance;
    }
    GetLoadVector(loadVector, start, 0.0);
    if (_sections.Next())
    {
        int zeilebe[2];
        int zeileun[2];
        previous->GetKnownRows(zeilebe);
        previous->GetUnknownRows(zeileun);
        zeilebe[0] = 0;
        zeilebe[1] = 1;
        zeileun[0] = 2;
        zeileun[1] = 3;
        Matrix z1(2, 2);
        Matrix z2(2, 2);
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                z1(i, j) = matrix(zeilebe[i], j);
                z2(i, j) = matrix(zeileun[i], j);
            }
        }
        Matrix l1(2, 1);
        Matrix l2(2, 1);
        for (int i = 0; i < 2; i++)
        {
            l1(i, 0) = loadVector(zeilebe[i], 0);
            l2(i, 0) = loadVector(zeileun[i], 0);
        }
        z1.Inverse();
        Matrix kk(2, 2);
        Matrix lk(2, 1);
        kk.Mul(z2, z1);
        lk.Mul(kk, l1);
        lk *= -1;
        lk += l2;
        _z1 = z1;
        _l1 = l1;
        Matrix transfer(4, 4, 1);
        L.Fill(0.0);
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                transfer(i + 2, j) = kk(i, j);
                L(i + 2, 0) = lk(i, 0);
            }
        }
        A.Mul(transfer, current->GetState());
        matrix = A;
        loadVector = L;
        SetCurrentSupportForces(forces, current, A, L);
        Matrix support(matrix);
        Matrix supportForces(4, 1);
        supportForces(3, 0) = forces(1, 0);
        support *= forces;
        support += loadVector;
        current->SetForce(supportForces);
        Matrix temp(2, 1);
        forces.Fill(0.0);
        for (int i = 0; i < 2; i++)
        {
            temp(i, 0) = support(i, 0);
        }
        temp -= _l1;
        forces.Mul(_z1, temp);
    }
    else
    {
        Matrix A(2, 2);
        Matrix result(2, 1);
        int zeilebe[2];
        Matrix support(matrix);
        current->GetKnownRows(zeilebe);
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                A(i, j) = matrix(zeilebe[i], j);
            }
            result(i, 0) = loadVector(zeilebe[i], 0);
        }
        result *= -1;
        forces.Solve(A, result);
        support *= forces;
        support += loadVector;
        _endState = support;
        _endState *= -1;
        for (int i = 0; i < 2; i++)
        {
            support(i, 0) = 0;
        }
        support(3, 0) *= -1;
        current->SetForce(support);
    }
}

Beam::Beam()
{
    _startState = Matrix(4, 1);
    _endState = Matrix(4, 1);
}

Beam::~Beam()
{
    _loads.Clear();
    _sections.Clear();
}

double Beam::Factorial(int value)
{
    if (value <= 1)
    {
        return (double)1;
    }
    return (double)value * Factorial(value - 1);
}

void Beam::ComputeSupportForces()
{
    SupportNode* current;
    Matrix A(4, 2);
    Matrix L(4, 1);
    Matrix forces(2, 1);
    Matrix support(4, 1);
    int zeileun[2];
    LoadNode* last;
    _sections.Reset();
    do {
        current = ((SupportNode*)_sections.GetItem());
        if (current->IsSupport())
        {
            if (!_loads.IsEmpty())
            {
                double position = current->GetPosition();
                _loads.Reset();
                do
                {
                    last = ((LoadNode *)_loads.GetItem());
                    if (last->GetLength() == 0.0 && fabs(last->GetStart() - position) < EPSILON)
                    {
                        last->MarkSupportLoad();
                    }
                }
                while (_loads.Next());
            }
        }
    }
    while (_sections.Next());

    _sections.Reset();

    if (!_sections.IsEmpty())
    {
        current = ((SupportNode*)_sections.GetItem());
        A = current->GetState();
        if (_sections.Next())
        {
            SetCurrentSupportForces(forces, current, A, L);
        }
        current->GetUnknownRows(zeileun);
        for (int i = 0; i < 2; i++)
        {
            if (zeileun[i] > 1)
            {
                support(zeileun[i], 0) = forces(i, 0);
            }
        }
        current->SetForce(support);
        for (int i = 0; i < 2; i++)
        {
            _startState(zeileun[i], 0) = forces(i, 0);
        }
    }
}

HRESULT __stdcall Beam::SetLength(double length)
{
    _length = length;
    return S_OK;
}

HRESULT __stdcall Beam::SetEI(double modulusOfElasticity, double momentOfInertia)
{
    _EI = modulusOfElasticity * momentOfInertia;
    return S_OK;
}

HRESULT Beam::CreateFixedSupport(double position)
{
    FixedSupportNode* obj = new FixedSupportNode(position);
    InsertSupport(obj);
    return S_OK;
}

HRESULT Beam::CreateHingedSupport(double position)
{
    HingedSupportNode* obj = new HingedSupportNode(position);
    InsertSupport(obj);
    return S_OK;
}

HRESULT Beam::CreateRollerSupport(double position)
{
    HingedSupportNode* obj = new HingedSupportNode(position);
    InsertSupport(obj);
    return S_OK;
}

HRESULT Beam::CreatePointLoad(double position, double value)
{
    if (position >= EPSILON)
    {
        PointLoadNode* obj = new PointLoadNode(position, value);
        InsertLoad(obj);
    }
    return S_OK;
}

HRESULT Beam::CreateLinearDistributedLoad(double position, double value, double length)
{
    LineadDistributedLoadNode* obj = new LineadDistributedLoadNode(position, length, value);
    InsertLoad(obj);
    return S_OK;
}

HRESULT Beam::Analyse()
{
    SupportNode* support;
    BOOL left = FALSE;
    BOOL right = FALSE;
    _sections.Reset();
    if (!_sections.IsEmpty())
        do
        {
            support = (SupportNode*)_sections.GetItem();
            if (support != NULL)
            {
                if (support->GetPosition() == 0)
                    left = TRUE;
                if (support->GetPosition() == _length)
                    right = TRUE;
            }
        }
        while (_sections.Next());
        if (!left)
        {
            InsertSupport(new FreeSupportNode(0));
        }
        if (!right)
        {
            InsertSupport(new FreeSupportNode(_length));
        }
        Section* Q;
        Section* M;
        Section* W;
        ComputeSupportForces();
        _sections.Reset();
        Matrix state1(4, 1);
        Matrix state2(4, 1);
        Matrix A(4, 4);
        Matrix L(4, 1);
        double position = 0;
        int isPointLoad = TRUE;
        if (!_sections.IsEmpty())
        {
            do
            {
                double end = ((SupportNode*)_sections.GetItem())->GetPosition();
                while (position < end)
                {
                    double distance = GetDistance(position, end, isPointLoad);
                    isPointLoad = distance < EPSILON ? FALSE : TRUE;
                    if (distance > EPSILON)
                    {
                        A = Matrix(4, 4);
                        GetMatrix(A, distance);
                        A *= state2;
                        state2 = A;
                    }
                    L.Fill(0.0);
                    GetLoadVector(L, position, distance);
                    state2 += L;
                    if (distance > EPSILON)
                    {
                        // shear force
                        Q = new Section;
                        Q->Start = position;
                        Q->Length = distance;
                        Q->A4 = 0;
                        Q->A3 = 0;
                        Q->A2 = 0;
                        Q->A1 = (state2(3, 0) - state1(3, 0)) / distance;
                        Q->A0 = state1(3, 0);

                        // bending moment
                        M = new Section;
                        M->Start = position;
                        M->Length = distance;
                        M->A4 = 0;
                        M->A3 = 0;
                        M->A2 = (state2(3, 0) - state1(3, 0)) / (2 * distance);
                        M->A1 = state1(3, 0);
                        M->A0 = state1(2, 0);

                        // displacement
                        W = new Section;
                        double Q1 = state1(3, 0) * (-1);
                        double M1 = state1(2, 0) * (-1);
                        double phi1 = state1(1, 0) * _EI;
                        double w1 = state1(0, 0) * _EI;
                        double Q2 = state2(3, 0) * (-1);
                        W->Start = position;
                        W->Length = distance;
                        W->A4 = ((Q2 - Q1) / (24 * distance)) / _EI;
                        W->A3 = (Q1 / 6) / _EI;
                        W->A2 = (M1 / 2) / _EI;
                        W->A1 = phi1 / _EI;
                        W->A0 = w1 / _EI;
                        _shearForces.Insert(Q);
                        _bendingMoments.Insert(M);
                        _displacements.Insert(W);
                    }
                    position += distance;
                    state1 = state2;
                }

                if (position < EPSILON)
                {
                    state2 = _startState;
                }
                else
                {
                    if (fabs(position - _length) < EPSILON)
                    {
                        state2 += _endState;
                    }
                    else
                    {
                        SupportNode* support = (SupportNode*) _sections.GetItem();
                        state2 += support->GetForce();
                    }
                }
                state1 = state2;
            }
            while (_sections.Next());
        }
        _shearForces.Reset();
        _bendingMoments.Reset();
        _displacements.Reset();
        return S_OK;
}

HRESULT Beam::GetNextSection(Section* shearForce, Section* bendingMoment, Section* displacement)
{
    memcpy(shearForce, _shearForces.GetItem(), sizeof(Section));
    memcpy(bendingMoment, _bendingMoments.GetItem(), sizeof(Section));
    memcpy(displacement, _displacements.GetItem(), sizeof(Section));
    if (_shearForces.Next() && _bendingMoments.Next() && _displacements.Next())
    {
        return S_OK;
    }
    return S_FALSE;
}
