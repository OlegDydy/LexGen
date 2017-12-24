namespace LexGen
{
    partial class Form1
    {
        /// <summary>
        /// Обязательная переменная конструктора.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Освободить все используемые ресурсы.
        /// </summary>
        /// <param name="disposing">истинно, если управляемый ресурс должен быть удален; иначе ложно.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Код, автоматически созданный конструктором форм Windows

        /// <summary>
        /// Требуемый метод для поддержки конструктора — не изменяйте 
        /// содержимое этого метода с помощью редактора кода.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.mmFile = new System.Windows.Forms.ToolStripMenuItem();
            this.mmCreate = new System.Windows.Forms.ToolStripMenuItem();
            this.mmOpen = new System.Windows.Forms.ToolStripMenuItem();
            this.mmSave = new System.Windows.Forms.ToolStripMenuItem();
            this.mmSaveAs = new System.Windows.Forms.ToolStripMenuItem();
            this.mmEdit = new System.Windows.Forms.ToolStripMenuItem();
            this.mEdit = new System.Windows.Forms.ToolStripMenuItem();
            this.mmUndo = new System.Windows.Forms.ToolStripMenuItem();
            this.mmRedo = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.mmCopy = new System.Windows.Forms.ToolStripMenuItem();
            this.mmCut = new System.Windows.Forms.ToolStripMenuItem();
            this.mmPaste = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.mmGenerate = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.panel1 = new System.Windows.Forms.Panel();
            this.LogBox = new System.Windows.Forms.RichTextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.splitter1 = new System.Windows.Forms.Splitter();
            this.openDlg = new System.Windows.Forms.OpenFileDialog();
            this.saveDlg = new System.Windows.Forms.SaveFileDialog();
            this.Editor = new System.Windows.Forms.RichTextBox();
            this.menuStrip1.SuspendLayout();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mmFile,
            this.mEdit,
            this.aboutToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(913, 24);
            this.menuStrip1.TabIndex = 1;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // mmFile
            // 
            this.mmFile.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mmCreate,
            this.mmOpen,
            this.mmSave,
            this.mmSaveAs,
            this.mmEdit});
            this.mmFile.Name = "mmFile";
            this.mmFile.Size = new System.Drawing.Size(37, 20);
            this.mmFile.Text = "File";
            // 
            // mmCreate
            // 
            this.mmCreate.Name = "mmCreate";
            this.mmCreate.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.N)));
            this.mmCreate.Size = new System.Drawing.Size(195, 22);
            this.mmCreate.Text = "Create New";
            // 
            // mmOpen
            // 
            this.mmOpen.Name = "mmOpen";
            this.mmOpen.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.O)));
            this.mmOpen.Size = new System.Drawing.Size(195, 22);
            this.mmOpen.Text = "Open...";
            this.mmOpen.Click += new System.EventHandler(this.mmOpen_Click);
            // 
            // mmSave
            // 
            this.mmSave.Name = "mmSave";
            this.mmSave.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.S)));
            this.mmSave.Size = new System.Drawing.Size(195, 22);
            this.mmSave.Text = "Save";
            this.mmSave.Click += new System.EventHandler(this.mmSave_Click);
            // 
            // mmSaveAs
            // 
            this.mmSaveAs.Name = "mmSaveAs";
            this.mmSaveAs.ShortcutKeys = ((System.Windows.Forms.Keys)(((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Shift) 
            | System.Windows.Forms.Keys.S)));
            this.mmSaveAs.Size = new System.Drawing.Size(195, 22);
            this.mmSaveAs.Text = "Save As...";
            this.mmSaveAs.Click += new System.EventHandler(this.mmSaveAs_Click);
            // 
            // mmEdit
            // 
            this.mmEdit.Name = "mmEdit";
            this.mmEdit.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Q)));
            this.mmEdit.Size = new System.Drawing.Size(195, 22);
            this.mmEdit.Text = "Exit";
            // 
            // mEdit
            // 
            this.mEdit.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mmUndo,
            this.mmRedo,
            this.toolStripSeparator1,
            this.mmCopy,
            this.mmCut,
            this.mmPaste,
            this.toolStripSeparator2,
            this.mmGenerate});
            this.mEdit.Name = "mEdit";
            this.mEdit.Size = new System.Drawing.Size(39, 20);
            this.mEdit.Text = "Edit";
            // 
            // mmUndo
            // 
            this.mmUndo.Name = "mmUndo";
            this.mmUndo.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Z)));
            this.mmUndo.Size = new System.Drawing.Size(144, 22);
            this.mmUndo.Text = "Undo";
            this.mmUndo.Click += new System.EventHandler(this.mmUndo_Click);
            // 
            // mmRedo
            // 
            this.mmRedo.Name = "mmRedo";
            this.mmRedo.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Y)));
            this.mmRedo.Size = new System.Drawing.Size(144, 22);
            this.mmRedo.Text = "Redo";
            this.mmRedo.Click += new System.EventHandler(this.mmRedo_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(141, 6);
            // 
            // mmCopy
            // 
            this.mmCopy.Name = "mmCopy";
            this.mmCopy.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.C)));
            this.mmCopy.Size = new System.Drawing.Size(144, 22);
            this.mmCopy.Text = "Copy";
            // 
            // mmCut
            // 
            this.mmCut.Name = "mmCut";
            this.mmCut.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.X)));
            this.mmCut.Size = new System.Drawing.Size(144, 22);
            this.mmCut.Text = "Cut";
            // 
            // mmPaste
            // 
            this.mmPaste.Name = "mmPaste";
            this.mmPaste.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.V)));
            this.mmPaste.Size = new System.Drawing.Size(144, 22);
            this.mmPaste.Text = "Paste";
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(141, 6);
            // 
            // mmGenerate
            // 
            this.mmGenerate.Name = "mmGenerate";
            this.mmGenerate.ShortcutKeys = System.Windows.Forms.Keys.F5;
            this.mmGenerate.Size = new System.Drawing.Size(144, 22);
            this.mmGenerate.Text = "Generate";
            this.mmGenerate.Click += new System.EventHandler(this.mmGenerate_Click);
            // 
            // aboutToolStripMenuItem
            // 
            this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            this.aboutToolStripMenuItem.ShortcutKeys = System.Windows.Forms.Keys.F1;
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size(52, 20);
            this.aboutToolStripMenuItem.Text = "About";
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.LogBox);
            this.panel1.Controls.Add(this.label1);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panel1.Location = new System.Drawing.Point(0, 400);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(913, 96);
            this.panel1.TabIndex = 3;
            // 
            // LogBox
            // 
            this.LogBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.LogBox.Location = new System.Drawing.Point(0, 22);
            this.LogBox.Name = "LogBox";
            this.LogBox.Size = new System.Drawing.Size(913, 74);
            this.LogBox.TabIndex = 1;
            this.LogBox.Text = "";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(3, 6);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(42, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Output:";
            // 
            // splitter1
            // 
            this.splitter1.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.splitter1.Location = new System.Drawing.Point(0, 397);
            this.splitter1.Name = "splitter1";
            this.splitter1.Size = new System.Drawing.Size(913, 3);
            this.splitter1.TabIndex = 4;
            this.splitter1.TabStop = false;
            // 
            // openDlg
            // 
            this.openDlg.FileName = "openFileDialog1";
            // 
            // Editor
            // 
            this.Editor.Dock = System.Windows.Forms.DockStyle.Fill;
            this.Editor.Font = new System.Drawing.Font("Liberation Mono", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.Editor.Location = new System.Drawing.Point(0, 24);
            this.Editor.Margin = new System.Windows.Forms.Padding(0);
            this.Editor.Name = "Editor";
            this.Editor.Size = new System.Drawing.Size(913, 373);
            this.Editor.TabIndex = 2;
            this.Editor.Text = "";
            this.Editor.WordWrap = false;
            this.Editor.TextChanged += new System.EventHandler(this.richTextBox1_TextChanged);
            this.Editor.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Editor_KeyDown);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(913, 496);
            this.Controls.Add(this.Editor);
            this.Controls.Add(this.splitter1);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.menuStrip1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "Form1";
            this.Text = "Lex Gen";
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem mmFile;
        private System.Windows.Forms.ToolStripMenuItem mmCreate;
        private System.Windows.Forms.ToolStripMenuItem mmOpen;
        private System.Windows.Forms.ToolStripMenuItem mmSave;
        private System.Windows.Forms.ToolStripMenuItem mmSaveAs;
        private System.Windows.Forms.ToolStripMenuItem mmEdit;
        private System.Windows.Forms.ToolStripMenuItem mEdit;
        private System.Windows.Forms.ToolStripMenuItem mmUndo;
        private System.Windows.Forms.ToolStripMenuItem mmRedo;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem mmCopy;
        private System.Windows.Forms.ToolStripMenuItem mmCut;
        private System.Windows.Forms.ToolStripMenuItem mmPaste;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripMenuItem mmGenerate;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.RichTextBox LogBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Splitter splitter1;
        private System.Windows.Forms.OpenFileDialog openDlg;
        private System.Windows.Forms.SaveFileDialog saveDlg;
        private System.Windows.Forms.RichTextBox Editor;
    }
}

