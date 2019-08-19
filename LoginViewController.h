#pragma once
#include"resource.h"
#include"Utils.h"
#include"ILoginViewOutput.h"
#include"UIDialogViewController.h"

INT_PTR WINAPI LoginDlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

namespace DrvFtaeAlarm {

	class LoginViewController final: public std::enable_shared_from_this<LoginViewController>, public UIDialogViewController, public ILoginViewInput {
	public:
		LoginViewController(const std::shared_ptr<UIDialogViewController>& parent, const std::shared_ptr<ILoginViewOutput> &output);
		LoginViewController(LoginViewController&& rhs) = delete;
		LoginViewController& operator=(LoginViewController&& rhs) = delete;
		LoginViewController(const LoginViewController& src) = delete;
		LoginViewController& operator=(const LoginViewController& rhs) = delete;
		void setupInitialState() override;
		void AddChildViewController(const std::shared_ptr<UIDialogViewController>& child) override;
		void LoadServerList(const std::vector<std::string>& servers) override;
		void LoadDatabasesList(const std::vector<std::string>& databases) override;
		void LoadConnectionSettings(const ConnectionAttributes& attributes) override;
		void WarningMessage(std::string message) override;
		void StartLoading() override;
		void StopLoading() override;
		void AddToParentView() override;
		void CloseView() override;
		void ShowView() override;
		void HideView() override;
		~LoginViewController();
		void LoadSeverInstances();
		void VerifyLogin();
		void VerifyPassword();
		void SaveLogin();
		void SavePassword();
		void SaveServerName();
		void SaveDatabaseName();
		void ChooseServer();
		void ConnectToServer();
		void ChooseAuthentication();
		void ChooseDatabase();
		void CheckConnectionToDatabase();
		void SetAlarmReportType();
		void SetEventReportType();
	private:
		std::shared_ptr<ILoginViewOutput> presenter;
	};

}

