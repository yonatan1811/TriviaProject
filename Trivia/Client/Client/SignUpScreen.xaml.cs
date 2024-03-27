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
    /// Interaction logic for SignUpScreen.xaml
    /// </summary>
    public partial class SignUpScreen : Window
    {
        public SignUpScreen()
        {
            InitializeComponent();
        }
        private void Home_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
            MainWindow mw = new MainWindow();
            mw.ShowDialog();
        }

        private void SendSignUpBtn(object sender, RoutedEventArgs e)
        {
            string _Username, _Password, _Email;

            _Username = Username.Text;
            _Password = Password.Password;
            _Email = Email.Text;

            string[] Values = new string[] { _Username, _Password, _Email };
            ClientReceivedMessage ClientMessage = new ClientReceivedMessage(ClientCodes.SignUp, Values);
            if (HandleSignUp(Session.CurrentUser.SendBackToServer(ClientMessage).Replace("\0", String.Empty)))
            {
                Session.JustSignedUp = true;
                frame.Source = new Uri(Paths.Home, UriKind.Relative);
            }
        }
    }

}
