object GPage: TGPage
  Left = 227
  Top = 122
  Width = 544
  Height = 375
  Caption = 'GPage'
  Color = clActiveBorder
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnMouseDown = FormMouseDown
  OnMouseMove = FormMouseMove
  OnMouseUp = FormMouseUp
  OnPaint = FormPaint
  PixelsPerInch = 96
  TextHeight = 13
  object PopupMenu1: TPopupMenu
    Left = 96
    Top = 24
    object OpenFormat1: TMenuItem
      Caption = 'Open Format'
    end
    object Addgamma1: TMenuItem
      Caption = 'Add gamma'
    end
  end
end
