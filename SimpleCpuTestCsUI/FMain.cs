using System;
using System.Windows.Forms;

namespace SimpleCpuTestCsUI
{
    public partial class FMain : Form
    {
        public FMain()
        {
            InitializeComponent();
            this.Text = "Simple CPU Benchmark UI";
            this.MinimumSize = new System.Drawing.Size(600, 400);

            // Підписуємося на події
            CpuBenchmark.OnLogReceived += AppendLog;
            CpuBenchmark.OnProgressChanged += UpdateProgress;
        }

        private async void btnStart_Click(object sender, EventArgs e)
        {
            btnStart.Enabled = false;
            txtLog.Clear();
            progressBar1.Value = 0;

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
            if (txtLog.InvokeRequired)
            {
                txtLog.Invoke(new Action<string>(AppendLog), message);
            }
            else
            {
                txtLog.AppendText(message);
                txtLog.SelectionStart = txtLog.Text.Length;
                txtLog.ScrollToCaret();
            }
        }

        protected override void OnFormClosing(FormClosingEventArgs e)
        {
            CpuBenchmark.OnLogReceived -= AppendLog;
            CpuBenchmark.OnProgressChanged -= UpdateProgress;
            base.OnFormClosing(e);
        }
    }
}