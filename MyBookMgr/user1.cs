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
    public partial class user1 : Form
    {
        public user1()
        {
            InitializeComponent();
            label1.Text = $"欢迎{Data.UName}登录";
        }

        private void menuStrip1_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {

        }

        private void 图书查看与借阅ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            user2 user = new user2();
            user.ShowDialog();
        }

        private void 我的图书与归还ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            user3 user = new user3();
            user.ShowDialog();
        }

        private void 退出ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void user1_Load(object sender, EventArgs e)
        {

        }

        private void 帮助ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show("请联系xyu.shi@outlook.com");
        }


    }
}
