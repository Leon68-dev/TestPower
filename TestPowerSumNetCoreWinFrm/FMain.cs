namespace TestPowerSumNetCoreWinFrm
{
    public partial class FMain : Form
    {
        public FMain()
        {
            InitializeComponent();
        }

        private void check()
        {
            long cc = 0;
            for (long i = 0; i < 1000000; i++)
            {
                cc++;
            }
        }

        private double runTest()
        {
            var ticks = DateTime.Now.Ticks;
            check();
            ticks = DateTime.Now.Ticks - ticks;

            TimeSpan ts = TimeSpan.FromTicks(ticks);
            return ts.TotalSeconds;
        }

        private void btnRunTest_Click(object sender, EventArgs e)
        {
            this.lblResult.Text = runTest().ToString("0.00000"); 
        }
    }
}
