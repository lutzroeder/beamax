
#include "stdafx.h"
#include "Dialog.h"

void CreateBeamDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_BEAM_LENGTH, _length);
}

void CreateBeamDialog::SetLength(double value)
{
    _length = value;
}

double CreateBeamDialog::GetLength()
{
    return _length;
}

void PropertiesDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_MODULUS_OF_ELASTICITY, _modulusOfElasticity);
    DDX_Text(pDX, IDC_MOMENT_OF_INERTIA, _momentOfInertia);
    DDX_Text(pDX, IDC_AXIAL_AREA, _axialArea);
}

void PropertiesDialog::SetModulusOfElasticity(double value)
{
    _modulusOfElasticity = value;
}

double PropertiesDialog::GetModulusOfElasticity()
{
    return _modulusOfElasticity;
}

void PropertiesDialog::SetMomentOfInertia(double value)
{
    _momentOfInertia = value;
}

double PropertiesDialog::GetMomentOfInertia()
{
    return _momentOfInertia;
}

void PropertiesDialog::SetAxialArea(double value)
{
    _axialArea = value;
}

double PropertiesDialog::GetAxialArea()
{
    return _axialArea;
}

BEGIN_MESSAGE_MAP(FixedSupportDialog, CDialog)
    ON_COMMAND(IDC_POSITION_LEFT, OnPositionStart)
    ON_COMMAND(IDC_POSITION_RIGHT, OnPositionEnd)
END_MESSAGE_MAP()

BOOL FixedSupportDialog::OnInitDialog()
{
    if (_position == TRUE)
        ((CButton *)GetDlgItem(IDC_POSITION_LEFT))->SetCheck(1);
    else
        ((CButton *)GetDlgItem(IDC_POSITION_RIGHT))->SetCheck(1);

    return TRUE;
}

void FixedSupportDialog::OnPositionStart()
{
    _position = TRUE;
}

void FixedSupportDialog::OnPositionEnd()
{
    _position = FALSE;
}

void FixedSupportDialog::SetPosition(BOOL value)
{
    _position = value;
}

BOOL FixedSupportDialog::GetPosition()
{
    return _position;
}

void HingedSupportDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_POSITION, _position);
}

void HingedSupportDialog::SetPosition(double value)
{
    _position = value;
}

double HingedSupportDialog::GetPosition()
{
    return _position;
}

void RollerSupportDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_POSITION, _position);
}

void RollerSupportDialog::SetPosition(double value)
{
    _position = value;
}

double RollerSupportDialog::GetPosition()
{
    return _position;
}

void PointLoadDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_POSITION, _position);
    DDX_Text(pDX, IDC_VALUE, _value);
}

void PointLoadDialog::SetPosition(double value)
{
    _position = value;
}

double PointLoadDialog::GetPosition()
{
    return _position;
}

void PointLoadDialog::SetValue(double value)
{
    _value = value;
}

double PointLoadDialog::GetValue()
{
    return _value;
}

void LinearDistributedLoadDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_POSITION, _position);
    DDX_Text(pDX, IDC_VALUE, _value);
    DDX_Text(pDX, IDC_LENGTH, _length);
}

void LinearDistributedLoadDialog::SetPosition(double value)
{
    _position = value;
}

double LinearDistributedLoadDialog::GetPosition()
{
    return _position;
}

void LinearDistributedLoadDialog::SetValue(double value)
{
    _value = value;
}

double LinearDistributedLoadDialog::GetValue()
{
    return _value;
}

void LinearDistributedLoadDialog::SetLength(double value)
{
    _length = value;
}

double LinearDistributedLoadDialog::GetLength()
{
    return _length;
}
