Imports System
Imports Tinkerforge

Module ExampleSimple
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your UV Light Bricklet 2.0

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim uvl As New BrickletUVLightV2(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Get current UV-A
        Dim uva As Integer = uvl.GetUVA()
        Console.WriteLine("UV-A: " + (uva/10.0).ToString() + " mW/m²")

        ' Get current UV-B
        Dim uvb As Integer = uvl.GetUVB()
        Console.WriteLine("UV-B: " + (uvb/10.0).ToString() + " mW/m²")

        ' Get current UV index
        Dim uvi As Integer = uvl.GetUVI()
        Console.WriteLine("UV Index: " + (uvi/10.0).ToString())

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
