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
    private async void SelectPathButton_Click(object sender, RoutedEventArgs e)
    {
        // Create a folder picker
        FolderPicker folderPicker = new Windows.Storage.Pickers.FolderPicker();

        var hwnd = WinRT.Interop.WindowNative.GetWindowHandle(this);
        WinRT.Interop.InitializeWithWindow.Initialize(folderPicker, hwnd);
        folderPicker.FileTypeFilter.Add("*");
        StorageFolder folder = await folderPicker.PickSingleFolderAsync();
        if (folder != null)
            FolderPathTextBox_Page1_1.Text = folder.Path;
        else
            FolderPathTextBox_Page1_1.Text = "";
    }
    public async void SelectFileButton_Click(object sender, RoutedEventArgs e)
    {
        FileOpenPicker filePicker = new FileOpenPicker();
        var hwnd = WinRT.Interop.WindowNative.GetWindowHandle(this);
        WinRT.Interop.InitializeWithWindow.Initialize(filePicker, hwnd);
        filePicker.FileTypeFilter.Add("*");
        var file = await filePicker.PickSingleFileAsync();
        if (file != null)
            DataFilePathTextBox_Page2_1.Text = file.Path;
        else
            DataFilePathTextBox_Page2_1.Text = "";
    }
}
