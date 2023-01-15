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
    public partial class regist : Form
    {
        public regist()
        {
            InitializeComponent();
        }

        private void regist_Load(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            string sex;
            if (radioButton1.Checked == true)
            {
                sex = "男";
            }
            else
            {
                sex = "女";
            }
            if (textBox1.Text == "" || textBox2.Text == "" || textBox4.Text == "" || textBox5.Text == "")
            {
                MessageBox.Show("输入不允许有空项！", "", MessageBoxButtons.OK);
                return;
            }
            if (textBox4.Text != textBox5.Text)
            {
                MessageBox.Show("两次输入的密码不同！", "", MessageBoxButtons.OK);
                return;
            }
            try
            {
                Dao dao = new Dao();
                string sql = $"select * from t_user where id='{textBox1.Text}'";
                IDataReader dc = dao.read(sql);
                if (dc.Read())
                {
                    MessageBox.Show("用户名已被占用！", "", MessageBoxButtons.OK);
                    return;
                }
                dc.Close();
                dao.Close();
            }
            catch
            {
                MessageBox.Show("无法连接到服务器，请稍后再试！", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

            try
            {
                Dao dao = new Dao();
                string sql = $"insert into t_user values('{textBox1.Text}','{textBox2.Text}','{sex}','{textBox4.Text}')";
                if (dao.Execute(sql) > 0)
                {
                    MessageBox.Show("注册成功", "", MessageBoxButtons.OK);
                    this.Close();
                }
            }
            catch
            {
                MessageBox.Show("无法连接到服务器，请稍后再试！", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void textBox1_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar != '\b')//允许输入退格键 
            {
                if ((e.KeyChar < '0') || (e.KeyChar > 'z') || (e.KeyChar > '9' && e.KeyChar < 'A') || 
                    (e.KeyChar > 'Z' && e.KeyChar < 'a'))//允许输入数字和字母 
                {
                    e.Handled = true;
                }
            }
        }

        private void textBox4_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar != '\b')//允许输入退格键 
            {
                if ((e.KeyChar < '0') || (e.KeyChar > 'z') || (e.KeyChar > '9' && e.KeyChar < 'A') ||
                    (e.KeyChar > 'Z' && e.KeyChar < 'a'))//允许输入数字和字母 
                {
                    e.Handled = true;
                }
            }
        }
        private void textBox5_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar != '\b')//允许输入退格键 
            {
                if ((e.KeyChar < '0') || (e.KeyChar > 'z') || (e.KeyChar > '9' && e.KeyChar < 'A') ||
                    (e.KeyChar > 'Z' && e.KeyChar < 'a'))//允许输入数字和字母 
                {
                    e.Handled = true;
                }
            }
        }
    }
}
