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
using FirstFloor.ModernUI.Windows.Controls;
using Client;
using Newtonsoft.Json;

namespace Trivia_Client.Pages.Options
{
    /// <summary>
    /// Interaction logic for BestScores.xaml
    /// </summary>
    public partial class BestScores : UserControl, IContent
    {
        private ModernFrame frame;

        public BestScores()
        {
            InitializeComponent();
            InitializeScores();
        }



        public void InitializeScores()
        {
            ClientReceivedMessage ClientMessage = new ClientReceivedMessage(ClientCodes.BestScores , "");
            string json =Session.CurrentUser.SendBackToServer(ClientMessage);
            getHighScoreResponse res = JsonConvert.DeserializeObject<getHighScoreResponse>(json);
            switch (res.scores.Length)
            {
                case 2:
                    FirstScore.Text = res.scores[0] + " - " + res.scores[1];
                    SecondScore.Text = null;
                    ThirdScore.Text = null;
                    break;

                case 4:
                    FirstScore.Text = res.scores[0] + " - " + res.scores[1];
                    SecondScore.Text = res.scores[2] + " - " + res.scores[3];
                    ThirdScore.Text = null;
                    break;
                default:
                    FirstScore.Text = res.scores[0] + " - " + res.scores[1];
                    SecondScore.Text = res.scores[2] + " - " + res.scores[3];
                    ThirdScore.Text = res.scores[4] + " - " + res.scores[5];
                    break;
            }
        }

        

        public void OnFragmentNavigation(FirstFloor.ModernUI.Windows.Navigation.FragmentNavigationEventArgs e)
        {
        }

        public void OnNavigatedFrom(FirstFloor.ModernUI.Windows.Navigation.NavigationEventArgs e)
        {
            FirstScore.Text = null;
            SecondScore.Text = null;
            ThirdScore.Text = null;
        }

        public void OnNavigatedTo(FirstFloor.ModernUI.Windows.Navigation.NavigationEventArgs e)
        {
            if (frame == null)
                frame = e.Frame;
            else
                InitializeScores();
        }

        public void OnNavigatingFrom(FirstFloor.ModernUI.Windows.Navigation.NavigatingCancelEventArgs e)
        {
        }
    }
}
