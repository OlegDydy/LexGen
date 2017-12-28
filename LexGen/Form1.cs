using System;
using System.Collections.Generic;
using System.IO;
using System.Windows.Forms;

namespace LexGen
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            panel1.Height = 0;
            History = new List<string>();
            HistoryId = 0;
            // Saved = 0;
            FileName = "";
        }

        String FileName;
        List<String> History;
        Int32 HistoryId;
        // Int32 Saved;
        Keys PrevKey;
        Boolean Changed;

        private void richTextBox1_TextChanged(object sender, EventArgs e)
        {
        }

        private void mmOpen_Click(object sender, EventArgs e)
        {
            if (Changed)
            {
                Changed = false;
                if (!Save())
                    return;
            }

            if (openDlg.ShowDialog() == DialogResult.OK)
            {
                //Saved = 0;
                HistoryId = 0;
                FileName = openDlg.FileName;
                History.Clear();
                StreamReader input = new StreamReader(FileName);
                Editor.Text = input.ReadToEnd();
                History.Add(Editor.Text);
                input.Close();
            }
        }

        private bool Save()
        {
            if (FileName.Length == 0)
            {
                if (openDlg.ShowDialog() != DialogResult.OK)
                    return false;
                FileName = openDlg.FileName;
            }
            StreamWriter output = new StreamWriter(FileName);
            output.Write(Editor.Text);
            output.Close();
            return true;
        }

        private void pushHistory()
        {
            // Changed = false;
            // History.Capacity = HistoryId;
            // History.Add(Editor.Text);
            // HistoryId++;
        }

        private void Undo()
        {
            // if (Changed)
        }

        private void Redo()
        {

        }

        private void mmSave_Click(object sender, EventArgs e)
        {
            Save();
        }

        private void mmSaveAs_Click(object sender, EventArgs e)
        {
            if (openDlg.ShowDialog() == DialogResult.OK)
            {
                FileName = openDlg.FileName;
                Save();
            }
        }

        private void mmGenerate_Click(object sender, EventArgs e)
        {
            Save();
            String header, body;
            panel1.Height = 0;

            if (LexGenerator.Gen(Editor.Text, out header, out body, Path.GetFileNameWithoutExtension(FileName)) != 0)
            {
                string l = LogBox.Text;
                l += LexGenerator.ErrStr;
                LogBox.Text = l;
                panel1.Height = 120;
            }
            StreamWriter output = new StreamWriter(Path.ChangeExtension(FileName, ".h"));
            output.Write(header);
            output.Close();
            output = new StreamWriter(Path.ChangeExtension(FileName, ".cpp"));
            output.Write(body);
            output.Close();
        }

        private void Editor_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Up || e.KeyCode == Keys.Down ||
                e.KeyCode == Keys.Left || e.KeyCode == Keys.Right ||
                e.KeyCode == Keys.Return
                )
            {
                //Editor.ta
                if (Changed)
                {
                    pushHistory();
                }
            }
            if ((PrevKey == Keys.Back || PrevKey == Keys.Delete))
            {
                pushHistory();
            }
            PrevKey = e.KeyCode;
        }

        private void mmUndo_Click(object sender, EventArgs e)
        {
            Undo();
        }

        private void mmRedo_Click(object sender, EventArgs e)
        {
            Redo();
        }

        About AboutBox = new About();

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AboutBox.Show();
        }

        private void mmCopy_Click(object sender, EventArgs e)
        {
            Clipboard.SetText(Editor.SelectedText);
        }

        private void mmCut_Click(object sender, EventArgs e)
        {
            Clipboard.SetText(Editor.SelectedText);
            int id = Editor.SelectionStart;
            string tmp =
                Editor.Text.Substring(0, id)+
                Editor.Text.Substring(id + Editor.SelectionLength);
            Editor.Text = tmp;
            Editor.SelectionStart = id;
        }

        private void mmPaste_Click(object sender, EventArgs e)
        {
            ;
            int id = Editor.SelectionStart;
            string tmp =
                Editor.Text.Substring(0, id) +
                Clipboard.GetText() +
                Editor.Text.Substring(id + Editor.SelectionLength);
            Editor.Text = tmp;
            Editor.SelectionStart = id;
        }
    }
}
