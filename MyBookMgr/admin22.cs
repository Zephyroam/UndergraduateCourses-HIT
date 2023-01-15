using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace BookMgr
{
    public partial class admin22 : Form
    {
        string Id = "";
        string Name = "";
        string Author = "";
        string Press = "";
        string Number = "";
        public admin22()
        {
            InitializeComponent();
        }

        private void admin22_Load(object sender, EventArgs e)
        {

        }

        public admin22(string id,string name,string author,string press,string number)
        {
            InitializeComponent();
            Id = textBox1.Text = id;
            Name = textBox2.Text = name;
            Author = textBox3.Text = author;
            Press = textBox4.Text = press;
            Number = textBox5.Text = number;
        }
        private void button1_Click(object sender, EventArgs e)
        {
            try
            {
                string sql = $"update t_book set id='{textBox1.Text}',[name]='{textBox2.Text}',author='{textBox3.Text}',press='{textBox4.Text}',number={textBox5.Text} where id='{Id}'";
                Dao dao = new Dao();
                if (dao.Execute(sql) > 0)
                {
                    MessageBox.Show("修改成功！");
                    this.Close();
                }
            }
            catch
            {
                MessageBox.Show("无法连接到服务器，请稍后再试！", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            textBox1.Text = Id;
            textBox2.Text = Name;
            textBox3.Text = Author;
            textBox4.Text = Press;
            textBox5.Text = Number;
        }

        private void textBox5_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar != '\b')//允许输入退格键 
            {
                if ((e.KeyChar < '0') || (e.KeyChar > '9'))//允许输入0-9数字 
                {
                    e.Handled = true;
                }
            }
        }

        private void textBox5_TextChanged(object sender, EventArgs e)
        {

        }
    }
}
