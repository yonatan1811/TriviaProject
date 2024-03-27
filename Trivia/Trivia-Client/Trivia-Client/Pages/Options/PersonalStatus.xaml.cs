using FirstFloor.ModernUI.Windows;
using FirstFloor.ModernUI.Windows.Controls;
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
    /// Interaction logic for PersonalStatus.xaml
    /// </summary>
    public partial class PersonalStatus : UserControl, IContent
    {
        private ModernFrame frame;

        public PersonalStatus()
        {
            InitializeComponent();
            InitializePersonalStatus();
        }

        private void InitializePersonalStatus()
        {
            ClientReceivedMessage ClientMessage = new ClientReceivedMessage(ClientCodes.PersonalState,"");
            string json = Session.CurrentUser.SendBackToServer(ClientMessage);
            getPersonalStatsResponse res = JsonConvert.DeserializeObject<getPersonalStatsResponse>(json);

            NumberofGames.Text = "Number of Games Played: " + res.statistics[1];
            NumberofCorrectAnswers.Text = "Number of Correct Answers: " + res.statistics[0];
            NumberofWrongAnswers.Text = "Number of Wrong Answers: " + (Convert.ToInt16(res.statistics[3]) - Convert.ToInt16(res.statistics[0])).ToString();
            AverageAnswerTime.Text = "Average Answer Time: " + res.statistics[2];
        }

        public void OnFragmentNavigation(FirstFloor.ModernUI.Windows.Navigation.FragmentNavigationEventArgs e)
        {

        }

        public void OnNavigatedFrom(FirstFloor.ModernUI.Windows.Navigation.NavigationEventArgs e)
        {

        }

        public void OnNavigatedTo(FirstFloor.ModernUI.Windows.Navigation.NavigationEventArgs e)
        {
            if (frame == null)
                frame = e.Frame;
            else
                InitializePersonalStatus();
        }

        public void OnNavigatingFrom(FirstFloor.ModernUI.Windows.Navigation.NavigatingCancelEventArgs e)
        {

        }
    }
}
