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
    [DllImport("CS调用链接库.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr Signal_Function(string path);
    //Page_1
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
        FloorSignalTextBox.Text = b[0];
        DoorSignalTextBox.Text = b[1];
        RunSignalTextBox.Text = b[2];
    }
    //Page_2
    public async void SelectFileButton_Click(object sender, RoutedEventArgs e)
    {
        FileOpenPicker filePicker = new FileOpenPicker();
        var hwnd = WinRT.Interop.WindowNative.GetWindowHandle(this);
        WinRT.Interop.InitializeWithWindow.Initialize(filePicker, hwnd);
        filePicker.FileTypeFilter.Add("*");
        var file = await filePicker.PickSingleFileAsync();
        if (file != null)
            DataFilePathTextBox.Text = file.Path;
        else
            DataFilePathTextBox.Text = "";
    }
    public void EnterButton_Click_Page2_1(object sender, RoutedEventArgs e)
    {

    }
    public async void A1ComboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
    {
        if (DataSelect_value[(string)A1ComboBox.SelectedItem] != -1 )
        {
            if (A1ComboBox.SelectedIndex == A2ComboBox.SelectedIndex || A1ComboBox.SelectedIndex == A3ComboBox.SelectedIndex)
            {
                ContentDialog DataSelect_warning = new ContentDialog
                {
                    Title = "WARNING",
                    Content = "Do Not Choose the Same Number !",
                    CloseButtonText = "OK",
                };
                DataSelect_warning.XamlRoot = this.Content.XamlRoot;
                await DataSelect_warning.ShowAsync();
                A1ComboBox.SelectedIndex = 0;
            }
        }
    }
    public async void A2ComboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
    {
        if (DataSelect_value[A2ComboBox.SelectedItem.ToString()] != -1)
        {
            if (A2ComboBox.SelectedIndex == A1ComboBox.SelectedIndex || A2ComboBox.SelectedIndex == A3ComboBox.SelectedIndex)
            {
                ContentDialog DataSelect_warning = new ContentDialog
                {
                    Title = "WARNING",
                    Content = "Do Not Choose the Same Number !",
                    CloseButtonText = "OK",
                };
                DataSelect_warning.XamlRoot = this.Content.XamlRoot;
                await DataSelect_warning.ShowAsync();
                A2ComboBox.SelectedIndex = 0;
            }
        }
    }
    public async void A3ComboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
    {
        if (DataSelect_value[A3ComboBox.SelectedItem.ToString()] != -1)
        {
            if (A3ComboBox.SelectedIndex == A1ComboBox.SelectedIndex || A3ComboBox.SelectedIndex == A2ComboBox.SelectedIndex)
            {
                ContentDialog DataSelect_warning = new ContentDialog
                {
                    Title = "WARNING",
                    Content = "Do Not Choose the Same Number !",
                    CloseButtonText = "OK",
                };
                DataSelect_warning.XamlRoot = this.Content.XamlRoot;
                await DataSelect_warning.ShowAsync();
                A3ComboBox.SelectedIndex = 0;
            }
        }
    }
    
}
