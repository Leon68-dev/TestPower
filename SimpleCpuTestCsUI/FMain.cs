namespace SimpleCpuTestCsUI
{
    public partial class FMain : Form
    {
        public FMain()
        {
            InitializeComponent();
            this.Text = "Simple CPU Benchmark UI";
            this.Size = new System.Drawing.Size(800, 600);

            // Підписуємося на подію логування
            CpuBenchmark.OnLogReceived += AppendLog;
            CpuBenchmark.OnProgressChanged += UpdateProgress;
        }

        private async void btnStart_Click(object sender, EventArgs e)
        {
            btnStart.Enabled = false;
            txtLog.Clear();
            progressBar1.Value = 0; // Скидаємо прогрес перед стартом

            try
            {
                await CpuBenchmark.RunAsync();
                MessageBox.Show("Benchmark Completed!", "Done", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error: {ex.Message}", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            finally
            {
                btnStart.Enabled = true;
            }
        }

        // Метод для оновлення Progress Bar
        private void UpdateProgress(int value)
        {
            if (progressBar1.InvokeRequired)
            {
                progressBar1.Invoke(new Action<int>(UpdateProgress), value);
            }
            else
            {
                progressBar1.Value = value;
            }
        }


        private void AppendLog(string message)
        {
            // Оскільки подія може прийти з іншого потоку, перевіряємо InvokeRequired
            if (txtLog.InvokeRequired)
            {
                txtLog.Invoke(new Action<string>(AppendLog), message);
            }
            else
            {
                txtLog.AppendText(message);
                // Автопрокрутка вниз
                txtLog.SelectionStart = txtLog.Text.Length;
                txtLog.ScrollToCaret();
            }
        }

        // Відписуємося при закритті форми, щоб уникнути витоків пам'яті
        protected override void OnFormClosing(FormClosingEventArgs e)
        {
            CpuBenchmark.OnLogReceived -= AppendLog;
            CpuBenchmark.OnProgressChanged -= UpdateProgress;
            base.OnFormClosing(e);
        }
    }

}
