Imports System
Imports Tinkerforge

Module ExampleCallback
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your UV Light Bricklet 2.0

    ' Callback subroutine for UV index callback
    Sub UVICB(ByVal sender As BrickletUVLightV2, ByVal uvi As Integer)
        Console.WriteLine("UV Index: " + (uvi/10.0).ToString())
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim uvl As New BrickletUVLightV2(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Register UV index callback to subroutine UVICB
        AddHandler uvl.UVICallback, AddressOf UVICB

        ' Set period for UVI callback to 1s (1000ms) without a threshold
        uvl.SetUVICallbackConfiguration(1000, False, "x"C, 0, 0)

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
