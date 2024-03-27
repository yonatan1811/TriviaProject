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
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Client
{
    /// <summary>
    /// Interaction logic for Stats.xaml
    /// </summary>
    public partial class Stats : Page
    {
        public Stats()
        {
            InitializeComponent();
        }

        private void BTNhome(object sender, RoutedEventArgs e)
        {
            MainWindow mw = new MainWindow();
            mw.ShowDialog();
        }

        private void btnPersonal(object sender, RoutedEventArgs e)
        {

            PersonalStats mw = new PersonalStats();
            mw.ShowDialog();
        }

        private void Leader(object sender, RoutedEventArgs e)
        {
            LeaderBoard win = new LeaderBoard();
            win.ShowDialog();
        }
    }
}
