using FirstFloor.ModernUI.Windows;
using FirstFloor.ModernUI.Windows.Controls;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading;
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
using System.Windows.Media.Media3D;
using Newtonsoft.Json;


namespace Trivia_Client.Pages.Options
{
    /// <summary>
    /// Interaction logic for RoomPage.xaml
    /// </summary>
    public partial class RoomPage : UserControl, IContent
    {
        private ModernFrame frame;
        private Thread HandleRequests;
        private string _RoomName;
        int _NumberOfPlayers, _QuestionNum, _QuestionTime;

        public RoomPage()
        {
            InitializeComponent();
            InitiateDetails();
        }

        /// <summary>
        /// Initiates the room's details (Name, Questions Details And Players Connected)
        /// </summary>
        public void InitiateDetails()
        {
            var Window = Application.Current.MainWindow as ModernWindow;

            Window.TitleLinks.Clear();
            Window.MenuLinkGroups.ElementAt(0).Links.RemoveAt(5);
            Window.MenuLinkGroups.ElementAt(0).Links.RemoveAt(4);

            Room ThisRoom = Session.CurrentUser.GetRoom();
            //GetPlayersInRoomRequest req = new GetPlayersInRoomRequest(ThisRoom.Id);
            //string json1 = JsonConvert.SerializeObject(req , Formatting.Indented);
            ClientReceivedMessage Message = new ClientReceivedMessage(17, "");

            string json = Session.CurrentUser.SendBackToServer(Message).Replace("\0", String.Empty);

            GetRoomStateResponse n = JsonConvert.DeserializeObject<GetRoomStateResponse>(json);


            //FFlush();

            _RoomName = RoomName.Text = ThisRoom.RoomName; // Initiating The Room's Name.

            int NumberOfPlayers = _NumberOfPlayers = (n.players.Length);
            _QuestionNum = ThisRoom.QuestionNumber;
            _QuestionTime = ThisRoom.QuestionTime;

            PlayersNum.Text = "Number of connected users: " + NumberOfPlayers + "/" + ThisRoom.MaxUsers; // initiating Number of Players.

            QuestionNum.Text = "Number of question in the trivia: " + ThisRoom.QuestionNumber; // Initiating Questions Number.

            QuestionTime.Text = "Seconds to answer a question: " + ThisRoom.QuestionTime; // Initiating Question Time.

            List<string> Users = new List<string>(Convert.ToInt16(n.players.Length));

            for (int i = 0; i < (n.players.Length); i++)
            {
                Users.Add(n.players[i]);
            }

            ListViewItem temp;
            Players.Items.Clear();
            foreach (var item in Users)
            {
                temp = new ListViewItem();
                temp.Content = item;
                Players.Items.Add(temp);
            }

            if (!ThisRoom.IsAdmin)
            {
                StartGame.Visibility = Visibility.Collapsed;
                CloseRoom.Visibility = Visibility.Collapsed;
                LeaveRoom.Visibility = Visibility.Visible;

            }
            else
            {
                StartGame.Visibility = Visibility.Visible;
                CloseRoom.Visibility = Visibility.Visible;
                LeaveRoom.Visibility = Visibility.Collapsed;
            }



            HandleRequests = new Thread(new ThreadStart(HandlePlayers)); // A Thread to listen to other players join/Exit
            HandleRequests.Start();
        }

        /// <summary>
        /// Checks if the the room was closed properly
        /// </summary>
        /// <param name="ReturnedMessage">The string returned from the server</param>
        /// <returns>True if closed properly, false if not</returns>
        public bool HandleCloseRoom(int isClosed)
        {
            if (isClosed == ServerCodes.offline)
            {
                Session.CurrentUser.SetRoom(null);
                return true;
            }
            else
                return false;
        }

        /// <summary>
        /// Checks if the user left the room properly
        /// </summary>
        /// <param name="ReturnedMessage">The string returned from the server</param>
        /// <returns>True if left properly, false if not</returns>
        public bool HandleLeaveRoom(int status)
        {
            if (status == ServerCodes.success)
            {
                Session.CurrentUser.SetRoom(null);
                Session.CurrentUser.SetGame(null);
                frame.Dispatcher.Invoke(new ChangeFrameCallback(ChangeFrame), new Uri(Paths.Home, UriKind.Relative));// Redirecting to the Game Page
                return true;
            }
            else
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
            else
                InitiateDetails();
            if (Session.CurrentUser != null && Session.CurrentUser.GetRoom() == null)
                e.Frame.Source = new Uri("./Pages/Options/CreateRoom.xaml", UriKind.Relative);
        }
        public void OnNavigatingFrom(FirstFloor.ModernUI.Windows.Navigation.NavigatingCancelEventArgs e)
        {
            ClientReceivedMessage Message;

            if (Session.CurrentUser != null && Session.CurrentUser.GetRoom() != null && Session.CurrentUser.GetRoom().IsAdmin) // Also Need to add handle Close Room Only If the Player Was Admin
            {
                Session.CurrentUser.SetRoom(null);
                Message = new ClientReceivedMessage(ClientCodes.CloseRoom, "");
                string req = JsonConvert.SerializeObject(Message, Formatting.Indented);
                Session.CurrentUser.GetSocket().Send(Encoding.ASCII.GetBytes(req));
            }
            else if (Session.CurrentUser != null && Session.CurrentUser.GetRoom() != null && !Session.CurrentUser.GetRoom().IsAdmin)
            {
                Session.CurrentUser.SetRoom(null);
                Message = new ClientReceivedMessage(ClientCodes.LeaveRoom, "");
                string req = JsonConvert.SerializeObject(Message, Formatting.Indented);
                Session.CurrentUser.GetSocket().Send(Encoding.ASCII.GetBytes(req));
            }

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
        #endregion

        /// <summary>
        /// Get's the server Response another time in order to clean the buffer
        /// </summary>
        private void FFlush()
        {
            byte[] bytes = new byte[1024];
            Session.CurrentUser.GetSocket().Receive(bytes);
        }

        #region Thread
        /// <summary>
        /// The main function for the thread which listens to new Requests
        /// </summary>
        private void HandlePlayers()
        {
            while (true)
            {
                Thread.Sleep(200);//waiting 1 sec each req
                byte[] bytes = new byte[1024];

                ClientReceivedMessage Message = new ClientReceivedMessage(17, "");
                string json = Session.CurrentUser.SendBackToServer(Message);
                if (json == null)
                {
                    return;
                }
                json = json.Replace("\0", String.Empty);
                GetRoomStateResponse n = JsonConvert.DeserializeObject<GetRoomStateResponse>(json);

                if (n.hasGameBegun == ServerCodes.offline) // Closing the Room
                {
                    HandleLeaveRoom(n.status);
                    break;
                }
                else if (n.hasGameBegun == ServerCodes.runningGame)
                {
                    HandleStartGame(json);
                    break;
                }
                else
                {
                    ServerReceivedMessage ServerMessage = new ServerReceivedMessage("17", json);
                    Players.Dispatcher.Invoke(new UpdateViewCallback(UpdateView), ServerMessage);
                }
            }
        }

        public delegate void UpdateViewCallback(ServerReceivedMessage ServerMessage);
        public delegate void ChangeFrameCallback(Uri Path);

        /// <summary>
        /// Changing the frame (Navigation)
        /// </summary>
        /// <param name="Path">The path to navigate to</param>
        private void ChangeFrame(Uri Path)
        {
            frame.Source = Path;
        }

        /// <summary>
        /// Updating the view according to the new message from the server
        /// </summary>
        /// <param name="ServerMessage">The message from the server</param>
        private void UpdateView(ServerReceivedMessage ServerMessage)
        {

            GetRoomStateResponse res = JsonConvert.DeserializeObject<GetRoomStateResponse>(ServerMessage._StringedMessage);
            Room ThisRoom = Session.CurrentUser.GetRoom();
            if (ThisRoom != null)
            {
                List<string> Users = new List<string>(res.players.Length);

                for (int i = 0; i < res.players.Length; i++)
                {
                    Users.Add(res.players[i]);
                }

                PlayersNum.Text = "Number of connected users: " + Users.Count + "/" + ThisRoom.MaxUsers; // initiating Number of Players.

                ListViewItem temp;
                Players.Items.Clear();

                foreach (var item in Users)
                {
                    temp = new ListViewItem();
                    temp.Content = item;
                    Players.Items.Add(temp);
                }
            }

        }
        #endregion

        private void CloseRoom_Click(object sender, RoutedEventArgs e)
        {
            HandleRequests.Suspend();
            ClientReceivedMessage ClientMessage = new ClientReceivedMessage(ClientCodes.CloseRoom, "");
            string h = Session.CurrentUser.SendBackToServer(ClientMessage);
            HandleCloseRoom(h);
        }

        private void HandleCloseRoom(string response)
        {
            statusResponse res = JsonConvert.DeserializeObject<statusResponse>(response);

            if (res.status == 0) // The Start Game action did not work
                return;

            Session.CurrentUser.SetRoom(null);
            frame.Dispatcher.Invoke(new ChangeFrameCallback(ChangeFrame), new Uri(Paths.Home, UriKind.Relative));// Redirecting to the Game Page


        }

        private void LeaveRoom_Click(object sender, RoutedEventArgs e)
        {
            ClientReceivedMessage n = new ClientReceivedMessage(Client.ClientCodes.LeaveRoom, "");
            Session.CurrentUser.sendBackToServerWithoutRes(n);
            HandleLeaveRoom(1);
        }

        private void StartGame_Click(object sender, RoutedEventArgs e)
        {
            HandleRequests.Suspend();
            ClientReceivedMessage ClientMessage = new ClientReceivedMessage(ClientCodes.StartGame, "");
            string h = Session.CurrentUser.SendBackToServer(ClientMessage);
            HandleStartGame(h);
        }

        private void HandleStartGame(string Response)
        {
            statusResponse res = JsonConvert.DeserializeObject<statusResponse>(Response);

            if (res.status == 0) // The Start Game action did not work
                return;

            Session.CurrentUser.SetRoom(null);

            ClientReceivedMessage Message = new ClientReceivedMessage(ClientCodes.GetQuestion, "");
            string json = Session.CurrentUser.SendBackToServer(Message);
            GetQuestionResponse qRes = JsonConvert.DeserializeObject<GetQuestionResponse>(json);
            Question Current = new Question();
            Current._Question = qRes.question;
            foreach (KeyValuePair<int, string> q in qRes.answers)
            {
                Current._Answers.Add(q.Value);
            }
            Client.Game newGame = new Client.Game(_NumberOfPlayers, _QuestionNum, _QuestionTime);
            newGame.CurrentQuestion = Current;
            Session.CurrentUser.SetGame(newGame);
            frame.Dispatcher.Invoke(new ChangeFrameCallback(ChangeFrame), new Uri(Paths.GamePage, UriKind.Relative));// Redirecting to the Game Page
        }
    }
}
