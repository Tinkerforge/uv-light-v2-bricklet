Imports System
Imports Tinkerforge

Module ExampleThreshold
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your UV Light Bricklet 2.0

    ' Callback subroutine for UV index callback
    Sub UVICB(ByVal sender As BrickletUVLightV2, ByVal uvi As Integer)
        Console.WriteLine("UV Index: " + (uvi/10.0).ToString())
        Console.WriteLine("UV index > 3. Use sunscreen!")
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim uvl As New BrickletUVLightV2(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Register UV index callback to subroutine UVICB
        AddHandler uvl.UVICallback, AddressOf UVICB

        ' Configure threshold for UV index "greater than 3"
        ' with a debounce period of 1s (1000ms)
        uvl.SetUVICallbackConfiguration(1000, False, ">"C, 3*10, 0)

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
