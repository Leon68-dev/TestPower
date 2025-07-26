namespace TestPowerSumNetCoreWinFrm
{
    partial class FMain
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            btnRunTest = new Button();
            label1 = new Label();
            lblResult = new Label();
            SuspendLayout();
            // 
            // btnRunTest
            // 
            btnRunTest.Location = new Point(158, 50);
            btnRunTest.Name = "btnRunTest";
            btnRunTest.Size = new Size(94, 29);
            btnRunTest.TabIndex = 0;
            btnRunTest.Text = "Test";
            btnRunTest.UseVisualStyleBackColor = true;
            btnRunTest.Click += btnRunTest_Click;
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new Point(2, 9);
            label1.Name = "label1";
            label1.Size = new Size(295, 20);
            label1.TabIndex = 1;
            label1.Text = "Time for calculate sum 1 to 1 000 000 (sec):";
            // 
            // lblResult
            // 
            lblResult.AutoSize = true;
            lblResult.Location = new Point(303, 9);
            lblResult.Name = "lblResult";
            lblResult.Size = new Size(17, 20);
            lblResult.TabIndex = 2;
            lblResult.Text = "0";
            // 
            // FMain
            // 
            AutoScaleDimensions = new SizeF(8F, 20F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(408, 91);
            Controls.Add(lblResult);
            Controls.Add(label1);
            Controls.Add(btnRunTest);
            FormBorderStyle = FormBorderStyle.FixedToolWindow;
            MaximizeBox = false;
            MinimizeBox = false;
            Name = "FMain";
            Opacity = 0.8D;
            StartPosition = FormStartPosition.CenterScreen;
            Text = "Test calc SUM";
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private Button btnRunTest;
        private Label label1;
        private Label lblResult;
    }
}
