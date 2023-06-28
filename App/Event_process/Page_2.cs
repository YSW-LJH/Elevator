using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using System;
using System.Threading.Tasks;
using Task = System.Threading.Tasks.Task;

namespace App;

public partial class MainWindow
{
    //Page_2
    public void EnterButton_Click_Page2_1(object sender, RoutedEventArgs e)
    {

    }
    public void Select_Num_Changed(object sender, RoutedEventArgs e)
    {
        if (A1ComboBox == null)
            return;
        A1ComboBox.IsEnabled = Select_Num.Value > 0;
        A2ComboBox.IsEnabled = Select_Num.Value > 1;
        A3ComboBox.IsEnabled = Select_Num.Value > 2;
        A4ComboBox.IsEnabled = Select_Num.Value > 3;
    }
    public async void A1ComboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
    {
        if (A1ComboBox.SelectedIndex!=0)
            if (!await Check())
                A1ComboBox.SelectedIndex = 0;
    }
    public async void A2ComboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
    {
        if (A1ComboBox.SelectedIndex != 0)
            if (!await Check())
                A2ComboBox.SelectedIndex = 0;
    }
    public async void A3ComboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
    {
        if (A1ComboBox.SelectedIndex != 0)
            if (!await Check())
                A3ComboBox.SelectedIndex = 0;
    }
    public async void A4ComboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
    {
        if (A1ComboBox.SelectedIndex != 0)
            if (!await Check())
                A4ComboBox.SelectedIndex = 0;
    }
    async Task<bool> Check()
    {
        if (A1ComboBox.SelectedIndex < A2ComboBox.SelectedIndex || A2ComboBox.SelectedIndex == 0)
            if (A2ComboBox.SelectedIndex < A3ComboBox.SelectedIndex || A3ComboBox.SelectedIndex == 0)
                if (A3ComboBox.SelectedIndex < A4ComboBox.SelectedIndex || A4ComboBox.SelectedIndex == 0)
                    return true;
        ContentDialog DataSelect_warning = new ContentDialog
        {
            Title = "WARNING",
            Content = "配置错误 !",
            CloseButtonText = "OK",
            XamlRoot = this.Content.XamlRoot
        };
        await DataSelect_warning.ShowAsync();
        return false;
    }
}
