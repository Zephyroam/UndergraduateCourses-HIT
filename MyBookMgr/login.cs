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
    public partial class login : Form
    {
        public login()
        {
            InitializeComponent();
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void button2_Click(object sender, EventArgs e)
        {
            regist re = new regist();
            re.ShowDialog();
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            if(textBoxID.Text!=""&&textBoxpw.Text!="")
            {
                Login();
            }
            else
            {
                MessageBox.Show("输入有空项，请重新输入！");
            }
        }
        //验证是否允许登录
        public void Login()
        {
            Dao dao = new Dao();
            string sql = "";
            if (radioButtonAdmin.Checked == true)
            {
                sql = $"select * from t_admin where id='{textBoxID.Text}' and pw='{textBoxpw.Text}'";
            }
            else if (radioButtonUser.Checked == true)
            {
                sql = $"select * from t_user where id='{textBoxID.Text}' and pw='{textBoxpw.Text}'";
            }
            try
            {
                IDataReader dc = dao.read(sql);
                if (dc.Read())
                {
                    //MessageBox.Show(dc[0].ToString() + dc["name"].ToString());
                    //MessageBox.Show("登陆成功！", "", MessageBoxButtons.OK);
                    this.Hide();
                    if (radioButtonAdmin.Checked == true)
                    {
                        Data.UID = textBoxID.Text;
                        Data.UName = "管理员";
                        admin1 admin = new admin1();
                        admin.ShowDialog();
                    }
                    else if (radioButtonUser.Checked == true)
                    {
                        Data.UID = textBoxID.Text;
                        Data.UName = dc[1].ToString();
                        user1 user = new user1();
                        //this.Hide();
                        user.ShowDialog();
                        //this.Show();
                    }
                    this.Close();
                }
                else
                {
                    MessageBox.Show("登陆失败！请检查用户名、密码和登陆身份。", "登陆失败", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
                dc.Close();
                dao.Close();
            }
            catch
            {
                MessageBox.Show("无法连接到服务器，请稍后再试！", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

        }

        private void textBoxpw_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 13)//回车键登录
            {
                if (textBoxID.Text != "" && textBoxpw.Text != "")
                {
                    Login();
                }
                else
                {
                    MessageBox.Show("输入有空项，请重新输入！");
                }

            }
        }
    }
}
