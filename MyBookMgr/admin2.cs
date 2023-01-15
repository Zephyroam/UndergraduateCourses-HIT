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
    public partial class admin2 : Form
    {
        public admin2()
        {
            InitializeComponent();
            Table();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            admin21 admin = new admin21();
            admin.ShowDialog();
            Table();
        }

        private void button6_Click(object sender, EventArgs e)
        {
            try
            {
                string id = dataGridView1.SelectedRows[0].Cells[0].Value.ToString();
                string name = dataGridView1.SelectedRows[0].Cells[1].Value.ToString();
                string author = dataGridView1.SelectedRows[0].Cells[2].Value.ToString();
                string press = dataGridView1.SelectedRows[0].Cells[3].Value.ToString();
                string number = dataGridView1.SelectedRows[0].Cells[4].Value.ToString();
                admin22 admin = new admin22(id, name, author, press, number);
                admin.ShowDialog();
                Table();
            }
            catch
            {
                MessageBox.Show("ERROR");
            }
        }

        private void button4_Click(object sender, EventArgs e)
        {
            Table();
        }

        private void admin2_Load(object sender, EventArgs e)
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

        private void button5_Click(object sender, EventArgs e)
        {
            string id = "";
            try
            {
                id = dataGridView1.SelectedRows[0].Cells[0].Value.ToString();//获取书号
            }
            catch
            {
                MessageBox.Show("请在表格选中要删除的图书记录！", "信息提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            try
            {
                DialogResult dr = MessageBox.Show("确认删除吗？", "提示", MessageBoxButtons.OKCancel, MessageBoxIcon.Question);
                if(dr==DialogResult.OK)
                {
                    string sql = $"delete from t_book where id='{id}'";
                    Dao dao = new Dao();
                    if(dao.Execute(sql)>0)
                    {
                        MessageBox.Show("删除成功！");
                        Table();
                    }
                    else
                    {
                        MessageBox.Show("删除失败！");
                    }
                    dao.Close();
                }
            }
            catch
            {
                MessageBox.Show("无法连接到服务器，请稍后再试！", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

        }

        private void dataGridView1_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {

        }

        private void dataGridView1_Click(object sender, EventArgs e)
        {
            try
            {
                label2.Text = dataGridView1.SelectedRows[0].Cells[0].Value.ToString()+" ";
                label2.Text += dataGridView1.SelectedRows[0].Cells[1].Value.ToString();
            }
            catch
            {
                MessageBox.Show("无法连接到服务器，请稍后再试！", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

        }
    }
}
