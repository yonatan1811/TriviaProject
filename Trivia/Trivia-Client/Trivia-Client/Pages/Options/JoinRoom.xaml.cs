using FirstFloor.ModernUI.Windows;
using FirstFloor.ModernUI.Windows.Controls;
using System;
using System.Collections.Generic;
using System.Linq;
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
using Newtonsoft.Json;

namespace Trivia_Client.Pages.Options
{
    /// <summary>
    /// Interaction logic for JoinRoom.xaml
    /// </summary>
    public partial class JoinRoom : UserControl, IContent
    {
        private ModernFrame frame;

        public JoinRoom()
        {
            InitializeComponent();
            InitializeRooms();
        }

        /// <summary>
        /// Initializes the list of the rooms
        /// </summary>
        private void InitializeRooms()
        {
            try
            {
                ClientReceivedMessage ClientMessage = new ClientReceivedMessage(ClientCodes.AllRoomsList, "");
                //ServerReceivedMessage ServerMessage = new ServerReceivedMessage("106", Session.CurrentUser.SendBackToServer(ClientMessage).Replace("/0", String.Empty));

                string response = Session.CurrentUser.SendBackToServer(ClientMessage).Replace("\0", String.Empty);
                GetRoomsResponse res = JsonConvert.DeserializeObject<GetRoomsResponse>(response); 

                Dictionary<string, int> ExistingRooms = new Dictionary<string, int>();

                for (int i = 0; i < (res.rooms.Length) && res.rooms.Length != 0; i ++)
                {
                    ExistingRooms.Add(res.rooms[i].name, (res.rooms[i].id));
                }

                ListViewItem temp;
                Rooms.Items.Clear();

                foreach (var item in ExistingRooms)
                {
                    temp = new ListViewItem();
                    temp.Content = item.Key;
                    temp.Uid = item.Value.ToString();
                    temp.AddHandler(ListViewItem.MouseDoubleClickEvent, new RoutedEventHandler(JoinRoomButton_Click));
                    Rooms.Items.Add(temp);
                }
            }
            catch(Exception Exc)
            {

            }
        }


        #region Control Interface Implementation
        public void OnFragmentNavigation(FirstFloor.ModernUI.Windows.Navigation.FragmentNavigationEventArgs e)
        {
        }
        public void OnNavigatedFrom(FirstFloor.ModernUI.Windows.Navigation.NavigationEventArgs e)
        {
            ErroMessages.Text = "";
        }
        public void OnNavigatedTo(FirstFloor.ModernUI.Windows.Navigation.NavigationEventArgs e)
        {
            if (frame == null)
                frame = e.Frame;
            else
                InitializeRooms();
        }
        public void OnNavigatingFrom(FirstFloor.ModernUI.Windows.Navigation.NavigatingCancelEventArgs e)
        {
            
        }
        #endregion

        private void RefreshButton_Click(object sender, RoutedEventArgs e)
        {
            InitializeRooms();
        }

        /// <summary>
        /// Joins the player pushed the button to the room he chosed
        /// </summary>
        private void JoinRoomButton_Click(object sender, RoutedEventArgs e)
        {
            ClientReceivedMessage ClientMessage = new ClientReceivedMessage(ClientCodes.AllRoomsList, "");
            string response = Session.CurrentUser.SendBackToServer(ClientMessage).Replace("\0", String.Empty);
            GetRoomsResponse res = JsonConvert.DeserializeObject<GetRoomsResponse>(response);
            ListViewItem temp = (ListViewItem)Rooms.SelectedItem;
            int max = 0;

            for (int i = 0; i < res.rooms.Length; i++)
            {
                if (res.rooms[i].id == int.Parse(temp.Uid))
                {
                    max = res.rooms[i].maxPlayers;
                }
            }


            JoinRoomRequest req = new JoinRoomRequest(int.Parse(temp.Uid));
            string json = JsonConvert.SerializeObject(req);
             ClientMessage = new ClientReceivedMessage(ClientCodes.JoinRoom,  json);
            Session.CurrentUser.SendBackToServer(ClientMessage);
            GetRoomStateResponse res1 = GetRoom();
            

            if (res.status == 1)
            {

                Room Joined = new Room((res1.questionCount), (max), (res1.answerTimeout), false, temp.Content.ToString());
                Joined.Id = Convert.ToInt16(temp.Uid);

                Session.CurrentUser.SetRoom(Joined);
                frame.Source = new Uri(Paths.RoomPage, UriKind.Relative);
            }
            else
            {

            }
        }

        private GetRoomStateResponse GetRoom()
        {
            ClientReceivedMessage ClientMessage = new ClientReceivedMessage(Client.ClientCodes.UpdateRoom, "");
            string json = Session.CurrentUser.SendBackToServer(ClientMessage).Replace("/0", String.Empty);
            GetRoomStateResponse req = JsonConvert.DeserializeObject<GetRoomStateResponse>(json);


            return req;
        }
    }
}
