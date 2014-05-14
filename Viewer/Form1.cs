using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Globalization;
using System.Collections;

namespace Viewer
{
    public partial class Form1 : Form
    {
        string fileName;

        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                fileName = openFileDialog1.FileName;
                BinaryReader br = new BinaryReader(File.Open(openFileDialog1.FileName, FileMode.Open));
                TextWriter tr = new StreamWriter(File.Open(fileName+".dat",FileMode.Create));
                Hashtable offset = new Hashtable();
                Hashtable links  = new Hashtable();

                long num = 0;
                int cur_offset = 0;
                int previous = 0;
                try
                {
                    tr.Write("# extrait du fichier" + fileName+"\n");
                    tr.Write("graph [\n");
                    tr.Write("\tcomment \"generated by viewer\" \n");
                    tr.Write("\tdirected 1\n");
                    //tr.Write("\tid 42\n");
                    //tr.Write("\tlabel "Hello, I am a graph"
                    while (true)
                    {
                        cur_offset = br.ReadInt32();
                        if (cur_offset < 0x70000000 || previous < 0x70000000)
                        {
                            if (!offset.ContainsKey(cur_offset.ToString("X")))
                            {
                                offset.Add(cur_offset.ToString("X"), num);
                                tr.Write("node [\n");
                                tr.Write("       id " + num.ToString() + "\n");
                                tr.Write("        label \"" + cur_offset.ToString("X") + "\" \n");
                                tr.Write("]\n");
                                num++;
                            }

                            if (!links.ContainsKey(cur_offset.ToString("X") + "to" + previous.ToString("X")) && (previous != cur_offset) && offset.ContainsKey(previous.ToString("X")))
                            {
                                links.Add(cur_offset.ToString("X") + "to" + previous.ToString("X"), null);
                                tr.Write("\tedge [\n");
                                tr.Write("\t    source " + offset[previous.ToString("X")] + "\n");
                                tr.Write("\t    target " + offset[cur_offset.ToString("X")] + "\n");
                                tr.Write("\t]\n");
                            }
                        }
                        previous = cur_offset;
                    }

                }
                catch (Exception )
                {

                }
                tr.Write("]\n");
                tr.Close();
                br.Close();
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            refresh_form();
        }

        private void refresh_form()
        {
            this.Refresh();

            Int32 min_offset = 0, max_offset = 0, curr_offset = 0;
            float v_mod, h_mod, num;
            BinaryReader br = new BinaryReader(File.Open(fileName, FileMode.Open));
            num = 0;
            try
            {
                br.BaseStream.Seek(0, SeekOrigin.Begin);
                curr_offset = br.ReadInt32();
                min_offset = curr_offset;
                max_offset = curr_offset;
                num++;

                while (true)
                {
                    curr_offset = br.ReadInt32();
                    num++;
                    if (curr_offset < min_offset) min_offset = curr_offset;
                    if (curr_offset > max_offset) max_offset = curr_offset;
                }
            }
            catch (System.IO.EndOfStreamException exp)
            {
                //MessageBox.Show("Exception caught!");
            }
            tbMinRO.Text = min_offset.ToString("X");
            tbMaxRO.Text = max_offset.ToString("X");

            try
            {
                min_offset = Int32.Parse(tbMinW.Text, NumberStyles.AllowHexSpecifier);
            }
            catch (Exception)
            {
            }

            try
            {
                max_offset = Int32.Parse(tbMaxW.Text, NumberStyles.AllowHexSpecifier);
            }
            catch (Exception)
            {
            }



            v_mod = (max_offset - min_offset) / (pVisual.Height-10);
            h_mod = num / (pVisual.Width-5);

            //MessageBox.Show("Min Offset="+min_offset.ToString("X")+" Max Offset="+max_offset.ToString("X")+"\n"+"v_mod= " + v_mod.ToString() + " h_mod= "+h_mod.ToString());


            Graphics test = pVisual.CreateGraphics();

            Pen dot = new Pen(new SolidBrush(Color.Teal), 1);

            num = 0;

            try
            {
                br.BaseStream.Seek(0, SeekOrigin.Begin);
                while (true)
                {
                    curr_offset = br.ReadInt32();
                    if (curr_offset > max_offset) continue;
                    if (curr_offset < min_offset) continue;

                    test.DrawRectangle(dot, 5+(int)(num / h_mod), 10+(int)((curr_offset - min_offset) / v_mod), 1, 1);
                    num++;
                }
            }
            catch (System.IO.EndOfStreamException exp)
            {
                //MessageBox.Show("Exception caught!");
            }
            br.Close(); 
        }
    }
}