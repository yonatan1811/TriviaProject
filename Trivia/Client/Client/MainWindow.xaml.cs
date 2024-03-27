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
using System.Media;
using System.Windows.Media.Animation;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace Client
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {

        private const string ServerIP = "127.0.0.1"; // Replace with the server IP address
        private const int ServerPort = 2222; // Replace with the server port number

        private TcpClient client;
        private NetworkStream stream;
        private Thread receiveThread;



        public MainWindow()
        {
            InitializeComponent();
            
        }

        public void Connect(object sender, RoutedEventArgs e)
        {
            try
            {
                client = new TcpClient();
                client.Connect(ServerIP, ServerPort);

                stream = client.GetStream();

                receiveThread = new Thread(ReceiveMessages);
                receiveThread.Start();
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error connecting to the server: " + ex.Message);
            }
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            this.Hide();
            LoginScreen log = new LoginScreen();
            log.ShowDialog();
        }
        private void SignUp_click(object sender, RoutedEventArgs e)
        {
            this.Hide();
            SignUpScreen log = new SignUpScreen();
            log.ShowDialog();
        }


        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            System.Windows.Application.Current.Shutdown();
        }

        private void exit_btn(object sender, RoutedEventArgs e)
        {
            System.Windows.Application.Current.Shutdown();

        }

        private void CreateRoomBtn_Click(object sender, RoutedEventArgs e)
        {
            Window w = new Window { Title = "Create room mate", Content = new CreateRoom() };
            w.ShowDialog();
        }


        private void StatsBtn_Click(object sender, RoutedEventArgs e)
        {
            //this.Hide();
            Window w = new Window { Title = "Stats bro!", Content = new Stats() };
            w.ShowDialog();
        }

        private void ReceiveMessages()
        {
            try
            {
                byte[] data = new byte[1024];
                while (true)
                {
                    int bytesRead = stream.Read(data, 0, data.Length);
                    string message = Encoding.ASCII.GetString(data, 0, bytesRead);

                }
            }
            catch (Exception ex)
            {

            }
        }


       


    }
}
