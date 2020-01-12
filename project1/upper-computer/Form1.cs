using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.IO.Ports;

namespace winformtest
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        _bad_apple_data bad_apple = new _bad_apple_data();
        private void Form1_Load(object sender, EventArgs e)
        {
            //初始化帧头帧尾
            bad_apple.data[673] = 0XA5;
            bad_apple.data[0] = 0X5A;
            //绑定时间字符串，
            this.label1.DataBindings.Add("Text", bad_apple, "time_string");
            this.textBox1.DataBindings.Add("Text", bad_apple, "image_data_string");
            this.trackBar1.DataBindings.Add("Value", bad_apple, "time");
            this.label2.DataBindings.Add("Text", bad_apple, "serialportstatus");
            //禁止多线程检查（我懒得写线程间通信了hhh）
            Control.CheckForIllegalCrossThreadCalls = false;
            //初始化image数值
            bad_apple.image_data = 1;
            //绑定图片
            this.pictureshow.DataBindings.Add("Image", bad_apple, "picture");
            //在接受超时时会抛出异常
            serialport.ReadTimeout = 100;//设置超时毫秒数

        }
        private void SerialPort_recrive(object sender, SerialDataReceivedEventArgs e)
        {
            if (serialport.ReadByte() != 0XA5)
            {
                return;
            }
            if(bad_apple.image_data == 4380)
            {
                this.play_control.PerformClick();
                return;
            }
            bad_apple.image_data++;
            update_picture();
        }
        private bool openserial_port()
        {
            //若串口已经打开则直接返回
            if (serialport.IsOpen)
            {
                return true;
            }

            string[] str = SerialPort.GetPortNames();
            foreach (string a in str)
            {
                serialport.PortName = a;
                try
                {
                    serialport.Open();
                    bad_apple.serialportstatus = "识别" + serialport.PortName + "中.....";
                    if (serialport.ReadByte() == 0XA5)
                    {
                        bad_apple.serialportstatus = serialport.PortName + "连接成功!";
                        return true;
                    }
                }
                catch (TimeoutException e1)
                {
                    serialport.Close();//打开成功但是超时读取则关闭串口
                }
                catch
                {

                }
                bad_apple.serialportstatus = serialport.PortName + "识别失败!";

            }
            //输出打开失败的提示信息
            if(str.Length > 0)
            {
                MessageBox.Show("所有串口都不是我想要的awsl", "检查一下单片机试试？");
            }
            else
            {
                MessageBox.Show("一个可用串口都莫得qaq", "快插上usb转串口啊");
            }
            return false;
        }
        private void update_picture()
        {
            //pictureshow.Image = bad_apple.picture;
            try
            {
                BinaryReader picture = new BinaryReader(File.Open(@"..\data-file\84x64data\" + bad_apple.image_data_string + ".txt", FileMode.Open));
                picture.Read(bad_apple.data, 1, 672);//读取数据
                picture.Dispose();
            }
            catch (FileNotFoundException e1)//在读取数据失败时给出警告，局部变量不需要清零
            {
                MessageBox.Show("找不到" + bad_apple.image_data_string + ".txt文件了qaq", "(你是不是移动或者删除文件了)");
            }

            
            try
            {
                serialport.Write(bad_apple.data, 0, 674);
            }
            catch
            {

            }

            return;
        }
        private void Play_control(object sender, EventArgs e)
        {
            if (bad_apple.state == "leisure")
            {
                if (!openserial_port())
                {
                    return;
                }
                ((Button)sender).Text = "暂停播放";
                serialport.DataReceived += SerialPort_recrive;//加入事件
                bad_apple.state = "playvideo";
                
            }
            else
            {
                ((Button)sender).Text = "开始播放";
                serialport.DataReceived -= SerialPort_recrive;//加入事件
                bad_apple.state = "leisure";
                
                try
                {
                    serialport.Close();
                }
                catch
                {

                }
            }

        }

        private void trackBar1_Scroll(object sender, EventArgs e)
        {
            if (bad_apple.state == "playvideo"|| (!openserial_port()))
            {
                return;
            }
            bad_apple.image_data = ((TrackBar)sender).Value * 20 + 1;
            update_picture();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (bad_apple.state == "playvideo" || (!openserial_port()))
            {
                return;
            }
            bad_apple.image_data--;
            update_picture();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            //当不处于播放状态且串口成功打开时更新
            if(bad_apple.state == "playvideo" || (!openserial_port()))
            {
                return;
            }
            bad_apple.image_data++;
            update_picture();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            if (bad_apple.state == "playvideo")
            {
                return;
            }
            try
            {
                bad_apple.image_data = Convert.ToInt32(textBox1.Text);
            }
            catch
            {
                MessageBox.Show("- -你看看你你输入的是整数吗", "qaq饶了我吧");

            }
            update_picture();
        }
    }

    public class _bad_apple_data: INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;
        private void SendChangeInfo(string propertyName)
        {
            if (this.PropertyChanged != null) 
            {
                PropertyChanged.Invoke(this, new PropertyChangedEventArgs(propertyName));
            }
        }
        private int _image_number = 1;//记录当前的帧数
        public byte[] data = new byte[674];//用于储存要传输的图像数据
        public string state = "leisure";//保存当前程序所处的状态
        public int time { set; get; }//记录当前食品播放时间
        public string serialportstatus { set; get; }//记录串口情况并输出
        public string image_data_string { set; get; }//以字符串形式记录，用于绑定
        public string time_string { private set; get; }
        public Image picture { private set; get; }
        public int image_data//留给对外的接口
        {
            set
            {
                _image_number = value;
                //在赋值之后检查值是否超出范围，作出限制并在超出限制时提示
                if (_image_number < 1)
                {
                    _image_number = 1;
                    MessageBox.Show("前面已经莫得东西le\nhhhhhhh","qaq");
                }
                else if (_image_number > 4380)
                {
                    _image_number = 4380;
                    MessageBox.Show("后面已经莫得东西le\nhhhhhhh", "qaq");
                }

                //当picture有储存图片时释放它占用的资源


                //更新图片，更新其他数据
                try
                {
                    Image a = Image.FromFile(@"..\data-file\picture\" + _image_number.ToString() + ".jpg", true);
                    if (picture != null)
                    {
                        picture.Dispose();
                    }
                    picture = a;
                }
                catch (FileNotFoundException e1)//当文件被移动时做出提示并处理异常
                {
                    MessageBox.Show("找不到"+ _image_number.ToString() + ".jpg文件了qaq", "(你是不是移动或者删除文件了)");
                    picture = null;
                    return;
                }
                image_data_string = _image_number.ToString();
                time_string = (_image_number / 20 / 60).ToString() + ":" + (_image_number / 20 % 60).ToString();
                time = (_image_number - 1) / 20;
                SendChangeInfo("image_data_string");//产生绑定更新的事件
            }
            get
            {
                return _image_number;
            }
        }
    }
}
