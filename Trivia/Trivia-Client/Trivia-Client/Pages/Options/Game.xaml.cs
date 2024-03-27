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
using System.Windows.Threading;
using System.Threading;
using Client;
using Newtonsoft.Json;
using System.Windows.Interop;
using static Trivia_Client.Pages.Options.RoomPage;

namespace Trivia_Client.Pages.Options
{
    /// <summary>
    /// Interaction logic for Game.xaml
    /// </summary>
    public partial class Game : UserControl, IContent
    {
        private Thread Timer;
        private ModernFrame frame;
        private int QuestionCounter, CorrectAnswers;

        public Game()
        {
            InitializeComponent();
            QuestionCounter = CorrectAnswers = 0;
            SetDetails();
        }

        private void SetDetails()
        {
            QuestionCounter++;

            ResetButtons();
            

            
            Client.Game CurrentGame = Session.CurrentUser.GetGame();

            Title.Text = "Question Number " + QuestionCounter.ToString();

            Question.Text = CurrentGame.CurrentQuestion._Question;

            Answer1.Content = CurrentGame.CurrentQuestion._Answers[0];
            Answer2.Content = CurrentGame.CurrentQuestion._Answers[1];
            Answer3.Content = CurrentGame.CurrentQuestion._Answers[2];
            Answer4.Content = CurrentGame.CurrentQuestion._Answers[3];

            Clock.Text = CurrentGame.QuestionTime.ToString();

            Timer = new Thread(new ThreadStart(Tick));
            Timer.Start();
        }


        #region Thread
        public delegate void TickCallback();
        public delegate int ClockCallback();
        /// <summary>
        /// The Main Function for the thread.
        /// </summary>
        private void Tick() 
        {
            while (!Clock.Dispatcher.Invoke(new ClockCallback(ClockTime)).Equals(0))
            {
                Thread.Sleep(1000);
                Clock.Dispatcher.Invoke(new TickCallback(Timer_Tick));
            }
            DefaultAnswer();
        }   

        void Timer_Tick()
        {
            Clock.Text = (Convert.ToInt16(Clock.Text) - 1).ToString();
        }

        int ClockTime()
        {
            return Convert.ToInt32(Clock.Text);
        }
        #endregion

        private void Answer_Click(int QuestionNumber)
        {
            try
            {
                Timer.Abort(); // Stopping the timer.
                string[] Values = new string[2];

                Values[0] = QuestionNumber.ToString();
                Values[1] = (Session.CurrentUser.GetGame().QuestionTime - Convert.ToInt16(Clock.Text)).ToString();
                SubmitAnswerRequest req = new SubmitAnswerRequest(QuestionNumber, Session.CurrentUser.GetGame().QuestionTime - Convert.ToInt16(Clock.Text));
                string json = JsonConvert.SerializeObject(req);
                ClientReceivedMessage ClientMessage = new ClientReceivedMessage(ClientCodes.Answer, json);
                string ans = Session.CurrentUser.SendBackToServer(ClientMessage);
                SubmitAnswerResponse res = JsonConvert.DeserializeObject<SubmitAnswerResponse>(ans);
                Answer1.IsEnabled = false;
                Answer2.IsEnabled = false;
                Answer3.IsEnabled = false;
                Answer4.IsEnabled = false;

                if (res.status == ServerCodes.success)
                {
                    CorrectAnswers++;
                    UpdateButtonColor(QuestionNumber, true);
                }
                else
                {
                    UpdateButtonColor(QuestionNumber, false);
                }
                Answer1.Dispatcher.Invoke((Action)(() => { }), DispatcherPriority.Render); // Forcing wpf to process the UI modification before the thread sleeps.
                Thread.Sleep(500);
                GetNextQuestion();
            }
            catch (Exception Exc)
            {
                Session.CurrentUser.SetGame(null);
                frame.Source = new Uri(Paths.Home, UriKind.Relative);
            }
        }

        private void DefaultAnswer()
        {
            try
            {
                Timer.Abort();
                SubmitAnswerRequest req = new SubmitAnswerRequest(0, -1);
                string json = JsonConvert.SerializeObject(req);
                ClientReceivedMessage ClientMessage = new ClientReceivedMessage(ClientCodes.Answer, json);
                string ans = Session.CurrentUser.SendBackToServer(ClientMessage);

                Answer1.IsEnabled = false;
                Answer2.IsEnabled = false;
                Answer3.IsEnabled = false;
                Answer4.IsEnabled = false;
                UpdateButtonColor(1, false);
                UpdateButtonColor(2, false);
                UpdateButtonColor(3, false);
                UpdateButtonColor(4, false);
                Answer1.Dispatcher.Invoke((Action)(() => { }), DispatcherPriority.Render); // Forcing wpf to process the UI modification before the thread sleeps.
                Thread.Sleep(500);
                GetNextQuestion();
            }
            catch (Exception Exc)
            {
                Session.CurrentUser.SetGame(null);
                frame.Source = new Uri(Paths.Home, UriKind.Relative);
            }
        }

        private void GetNextQuestion()
        {
            ClientReceivedMessage Message = new ClientReceivedMessage(ClientCodes.GetQuestion, "");
            string json = Session.CurrentUser.SendBackToServer(Message);
            GetQuestionResponse qRes = JsonConvert.DeserializeObject<GetQuestionResponse>(json);

            Client.Game CurrentGame = Session.CurrentUser.GetGame();
            if (qRes.status == ServerCodes.success && qRes.question != "ERROR")
            {
                Question Next = new Client.Question();

                Next._Question = qRes.question;
                foreach (KeyValuePair<int, string> q in qRes.answers)
                {
                    Next._Answers.Add(q.Value);
                }
                CurrentGame.CurrentQuestion = Next;
                SetDetails();
            }
            else
            {
                HandleEndGame();

                //leave game
                ClientReceivedMessage msg = new ClientReceivedMessage(ClientCodes.LeaveGame, "");
                Session.CurrentUser.SendBackToServer(msg);
                frame.Source = new Uri(Paths.Home, UriKind.Relative);
            }
        }

        private void UpdateButtonColor(int QuestionNum, bool Indicator)
        {
            if (Indicator)
            {
                switch (QuestionNum)
                {
                    case 1:
                        Answer1.Background = Brushes.ForestGreen;
                        break;

                    case 2:
                        Answer2.Background = Brushes.ForestGreen;
                        break;

                    case 3:
                        Answer3.Background = Brushes.ForestGreen;
                        break;

                    case 4:
                        Answer4.Background = Brushes.ForestGreen;
                        break;
                }
            }
            else
            {
                switch (QuestionNum)
                {
                    case 1:
                        Answer1.Background = Brushes.Red;
                        break;

                    case 2:
                        Answer2.Background = Brushes.Red;
                        break;

                    case 3:
                        Answer3.Background = Brushes.Red;
                        break;

                    case 4:
                        Answer4.Background = Brushes.Red;
                        break;
                }
            }
        }

        private void ResetButtons()
        {
            Answer1.IsEnabled = true;
            Answer1.Background = Brushes.DimGray;

            Answer2.IsEnabled = true;
            Answer2.Background = Brushes.DimGray;

            Answer3.IsEnabled = true;
            Answer3.Background = Brushes.DimGray;

            Answer4.IsEnabled = true;
            Answer4.Background = Brushes.DimGray;
        }

        private void HandleEndGame()
        {
            Clock.Text = "Waiting for results";
            ClientReceivedMessage msg = new ClientReceivedMessage(ClientCodes.hasGameEnded, "");
            string json = Session.CurrentUser.SendBackToServer(msg);
            Thread.Sleep(3000);
            msg = new ClientReceivedMessage(ClientCodes.getResults, "");
            json = Session.CurrentUser.SendBackToServer(msg);
            ServerReceivedMessage ServerMessage = new ServerReceivedMessage((ClientCodes.getResults).ToString(),json);
            NavigationWindow GameScores = new NavigationWindow();
            GameScores.Navigate(new Scores(ServerMessage, GameScores));
            GameScores.Show();
            ResetButtons();
            Thread.Sleep(3000);
            Session.CurrentUser.SetGame(null);
        }

        #region Control Interface Implementation
        public void OnFragmentNavigation(FirstFloor.ModernUI.Windows.Navigation.FragmentNavigationEventArgs e)
        {
            
        }

        public void OnNavigatedFrom(FirstFloor.ModernUI.Windows.Navigation.NavigationEventArgs e)
        {
            var Window = Application.Current.MainWindow as ModernWindow;

            Window.MenuLinkGroups.ElementAt(0).Links.Add(new FirstFloor.ModernUI.Presentation.Link()
            {
                DisplayName = "Sign Out",
                Source = new Uri(Paths.SignOut, UriKind.Relative)
            });

            Window.MenuLinkGroups.ElementAt(0).Links.Add(new FirstFloor.ModernUI.Presentation.Link()
            {
                DisplayName = "Settings",
                Source = new Uri(Paths.SettingsPage, UriKind.Relative)
            });
        }

        public void OnNavigatedTo(FirstFloor.ModernUI.Windows.Navigation.NavigationEventArgs e)
        {
            if (frame == null)
                frame = e.Frame;
            else
            {
                QuestionCounter = CorrectAnswers = 0;
                SetDetails();
            }

            if (Session.CurrentUser.GetGame() == null)
                e.Frame.Source = new Uri(Paths.Home, UriKind.Relative);
        }

        public void OnNavigatingFrom(FirstFloor.ModernUI.Windows.Navigation.NavigatingCancelEventArgs e)
        {
            
        }
        #endregion

        #region Navigation Functions
        private void Answer1_Click(object sender, RoutedEventArgs e)
        {
            Answer_Click(1);
        }

        private void Answer2_Click(object sender, RoutedEventArgs e)
        {
            Answer_Click(2);
        }

        private void Answer3_Click(object sender, RoutedEventArgs e)
        {
            Answer_Click(3);
        }

        private void LeaveGameButton(object sender, RoutedEventArgs e)
        {
            ClientReceivedMessage nee = new ClientReceivedMessage(ClientCodes.LeaveGame, "");
            string h = Session.CurrentUser.SendBackToServer(nee);
            frame.Source = new Uri(Paths.Home, UriKind.Relative);
            //HandleEndGame();
        }

        private void Answer4_Click(object sender, RoutedEventArgs e)
        {
            Answer_Click(4);
        }
        #endregion
    }
}
