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
using System.Text.Json;



namespace Client
{


    /// <summary>
    /// Interaction logic for CreateRoom.xaml
    /// </summary>
    public partial class CreateRoom : UserControl
    {
        public CreateRoom()
        {
            InitializeComponent();
        }

        private void Number_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key < Key.D0 || e.Key > Key.D9)
            {
                // Determine whether the keystroke is a number from the keypad.
                if (e.Key < Key.NumPad0 || e.Key > Key.NumPad9)
                {
                    // Determine whether the keystroke is a backspace.
                    if (e.Key != Key.Back)
                    {
                        // A non-numerical keystroke was pressed.
                        // Set the flag to true and evaluate in KeyPress event.
                        e.Handled = true;
                    }
                }
            }

            // Determine wheter the keystroke is a space
            if (e.Key == Key.Space)
            {
                e.Handled = true;
            }
        }

        public class Room
        {
            public int quetionNumber { get; set; }
            public int quetionTime { get; set; }
            public int Playersnumber { get; set; }
            public string roomname { get; set; }
        }


        private int convert_string_int(string w)
        {
            int q = 0;
            bool sec = int.TryParse(w, out q);
            if (sec)
            {
                return q;
            }
            return -1;
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {

            if (QuestionsNumber != null && QuestionTime != null && PlayersNumber != null && RoomName != null)
            {

                Room r = new Room() { quetionNumber = convert_string_int(QuestionsNumber.Text), roomname = RoomName.Text , Playersnumber = convert_string_int(PlayersNumber.Text) , quetionTime = convert_string_int(QuestionTime.Text) };
                string strJson = JsonSerializer.Serialize<Room>(r);
            }

        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {

        }
    }
}
