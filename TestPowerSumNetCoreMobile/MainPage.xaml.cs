namespace TestPowerSumNetCoreMobile
{
    public partial class MainPage : ContentPage
    {
        int count = 0;

        public MainPage()
        {
            InitializeComponent();
        }

        private void OnCounterClicked(object sender, EventArgs e)
        {
            lblTResult.Text = runTest().ToString("0.00000");
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

    }

}
