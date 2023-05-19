using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using System;
using Windows.Storage.Pickers;
using Windows.Storage;
using System.Runtime.InteropServices;
using System.IO;

namespace App;

public partial class MainWindow
{
    [DllImport("Resources\\CS调用链接库.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr Signal_Function(string path);
}
