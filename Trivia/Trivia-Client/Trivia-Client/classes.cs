using System;
using System.Collections.Generic;
using System.Text;
using System.Net.Sockets;
using Newtonsoft.Json;

namespace Client
{
    public static class Session
    {
        public static User CurrentUser = null;
        public static bool Logged = false;
        public static bool JustSignedUp = false;
    }
    #region User Room Game Question
    public class User
    {
        private string Username;
        private string Password;
        private Socket ClientSocket;
        private Room CurrRoom;
        private Game CurrGame;


        public User()
        {
            Username = null;
            Password = null;
        }

        public User(string Username, string Password)
        {
            this.Username = Username;
            this.Password = Password;
        }

        public User(User other)
        {
            Username = other.Username;
            Password = other.Password;
        }

        public User(Socket ClientSocket)
        {
            this.ClientSocket = ClientSocket;
        }

        public void SetDetails(string Username, string Password)
        {
            this.Username = Username;
            this.Password = Password;
        }

        public Socket GetSocket()
        {
            return ClientSocket;
        }

        public void SetGame(Game ToChange)
        {
            CurrGame = ToChange;
        }

        public Game GetGame()
        {
            return CurrGame;
        }

        public void SetRoom(Room ToChange)
        {
            CurrRoom = ToChange;
        }

        public Room GetRoom()
        {
            return CurrRoom;
        }

        public string SendBackToServer(ClientReceivedMessage Message)
        { 
            string Send = Message.intToPaddedBinStr(Message._MessageCode, 8) + Message.intToPaddedBinStr(Message._jsonString.Length * 8,4*8) + Message.StringToBinary(Message._jsonString);
            byte[] ToSend = Encoding.ASCII.GetBytes(Send);
            byte[] ToRecieve = new byte[40];
            try
            {
                ClientSocket.Send(ToSend);
                int bytesRec = ClientSocket.Receive(ToRecieve);
                string strMsg = System.Text.Encoding.UTF8.GetString(ToRecieve);
                int code = Message.binToDec(strMsg.Substring(0, 8));
                int len = Message.binToDec(strMsg.Substring(8, 32));
                byte[] binMsg = new byte[len];
                ClientSocket.Receive(binMsg);
                strMsg = System.Text.Encoding.UTF8.GetString(binMsg);
                string json = Message.BinaryToString(strMsg);
                return json;
            }
            catch (Exception e) { return null; }
        }
        public void sendBackToServerWithoutRes(ClientReceivedMessage Message)
        {
            string Send = Message.intToPaddedBinStr(Message._MessageCode, 8) + Message.intToPaddedBinStr(Message._jsonString.Length * 8, 4 * 8) + Message.StringToBinary(Message._jsonString);
            byte[] ToSend = Encoding.ASCII.GetBytes(Send);
            byte[] ToRecieve = new byte[40];
            try
            {
                ClientSocket.Send(ToSend);
            }
            catch (Exception e) { return; }
        }

        public string GetUsername()
        {
            return Username;
        }
    }

    public class Room
    {
        public int MaxUsers { get; set; }
        public int QuestionNumber { get; set; }
        public int QuestionTime { get; set; }
        public List<string> Users { get; private set; }
        public bool IsAdmin { get; set; }
        public int Id { get; set; }
        public string RoomName { get; set; }

        public Room(int QuestionNumber, int MaxUsers, int QuestionTime, bool IsAdmin, string RoomName)
        {
            this.QuestionNumber = QuestionNumber;
            this.QuestionTime = QuestionTime;
            this.MaxUsers = MaxUsers;
            this.IsAdmin = IsAdmin;
            this.RoomName = RoomName;
        }

        public void SetUsers(List<string> other)
        {
            this.Users = new List<string>(other);
        }
    }

    public class Game
    {
        public int NumberOfUsers { get; private set; }
        public int QuestionNumber { get; private set; }
        public int QuestionTime { get; private set; }
        public Question CurrentQuestion { get; set; }

        public Game(int NumberOfUsers, int QuestionNumber, int QuestionTime)
        {
            this.NumberOfUsers = NumberOfUsers;
            this.QuestionNumber = QuestionNumber;
            this.QuestionTime = QuestionTime;
        }
    }

    public class Question
    {
        public string _Question;
        public List<string> _Answers;

        public Question()
        {
            _Answers = new List<string>(4);
        }
    }
    #endregion

    #region ReceivedMessages
    public class ClientReceivedMessage
    {
        public string _jsonString { get; }
        public int _MessageCode { get; }

        public ClientReceivedMessage(int Code,string json)
        {
            _MessageCode = Code;
            _jsonString = json;
        }

        public  string intToPaddedBinStr(int dec,int padding)
        {
            int binary = 0, product = 1, remainder = 0;
            while (dec != 0)
            {
                remainder = dec % 2;
                binary = binary + (remainder * product);
                dec = dec / 2;
                product *= 10;
            }
            string strNum = binary.ToString();
            return strNum.PadLeft(padding, '0');
        }

        public string StringToBinary(string data)
        {
            StringBuilder sb = new StringBuilder();

            foreach (char c in data.ToCharArray())
            {
                sb.Append(Convert.ToString(c, 2).PadLeft(8, '0'));
            }
            return sb.ToString();
        }

        public  string BinaryToString(string data)
        {
            List<Byte> byteList = new List<Byte>();

            for (int i = 0; i < data.Length; i += 8)
            {
                byteList.Add(Convert.ToByte(data.Substring(i, 8), 2));
            }
            return Encoding.ASCII.GetString(byteList.ToArray());
        }

        public int binToDec(string num)
        {
            double dec = 0;
            for (int i = 0; i < num.Length; i++)
            {

                dec += ((num[i] - '0')) * Math.Pow(2, (num.Length) - i - 1);
            }
            return (int)dec;
        }

        public static string[] RemoveDash(string[] s)
        {
            bool Found = false;
            string[] se = new string[s.Length * 2];
            for (int i = 0; i < se.Length; i++)
            {
                se[i] = "";
            }
            for (int i = 0; i < s.Length; i++)
            {
                for (int j = 0; j < s[i].Length; j++)
                {
                    if (s[i][j] != '-' && Found == false)
                    {
                        se[i] += s[i][j];
                    }
                    else if (s[i][j] == '-')
                    {
                        Found = true;
                    }
                    else
                    {
                        se[i + 1] += s[i][j];
                    }
                    
                }
            }

            return se;
        }


        //private static string StringedForgotPassword(ClientReceivedMessage)
        //{
        //    JsonSerializer serializer = new JsonSerializer();
        //    string json = JsonConvert.SerializeObject(this, Formatting.Indented);
        //}

        //public void StringedSignIn()
        //{
        //    JsonSerializer serializer = new JsonSerializer();
        //    string json = JsonConvert.SerializeObject(this, Formatting.Indented);
        //}
        //public static string StringedSignIn(ClientReceivedMessage r)
        //{
        //    JsonSerializer serializer = new JsonSerializer();
        //    string json = JsonConvert.SerializeObject(r, Formatting.Indented);
        //    return json;
        //}

        //public void StringedSignUp()
        //{
        //    JsonSerializer serializer = new JsonSerializer();
        //    string json = JsonConvert.SerializeObject(this, Formatting.Indented);
        //}

        //public void StringedAllRoomUsers()
        //{
        //    JsonSerializer serializer = new JsonSerializer();
        //    string json = JsonConvert.SerializeObject(this, Formatting.Indented);
        //}

        //public void StringedJoinRoom()
        //{
        //    JsonSerializer serializer = new JsonSerializer();
        //    string json = JsonConvert.SerializeObject(this, Formatting.Indented);
        //}

        //public void StringedCreateRoom()
        //{
        //    JsonSerializer serializer = new JsonSerializer();
        //    string json = JsonConvert.SerializeObject(this, Formatting.Indented);
        //}

        //public void StringedAnswer()
        //{
        //    JsonSerializer serializer = new JsonSerializer();
        //    string json = JsonConvert.SerializeObject(this, Formatting.Indented);
        //}
    }

    public class ServerReceivedMessage
    {
        public string[] _Values { get; private set; }
        public string _MessageCode { get; private set; }
        public string _StringedMessage { get; set; }
        public Exception ErrorMessage { get; private set; }

        public ServerReceivedMessage(string Code)
        {
            _MessageCode = _StringedMessage = Code;
            _Values = new string[0];
        }

        public ServerReceivedMessage(string Code, string StringedMessage)
        {
            _StringedMessage = StringedMessage;

            //switch (Code)
            //{
            //    case ServerCodes.ForgotPassword:
            //        ParameteredForgotPassword();
            //        break;

            //    case ServerCodes.AllRooms:
            //        ParameteredAllRooms();
            //        break;

            //    case ServerCodes.AllRoomUsers:
            //        ParameteredAllRoomUsers();
            //        break;

            //    case ServerCodes.JoinRoom:
            //        ParameteredJoinRoom();
            //        break;

            //    case ServerCodes.SendQuestions:
            //        ParameteredSendQuestion();
            //        break;

            //    case ServerCodes.EndGame:
            //        ParameteredEndGame();
            //        break;

            //    case ServerCodes.BestScores:
            //        ParameteredBestScores();
            //        break;

            //    case ServerCodes.PersonalState:
            //        ParameteredPersonalState();
            //        break;


            }
        }

        

        

        //public void ParameteredJoinRoom()
        //{
        //    string temp = _StringedMessage, Indicator;

        //    temp = temp.Substring(3); // Skipping the Message Code.

        //    Indicator = temp.Substring(0, 1); // getting the status.
        //    temp = temp.Substring(1);

        //    switch (Indicator)
        //    {
        //        case "0":
        //            _Values = new string[3];

        //            _Values[0] = temp.Substring(0, 2);
        //            temp = temp.Substring(2);

        //            _Values[1] = temp.Substring(0, 2);
        //            temp = temp.Substring(2);

        //            _Values[2] = temp.Substring(0, 1);
        //            break;

        //        case "1":
        //            ErrorMessage = new Exception("Room is Full");
        //            break;

        //        case "2":
        //            ErrorMessage = new Exception("Room doesn't exist or Other");
        //            break;

        //        default:
        //            ErrorMessage = new Exception("Unknown Error");
        //            break;
        //    }
        //}

        //public void ParameteredSendQuestion()
        //{
        //    string temp = _StringedMessage;
        //    int Length;
        //    _MessageCode = temp.Substring(0, 3);
        //    temp = temp.Substring(3); // Skipping the Message Code.

        //    _Values = new string[5];

        //    for (int i = 0; i < 5; i++)
        //    {
        //        Length = Convert.ToInt16(temp.Substring(0, 3));
        //        temp = temp.Substring(3); // Skipping the length

        //        _Values[i] = temp.Substring(0, Length);
        //        temp = temp.Substring(Length); // Skipping the name
        //    }
        //}

        //public void ParameteredEndGame()
        //{
        //    string temp = _StringedMessage;
        //    int Length;
        //    _MessageCode = temp.Substring(0, 3);
        //    temp = temp.Substring(3);

        //    int NumberOfUsers = Convert.ToInt16(temp.Substring(0, 1));
        //    temp = temp.Substring(1);

        //    _Values = new string[NumberOfUsers * 2];

        //    for (int i = 1; i < NumberOfUsers * 2; i += 2)
        //    {
        //        Length = Convert.ToInt16(temp.Substring(0, 2));
        //        temp = temp.Substring(2);

        //        _Values[i - 1] = temp.Substring(0, Length);
        //        temp = temp.Substring(Length);

        //        _Values[i] = temp.Substring(0, 2);
        //        temp = temp.Substring(2);
        //    }
        //}

        //public void ParameteredBestScores()
        //{
        //    string temp = _StringedMessage.Replace("\0", String.Empty);
        //    int Length, i;
        //    temp = temp.Substring(3); // Skipping the Message code.

        //    string[] TempValues = new string[6];

        //    for (i = 0; i < 6 && temp != String.Empty; i += 2)
        //    {
        //        Length = Convert.ToInt16(temp.Substring(0, 2));
        //        temp = temp.Substring(2);

        //        TempValues[i] = temp.Substring(0, Length); // Username
        //        temp = temp.Substring(Length);

        //        TempValues[i + 1] = temp.Substring(0, 6); // Scores
        //        temp = temp.Substring(6);
        //    }

        //    _Values = new string[i];

        //    for (int j = 0; j < i; j++)
        //    {
        //        _Values[j] = TempValues[j];
        //    }
        //}

    //    public void ParameteredPersonalState()
    //    {
    //        string temp = _StringedMessage;
    //        temp = temp.Substring(3); // Skipping the Message Code.

    //        _Values = new string[4];
    //        _Values[0] = temp.Substring(0, 4); // Number of Games Played
    //        temp = temp.Substring(4);

    //        _Values[1] = temp.Substring(0, 6); // Number of Correct Answers
    //        temp = temp.Substring(6);

    //        _Values[2] = temp.Substring(0, 6); // Number of Wrong Answers
    //        temp = temp.Substring(6);

    //        _Values[3] = temp; // Average Time for answer
    //    }
    //}
    #endregion




class RoomData
    {
        public int id;
        public string name;
        public int maxPlayers;
        public int numOfQuestions;
        public int timePerQuestion;
        public int isActive;

        public RoomData(int id, string name, int maxPlayers, int numOfQuestions, int timePerQuestion, int isActive)
        {
            this.id = id;
            this.name = name;
            this.maxPlayers = maxPlayers;
            this.numOfQuestions = numOfQuestions;
            this.timePerQuestion = timePerQuestion;
            this.isActive = isActive;
        }
    }

    // Response classes

    class statusResponse
    {
        public int status;

        public statusResponse(int status)
        {
            this.status = status;
        }
    }

    class GetRoomsResponse
    {
        public int status;
        public RoomData[] rooms;

        public GetRoomsResponse(int status, RoomData[] rooms)
        {
            this.status = status;
            this.rooms = rooms;
        }
    }

    class GetPlayersInRoomResponse
    {
        public string[] players;

        public GetPlayersInRoomResponse(string[] players)
        {
            this.players = players;
        }
    }

    class getHighScoreResponse
    {
        public int status;
        public string[] scores;

        public getHighScoreResponse(int status, string[] statistics)
        {
            this.status = status;
            this.scores = statistics;
        }
    }

    class getPersonalStatsResponse
    {
        public int status;
        public string[] statistics;

        public getPersonalStatsResponse(int status, string[] statistics)
        {
            this.status = status;
            this.statistics = statistics;
        }
    }

    class GetRoomStateResponse
    {
        public int status;
        public int hasGameBegun;
        public string[] players;
        public int questionCount;
        public int answerTimeout;

        public GetRoomStateResponse(int status, int hasGameBegun, string[] players, int questionCount, int answerTimeout)
        {
            this.status = status;
            this.hasGameBegun = hasGameBegun;
            this.players = players;
            this.questionCount = questionCount;
            this.answerTimeout = answerTimeout;
        }
    }

    class ErrorResponse
    {
        public string message;

        public ErrorResponse(string message)
        {
            this.message = message;
        }
    }

    class GetQuestionResponse
    {
        public int status;
        public string question;
        public Dictionary<int, string> answers;

        public GetQuestionResponse(int status, string question, Dictionary<int, string> answers)
        {
            this.status = status;
            this.question = question;
            this.answers = answers;
        }
    }

    class SubmitAnswerResponse
    {
        public int status;
        public int correctAnswerId;

        public SubmitAnswerResponse(int status, int correctAnswerId)
        {
            this.status = status;
            this.correctAnswerId = correctAnswerId;
        }
    }

    class PlayerResult
    {
        public string username;
        public int correctAnswerCount;
        public int wrongAnswerCount;
        public int averageAnswerTime;

        public PlayerResult(string username, int correctAnswerCount, int worngAnswerCount, int averageAnswerTime)
        {
            this.username = username;
            this.correctAnswerCount = correctAnswerCount;
            this.wrongAnswerCount = worngAnswerCount;
            this.averageAnswerTime = averageAnswerTime;
        }
    }

    class GetGameResultsResponse
    {
        public int status;
        public PlayerResult[] results;

        public GetGameResultsResponse(int status, PlayerResult[] results)
        {
            this.status = status;
            this.results = results;
        }
    }

    class HasGameEndedResponse
    {
        public int hasEnded;
        public HasGameEndedResponse(int i)
        {
            hasEnded = i;
        }
    }

    // Request classes

    class LoginRequest
    {
        public string username;
        public string password;

        public LoginRequest(string username, string password)
        {
            this.username = username;
            this.password = password;
        }
    }

    class SignupRequest
    {
        public string username;
        public string password;
        public string email;

        public SignupRequest(string username, string password, string email)
        {
            this.username = username;
            this.password = password;
            this.email = email;
        }
    }

    class GetPlayersInRoomRequest
    {
        public int roomID;

        public GetPlayersInRoomRequest(int roomID)
        {
            this.roomID = roomID;
        }
    }

    class JoinRoomRequest
    {
        public int roomID;

        public JoinRoomRequest(int roomID)
        {
            this.roomID = roomID;
        }
    }

    class CreateRoomRequest
    {
        public string roomName;
        public int maxUsers;
        public int questionCount;
        public int answerTimeOut;

        public CreateRoomRequest(string roomName, int maxUsers, int questionCount, int answerTimeOut)
        {
            this.roomName = roomName;
            this.maxUsers = maxUsers;
            this.questionCount = questionCount;
            this.answerTimeOut = answerTimeOut;
        }
    }

    class SubmitAnswerRequest
    {
        public int answerId;
        public int timeToAnswer;
        public SubmitAnswerRequest(int answerId,int time)
        {
            this.answerId = answerId;
            this.timeToAnswer = time; 
        }
    }



    #region Codes
    public static class ClientCodes
    {
        public const int SignIn = 7;
        //public const int ForgotPassword = 202;
        public const int SignUp = 6;
        public const int AllRoomsList = 9;
        public const int AllRoomUsers = 10;
        public const int JoinRoom = 11;
        public const int LeaveRoom = 18;
        public const int CreateRoom = 8;
        public const int CloseRoom = 15;
        public const int StartGame = 16;
        public const int Answer = 20;
        public const int LeaveGame = 21;
        public const int BestScores = 14;
        public const int PersonalState = 13;
        public const int GetQuestion = 22;
        public const int LeaveApp = 33;
        public const int UpdateRoom = 17;
        public const int hasGameEnded = 23;
        public const int getResults = 19;
        public const int success = 1;
        public const int fail = 0;


    }

    public static class ServerCodes
    {
        #region SignIn
        public const int SignIn = 7;
        //public const int ForgotPassword = 202;
        public const int SignUp = 6;
        public const int AllRoomsList = 9;
        public const int AllRoomUsers = 10;
        public const int JoinRoom = 11;
        public const int LeaveRoom = 18;
        public const int CreateRoom = 8;
        public const int CloseRoom = 15;
        public const int StartGame = 16;
        public const int Answer = 20;
        public const int LeaveGame = 21;
        public const int BestScores = 14;
        public const int PersonalState = 13;
        public const int LeaveApp = 299;
        public const int EndGame = 121;
        public const int SendQuestions = 118;
        public const int offline = 0;
        public const int waitingToStart = 1;
        public const int runningGame = 2;


        public const int success = 1;
        public const int fail = 0;
    }
    #endregion

    public static class Paths
    {
        public const string SettingsPage = "./Pages/SettingsPage.xaml";
        public const string Home = "./Pages/Home.xaml";
        public const string ErrorPage = "./Pages/ErrorPage.xaml";
        public const string RoomPage = "./Pages/Options/RoomPage.xaml";
        public const string JoinRoom = "./Pages/Options/JoinRoom.xaml";
        public const string CreateRoom = "./Pages/Option/CreateRoom.xaml";
        public const string SignUp = "./Pages/Connection/SignUp.xaml";
        public const string SignOut = "./Pages/Connection/SignOut.xaml";
        public const string Login = "./Pages/Connection/Login.xaml";
        public const string GamePage = "./Pages/Options/Game.xaml";
        public const string Scores = "./Pages/Options/Scores.xaml";
        public const string BestScores = "./Pages/Options/BestScores.xaml";
    }
}

#endregion