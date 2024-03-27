using System;
using System.Collections.Generic;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace Client
{
    /// <summary>
    /// Interaction logic for LeaderBoard.xaml
    /// </summary>
    public partial class LeaderBoard : Window
    {
        public LeaderBoard()
        {
            InitializeComponent();
            ClickMe();
        }


        private void ClickMe()
        {
            DataGrid d1 = new DataGrid();
            DataGridTextColumn d = new DataGridTextColumn();
            d.Header = "Name";
            d1.Columns.Add(d);
            grid1.Children.Add(d1);
        }
    }
}
