#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace NPC_edit {

	/// <summary>
	/// Summary for Edit_nodeA
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Edit_nodeA : public System::Windows::Forms::Form
	{
	public:
		Edit_nodeA(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

		bool edit;

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Edit_nodeA()
		{
			if (components)
			{
				delete components;
			}
		}
	public: System::Windows::Forms::Button^  ok;
	protected: 
	public: System::Windows::Forms::Button^  Cancel;
	public: System::Windows::Forms::TextBox^  answer;
	public: System::Windows::Forms::NumericUpDown^  link;

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
			this->ok = (gcnew System::Windows::Forms::Button());
			this->Cancel = (gcnew System::Windows::Forms::Button());
			this->answer = (gcnew System::Windows::Forms::TextBox());
			this->link = (gcnew System::Windows::Forms::NumericUpDown());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->link))->BeginInit();
			this->SuspendLayout();
			// 
			// ok
			// 
			this->ok->Location = System::Drawing::Point(298, 51);
			this->ok->Name = L"ok";
			this->ok->Size = System::Drawing::Size(75, 23);
			this->ok->TabIndex = 3;
			this->ok->Text = L"OK";
			this->ok->UseVisualStyleBackColor = true;
			this->ok->Click += gcnew System::EventHandler(this, &Edit_nodeA::ok_Click);
			// 
			// Cancel
			// 
			this->Cancel->Location = System::Drawing::Point(3, 51);
			this->Cancel->Name = L"Cancel";
			this->Cancel->Size = System::Drawing::Size(75, 23);
			this->Cancel->TabIndex = 2;
			this->Cancel->Text = L"Отмена";
			this->Cancel->UseVisualStyleBackColor = true;
			this->Cancel->Click += gcnew System::EventHandler(this, &Edit_nodeA::Cancel_Click);
			// 
			// answer
			// 
			this->answer->Location = System::Drawing::Point(3, 3);
			this->answer->Multiline = true;
			this->answer->Name = L"answer";
			this->answer->Size = System::Drawing::Size(291, 42);
			this->answer->TabIndex = 0;
			this->answer->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Edit_nodeA::answer_KeyDown);
			// 
			// link
			// 
			this->link->Location = System::Drawing::Point(300, 4);
			this->link->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10000, 0, 0, 0});
			this->link->Name = L"link";
			this->link->Size = System::Drawing::Size(73, 20);
			this->link->TabIndex = 1;
			// 
			// Edit_nodeA
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(377, 78);
			this->ControlBox = false;
			this->Controls->Add(this->link);
			this->Controls->Add(this->answer);
			this->Controls->Add(this->Cancel);
			this->Controls->Add(this->ok);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->Name = L"Edit_nodeA";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"Edit_nodeA";
			this->Shown += gcnew System::EventHandler(this, &Edit_nodeA::Edit_nodeA_Shown);
			this->Load += gcnew System::EventHandler(this, &Edit_nodeA::Edit_nodeA_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->link))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	public: System::Void ok_Click(System::Object^  sender, System::EventArgs^  e)
		{
			edit=true;
			Close();
		}
	private: System::Void Edit_nodeA_Load(System::Object^  sender, System::EventArgs^  e)
		{
				edit=false;
		}
private: System::Void Cancel_Click(System::Object^  sender, System::EventArgs^  e)
		{
			Close();
		}
private: System::Void Edit_nodeA_Shown(System::Object^  sender, System::EventArgs^  e)
		{
			answer->Focus();
		}
private: System::Void answer_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
		{
			if (e->KeyCode==System::Windows::Forms::Keys::Escape) Close();
			if (e->KeyCode==System::Windows::Forms::Keys::F1) { edit=true; Close(); }

			if (e->KeyCode==System::Windows::Forms::Keys::PageUp) { if (link->Value<link->Maximum) link->Value++; }
			if (e->KeyCode==System::Windows::Forms::Keys::PageDown) { if (link->Value>link->Minimum) link->Value--; }
		}
};
}
