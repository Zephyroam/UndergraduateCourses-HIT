using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Data.SqlClient;
using System.Windows.Forms;

namespace BookMgr
{
    class Dao
    {
        SqlConnection sc;
        public SqlConnection connect()
        {
            string str = @"server=Dedsec;database=BookDB;Trusted_Connection=SSPI";
            //string str = @"Data Soure=DEDSECR-S;Initial Catalog=BookDB;Intergrated Security=True";
            sc = new SqlConnection(str);
            sc.Open();
            return sc;
        }
        public SqlCommand command(string sql)
        {
            SqlCommand cmd = new SqlCommand(sql, connect());
             return cmd;
        }
        public int Execute(string sql)
        {
            return command(sql).ExecuteNonQuery();
        }
        public SqlDataReader read(string sql)
        {
            return command(sql).ExecuteReader();
        }
        public void Close()
        {
            sc.Close();
        }
    }
}
