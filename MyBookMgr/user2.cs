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
    public partial class user2 : Form
    {
        public user2()
        {
            InitializeComponent();
            Table();
        }

        private void user2_Load(object sender, EventArgs e)
        {
            try
            {
                label2.Text = dataGridView1.SelectedRows[0].Cells[0].Value.ToString() + " ";
                label2.Text += dataGridView1.SelectedRows[0].Cells[1].Value.ToString();
            }
            catch
            {
                MessageBox.Show("无法连接到服务器，请稍后再试！", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        //从数据库读取数据显示在表格控件之中
        public void Table()
        {
            try
            {
                dataGridView1.Rows.Clear();//清空旧数据
                Dao dao = new Dao();
                string sql = "select * from t_book";
                IDataReader dc = dao.read(sql);
                while (dc.Read())
                {
                    dataGridView1.Rows.Add(dc[0].ToString(), dc[1].ToString(), dc[2].ToString(), dc[3].ToString(), dc[4].ToString());
                }
                dc.Close();
                dao.Close();
            }
            catch
            {
                MessageBox.Show("无法连接到服务器，请稍后再试！", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        //根据书号显示数据
        public void TableID()
        {
            try
            {
                dataGridView1.Rows.Clear();//清空旧数据
                Dao dao = new Dao();
                string sql = $"select * from t_book where id='{textBox1.Text.ToString()}'";
                IDataReader dc = dao.read(sql);
                while (dc.Read())
                {
                    dataGridView1.Rows.Add(dc[0].ToString(), dc[1].ToString(), dc[2].ToString(), dc[3].ToString(), dc[4].ToString());
                }
                dc.Close();
                dao.Close();
            }
            catch
            {
                MessageBox.Show("无法连接到服务器，请稍后再试！", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        //根据书名显示数据
        public void TableName()
        {
            try
            {
                dataGridView1.Rows.Clear();//清空旧数据
                Dao dao = new Dao();
                string sql = $"select * from t_book where name like '%{textBox2.Text.ToString()}%'";
                IDataReader dc = dao.read(sql);
                while (dc.Read())
                {
                    dataGridView1.Rows.Add(dc[0].ToString(), dc[1].ToString(), dc[2].ToString(), dc[3].ToString(), dc[4].ToString());
                }
                dc.Close();
                dao.Close();
            }
            catch
            {
                MessageBox.Show("无法连接到服务器，请稍后再试！", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }


        //借出图书
        private void button1_Click(object sender, EventArgs e)
        {
            string id = "";
            try
            {
                id = dataGridView1.SelectedRows[0].Cells[0].Value.ToString();//获取书号
            }
            catch
            {
                MessageBox.Show("请在表格选中要借出的图书！", "信息提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            try
            {
                int number = Convert.ToInt32(dataGridView1.SelectedRows[0].Cells[4].Value.ToString());
                if(number == 0)
                {
                    MessageBox.Show("图书数量不足，请联系管理人员购入", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
                else
                {
                    DialogResult dr = MessageBox.Show("确认借出？", "提示", MessageBoxButtons.OKCancel, MessageBoxIcon.Question);
                    if (dr == DialogResult.OK)
                    {
                        string sql = $"insert into t_lend ([uid],[bid],[datetime]) values('{Data.UID}','{id}',GETDATE());update t_book set number=number-1 where id='{id}'";
                        Dao dao = new Dao();
                        if (dao.Execute(sql) > 1)
                        {
                            MessageBox.Show($"用户{Data.UName}借出图书{id}成功！");
                            Table();
                        }
                        else
                        {
                            MessageBox.Show("借出失败！");
                        }
                        dao.Close();
                    }
                }
            }
            catch
            {
                MessageBox.Show("无法连接到服务器，请稍后再试！", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void button6_Click(object sender, EventArgs e)
        {

        }

        private void button5_Click(object sender, EventArgs e)
        {

        }

        private void button4_Click(object sender, EventArgs e)
        {
            Table();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            TableID();
            textBox1.Clear();
            textBox2.Clear();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            TableName();
            textBox1.Clear();
            textBox2.Clear();
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void dataGridView1_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {

        }

        private void dataGridView1_Click(object sender, EventArgs e)
        {
            try
            {
                label2.Text = dataGridView1.SelectedRows[0].Cells[0].Value.ToString() + " ";
                label2.Text += dataGridView1.SelectedRows[0].Cells[1].Value.ToString();
            }
            catch
            {
                MessageBox.Show("无法连接到服务器，请稍后再试！", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

    }
}
