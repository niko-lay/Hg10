import bb.cascades 1.2
import Network.LoginController 1.0
import conf.SettingsController 1.0
import com.netimage 1.0

NavigationPane {
    id: nav
    property variant tpage
    property variant colorPage
    signal done ()
    
	Page {
        id: settingsPage
        
        
        titleBar: TitleBar {
            title: qsTr("Settings")
            dismissAction: ActionItem {
                title: qsTr("Close")
                onTriggered: {
                    // Emit the custom signal here to indicate that this page needs to be closed
                    // The signal would be handled by the page which invoked it
                    nav.done();
                }
            }
            acceptAction: ActionItem {
                title: qsTr("Save")
                onTriggered: {
                    settingsController.save(); 
                    nav.done();
                }
            }
        }
        
        ScrollView {
            id: settingPage
            property string userName;
            
    	    Container {
    	        layout: StackLayout {
    	            orientation: LayoutOrientation.TopToBottom
    	        }
                id: headerContainer
                horizontalAlignment: HorizontalAlignment.Fill
    	        
                function themeStyleToHeaderColor(style){
                    switch (style) {
                        case VisualStyle.Bright:
                            return Color.create(0.96,0.96,0.96);
                        case VisualStyle.Dark: 
                            return Color.create(0.15,0.15,0.15);
                        default :
                            return Color.create(0.96,0.96,0.96);    
                    }
                    return Color.create(0.96,0.96,0.96); 
                }
    	        
                // --------------------------------------------------------------------------
                // Login settings
                
                Container {
                    minHeight: 100
                    maxHeight: 100
                }
                
                ImageView {
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                    id: avatarOwnImg
                    scalingMethod: ScalingMethod.AspectFit
                    minHeight: 200
                    maxHeight: 200
                    minWidth: 200
                    maxWidth: 200
                    image: trackerOwn.image
                    
                    attachedObjects: [
                        NetImageTracker {
                            id: trackerOwn
                            
                            source: settingsController.avatar                                    
                        } 
                    ]
                }
                
    	        Label {
    	            id: userLabel
                    text: qsTr("User: ") + settingsController.userName
                    horizontalAlignment: HorizontalAlignment.Center
                }
    
    	        
    	        // Commit button
    	        Button {
    	            id: loginButton
    	            text: qsTr("Connect")
    	            horizontalAlignment: HorizontalAlignment.Fill
    	            onClicked: {
                        if(!tpage)
                        	tpage = loginPage.createObject();
                        nav.push(tpage);
    	            }
    	            visible: !loginController.isLogged()
    	        }
    	        
    	        
    	        Button {
    	            id: logOutButton
    	            text: qsTr("log out");
    	            horizontalAlignment: HorizontalAlignment.Fill
    	            onClicked: {
    	                loginController.logOut();
    	                loginButton.setVisible(true);
    	                logOutButton.setVisible(false);
                        userLabel.setText(qsTr("User: "));
    	            }
    	            visible: loginController.isLogged()
    	        }
    	        
    	        Divider { }
    	        
                // --------------------------------------------------------------------------
                // Theme setting
                DropDown {
                    id: theme
                    title: qsTr("Visual Theme")
                    options: [
                        Option {
                            text: qsTr("Bright")
                            selected: settingsController.theme == 1 ? true : false
                            value: 1
                        },
                        Option {
                            text: qsTr("Dark")
                            selected: settingsController.theme == 2 ? true : false
                            value: 2
                        } 
                    ]
                    selectedOption: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? 2 : 1
                    onSelectedOptionChanged: {
                        settingsController.theme = theme.selectedOption.value;
                    }
                
                } 
    	        
    	        Divider { }
    	        
                Button {
                    id: clearHistory
                    text: qsTr("Clear history");
                    horizontalAlignment: HorizontalAlignment.Fill
                    onClicked: {
                        loginController.deleteHistory();
                    }
                }
    	    }
    	    
    	    
    	    attachedObjects: [
                LoginController {
                    id: loginController
                },
                SettingsController {
                    id: settingsController
                },
                ComponentDefinition {
                    id: loginPage
                    source: "LoginForm.qml"
                
                }
            ]
    	    
            onUserNameChanged: {
                console.debug("name changed");
                userLabel.setText(qsTr("User: ") + settingsController.userName);
            }
    	}
    } 
	
	onCreationCompleted: {
        settingPage.userName =  settingsController.userName;
    }
	
	onPopTransitionEnded: {
	    
        userLabel.setText(qsTr("User: ") + loginController.savedlogin);
        loginButton.setVisible(!loginController.isLogged());
        logOutButton.setVisible(loginController.isLogged());
    }
	
}