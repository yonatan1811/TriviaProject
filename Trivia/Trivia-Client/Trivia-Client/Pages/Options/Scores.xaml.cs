using FirstFloor.ModernUI.Windows;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using FirstFloor.ModernUI.Windows.Navigation;
using Client;
using Newtonsoft.Json;

namespace Trivia_Client.Pages.Options
{
    /// <summary>
    /// Interaction logic for Scores.xaml
    /// </summary>
    public partial class Scores : UserControl, IContent
    {
        private NavigationWindow NavWindow;

        public Scores(ServerReceivedMessage ServerMessage, NavigationWindow MyWindow)
        {
            InitializeComponent();
            InitializeScores(ServerMessage);
            NavWindow = MyWindow;
        }

        private void InitializeScores(ServerReceivedMessage ServerMessage)
        {

            GetGameResultsResponse res = JsonConvert.DeserializeObject<GetGameResultsResponse>(ServerMessage._StringedMessage);
            ListViewItem Item;
            UIScores.Items.Clear();
            for (int i = 0; i < res.results.Length; i++)
            {
                Item = new ListViewItem();
                Item.Content = res.results[i].username + " - " + "correct:" + res.results[i].correctAnswerCount.ToString() + " ,wrong:" + res.results[i].wrongAnswerCount.ToString() + " ,avg answer time:" + res.results[i].averageAnswerTime.ToString();
                UIScores.Items.Add(Item);
            }
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            NavWindow.Close();
        }

        public void OnFragmentNavigation(FirstFloor.ModernUI.Windows.Navigation.FragmentNavigationEventArgs e)
        {
        }

        public void OnNavigatedFrom(FirstFloor.ModernUI.Windows.Navigation.NavigationEventArgs e)
        {
        }

        public void OnNavigatedTo(FirstFloor.ModernUI.Windows.Navigation.NavigationEventArgs e)
        {
        }

        public void OnNavigatingFrom(FirstFloor.ModernUI.Windows.Navigation.NavigatingCancelEventArgs e)
        {
        }
    }
}
