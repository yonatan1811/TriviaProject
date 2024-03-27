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
using Client;
using Newtonsoft.Json;

namespace Trivia_Client.Pages.Connection
{
    /// <summary>
    /// Interaction logic for SignUp.xaml
    /// </summary>
    public partial class SignUp : UserControl, IContent
    {
        private ModernFrame frame;

        public SignUp()
        {
            InitializeComponent();
        }

        /// <summary>
        /// Signing up the user.
        /// </summary>
        private void Button_Click(object sender, RoutedEventArgs e)
        {
            string _Username, _Password, _Email;

            _Username = Username.Text;
            _Password = Password.Password;
            _Email = Email.Text;



            SignupRequest req = new SignupRequest(_Username, _Password, _Email);
            
            ClientReceivedMessage ClientMessage = new ClientReceivedMessage((ClientCodes.SignUp), JsonConvert.SerializeObject(req , Formatting.Indented));
            
            if (HandleSignUp(Session.CurrentUser.SendBackToServer(ClientMessage).Replace("\0", String.Empty)))
            {
                Session.JustSignedUp = true;
                frame.Source = new Uri(Paths.Home, UriKind.Relative); //change back to the home page
                Session.CurrentUser.SetDetails(Username.Text, Password.Password);
                Session.Logged = true;
            }
        }


        /// <summary>
        /// Checks if the user was properly signed up
        /// </summary>
        /// <param name="ReturnedMessage">The string returned from the server</param>
        /// <returns>True if was created properly, false if not</returns>
        private bool HandleSignUp(string ReturnedMessage)
        {
            statusResponse w = JsonConvert.DeserializeObject<statusResponse>(ReturnedMessage);

            if (w.status == ServerCodes.success)
                return true;
            
            return false;
        }

        #region Control Interface Implementation
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
        }
        public void OnNavigatingFrom(FirstFloor.ModernUI.Windows.Navigation.NavigatingCancelEventArgs e)
        {
           
        }
        #endregion
    }
}
