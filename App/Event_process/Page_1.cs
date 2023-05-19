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
    //Page_1
    public async void EnterButton_Click_Page1_1(object sender, RoutedEventArgs e)
    {
        string path = FolderPathTextBox_Page1_1.Text;
        if(!Directory.Exists(path))
        {
            ContentDialog DataSelect_warning = new ContentDialog
            {
                Title = "WARNING",
                Content = "Path Does Not Exist !",
                CloseButtonText = "OK",
            };
            DataSelect_warning.XamlRoot = this.Content.XamlRoot;
            await DataSelect_warning.ShowAsync();
            A1ComboBox.SelectedIndex = 0;
            FolderPathTextBox_Page1_1.Text = null;
            return;
        }
        IntPtr a = Signal_Function(path);
        string[] b=Marshal.PtrToStringAnsi(a).Split("{Splite}");
        string[] floor = b[0].Split("{floor}");
        FloorSignalTextBox_IDaBit.Text = floor[0];
        FloorSignalTextBox_Pos.Text = floor[1];
        FloorSignalTextBox_Range.Text = floor[2];
        FloorSignalTextBox_Increment.Text = floor[3];
        DoorSignalTextBox.Text = b[1];
        RunSignalTextBox.Text = b[2];
    }    
}
