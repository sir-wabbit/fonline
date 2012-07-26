#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace NPC_edit {

	/// <summary>
	/// Summary for Edit_node
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Edit_node : public System::Windows::Forms::Form
	{
	public:
		Edit_node(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}
	public: System::Windows::Forms::ComboBox^  cbNot_answ;
	public: System::Windows::Forms::NumericUpDown^  numTime_answ;

	public: 


		bool edit;

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Edit_node()
		{
			if (components)
			{
				delete components;
			}
		}
	public: System::Windows::Forms::RichTextBox^  text;
	public: System::Windows::Forms::NumericUpDown^  num;
	protected: 

	protected: 

	public: System::Windows::Forms::Button^  cancel;
	public: System::Windows::Forms::Button^  ok;


	public:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->text = (gcnew System::Windows::Forms::RichTextBox());
			this->num = (gcnew System::Windows::Forms::NumericUpDown());
			this->cancel = (gcnew System::Windows::Forms::Button());
			this->ok = (gcnew System::Windows::Forms::Button());
			this->cbNot_answ = (gcnew System::Windows::Forms::ComboBox());
			this->numTime_answ = (gcnew System::Windows::Forms::NumericUpDown());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->num))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numTime_answ))->BeginInit();
			this->SuspendLayout();
			// 
			// text
			// 
			this->text->Location = System::Drawing::Point(3, 2);
			this->text->Name = L"text";
			this->text->Size = System::Drawing::Size(292, 146);
			this->text->TabIndex = 0;
			this->text->Text = L"";
			this->text->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Edit_node::text_KeyDown);
			this->text->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Edit_node::text_KeyPress);
			// 
			// num
			// 
			this->num->Location = System::Drawing::Point(84, 154);
			this->num->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {999, 0, 0, 0});
			this->num->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->num->Name = L"num";
			this->num->Size = System::Drawing::Size(128, 20);
			this->num->TabIndex = 1;
			this->num->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			// 
			// cancel
			// 
			this->cancel->Location = System::Drawing::Point(3, 154);
			this->cancel->Name = L"cancel";
			this->cancel->Size = System::Drawing::Size(75, 23);
			this->cancel->TabIndex = 2;
			this->cancel->Text = L"Отмена";
			this->cancel->UseVisualStyleBackColor = true;
			this->cancel->Click += gcnew System::EventHandler(this, &Edit_node::cancel_Click);
			this->cancel->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Edit_node::cancel_KeyDown);
			// 
			// ok
			// 
			this->ok->Location = System::Drawing::Point(220, 154);
			this->ok->Name = L"ok";
			this->ok->Size = System::Drawing::Size(75, 23);
			this->ok->TabIndex = 3;
			this->ok->Text = L"OK";
			this->ok->UseVisualStyleBackColor = true;
			this->ok->Click += gcnew System::EventHandler(this, &Edit_node::ok_Click);
			// 
			// cbNot_answ
			// 
			this->cbNot_answ->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cbNot_answ->FormattingEnabled = true;
			this->cbNot_answ->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"Завершить диалог", L"Начать атаку"});
			this->cbNot_answ->Location = System::Drawing::Point(3, 180);
			this->cbNot_answ->Name = L"cbNot_answ";
			this->cbNot_answ->Size = System::Drawing::Size(209, 21);
			this->cbNot_answ->TabIndex = 4;
			// 
			// numTime_answ
			// 
			this->numTime_answ->Location = System::Drawing::Point(220, 180);
			this->numTime_answ->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {3000000, 0, 0, 0});
			this->numTime_answ->Name = L"numTime_answ";
			this->numTime_answ->Size = System::Drawing::Size(75, 20);
			this->numTime_answ->TabIndex = 5;
			this->numTime_answ->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {60000, 0, 0, 0});
			// 
			// Edit_node
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(299, 205);
			this->ControlBox = false;
			this->Controls->Add(this->numTime_answ);
			this->Controls->Add(this->cbNot_answ);
			this->Controls->Add(this->ok);
			this->Controls->Add(this->cancel);
			this->Controls->Add(this->num);
			this->Controls->Add(this->text);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->Name = L"Edit_node";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"Edit_node";
			this->Shown += gcnew System::EventHandler(this, &Edit_node::Edit_node_Shown);
			this->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &Edit_node::Edit_node_KeyUp);
			this->Load += gcnew System::EventHandler(this, &Edit_node::Edit_node_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->num))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numTime_answ))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	public: System::Void Edit_node_Load(System::Object^  sender, System::EventArgs^  e)
		{
			edit=false;
		}
	public: System::Void ok_Click(System::Object^  sender, System::EventArgs^  e)
		{
			edit=true;
			Close();
		}
private: System::Void cancel_Click(System::Object^  sender, System::EventArgs^  e)
		{
			Close();
		}
private: System::Void Edit_node_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
		{
			
		}
private: System::Void text_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e)
		{
			
		}
private: System::Void text_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
		{
			if (e->KeyCode==System::Windows::Forms::Keys::Escape) Close();
			if (e->KeyCode==System::Windows::Forms::Keys::F1) { edit=true; Close(); }

			if (e->KeyCode==System::Windows::Forms::Keys::PageUp) { if (num->Value<num->Maximum) num->Value++; }
			if (e->KeyCode==System::Windows::Forms::Keys::PageDown) { if (num->Value>num->Minimum) num->Value--; }
		}
private: System::Void cancel_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
		{

		}
private: System::Void Edit_node_Shown(System::Object^  sender, System::EventArgs^  e)
		{
			text->Focus();
			text->SelectAll();
		}
};
}
