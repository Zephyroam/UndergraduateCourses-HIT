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
    public partial class user3 : Form
    {
        public user3()
        {
            InitializeComponent();
            Table();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string id = "";

            try
            {
                id = dataGridView1.SelectedRows[0].Cells[1].Value.ToString();//获取书号
            }
            catch
            {
                MessageBox.Show("请在表格选中要归还的图书！", "信息提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            try
            {
                string no = dataGridView1.SelectedRows[0].Cells[0].Value.ToString();
                DialogResult dr = MessageBox.Show("确认归还？", "提示", MessageBoxButtons.OKCancel, MessageBoxIcon.Question);
                if (dr == DialogResult.OK)
                {
                    string sql = $"delete from t_lend where [no]={no};update t_book set number=number+1 where id='{id}'";
                    Dao dao = new Dao();
                    if (dao.Execute(sql) > 1)
                    {
                        MessageBox.Show($"用户{Data.UName}归还图书{id}成功！");
                        Table();
                    }
                    else
                    {
                        MessageBox.Show("归还失败，请联系管理员处理！");
                    }
                    dao.Close();
                }
            }
            catch
            {
                MessageBox.Show("无法连接到服务器，请稍后再试！", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }


        }
        public void Table()
        {
            try
            {
                dataGridView1.Rows.Clear();//清空旧数据
                Dao dao = new Dao();
                string sql = $"select [no],[bid],[datetime] from t_lend where [uid]='{Data.UID}'";
                IDataReader dc = dao.read(sql);
                while (dc.Read())
                {
                    dataGridView1.Rows.Add(dc[0].ToString(), dc[1].ToString(), dc[2].ToString());
                }
                dc.Close();
                dao.Close();
            }
            catch
            {
                MessageBox.Show("无法连接到服务器，请稍后再试！", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void button4_Click(object sender, EventArgs e)
        {
            Table();
        }
    }
}
