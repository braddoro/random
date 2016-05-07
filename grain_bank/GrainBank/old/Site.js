//**************************************************************************
//
// File        :  Site.js
//
// Copyright   :  Copyright 2007 American Tire Distributors, Inc.
//
// Author(s)   :  Rob Helgeson - rhelgeson@atd-us.com
//
//                American Tire Distributors
//                12200 Herbert Wayne Ct.
//                Huntersville, NC 28078
//
//**************************************************************************
isc.defineClass("Site", "CEWindow");

isc.applicationWindow.registeredObjects.add({
    className: "Site",
    isProtected: true,
    singleton: true,
    module: "Site",
    resource: "Window",
    operation_type: "Show"
});

isc.Site.addProperties({
    title: "Location Maintenance",
    name: "Site",
    canDragReposition: true,
    canDragResize: true,
    autoCenter: true,
    width: "98%",
    height: "95%",
    showShadow: true,
    showMinimizeButton: false,
    initWidget: function(){

        this.Super("initWidget", arguments);

        this.dataHandler = isc.Class.create({
            parent: this,
            name: "dataHandler",
            values: {
                "CLASS_NAME": this.Class,
                "COUNTRY": "",
                "CUST_SOURCE": "",
                "SITE_ID": 0,
                "SITE_SOURCE": "",
                "SITE_TYPE": "",
                "STATE_PROV": ""
            },
            valueChangedCallback: function(item, newValue) {
                switch(item){
                case "setTitle":
                    this.parent.setTitle(lang_trans("location", isc.locale_EAS_shared_strings.location) + " - " + newValue);
                    break;
                case "applySourceStyle":
                    this.parent.siteHeaderDForm.applySourceStyle();
                    break;
                case "initTabVisibility":
                    this.parent.shipToTabSet.initTabVisibility();
                    break;
                case "setValues":
                    var newData = isc.addProperties({}, this.parent.siteHeaderDForm.getValues(), newValue);
                    this.parent.siteHeaderDForm.setValues(newData);
                    break;
                case "selectTab":
                    this.parent.shipToTabSet.selectTab(newValue);
                    break;
                case "fetch":
                    this.parent.shipToTabSet.fetchData(newValue);
                    break;
                case "loadSite":
                    this.parent.loadSite("LIVE", newValue);
                    this.parent.refreshCustomerSiteList();
                    break;
                case "whenLoaded":
                    if( this.parent.shipToTabSet &&
                        this.parent.shipToTabSet.tabs.find({id: newValue})
                        ){
                        this.parent.shipToTabSet.tabs.find({id: newValue}).whenLoaded();
                    }
                    break;
                case "get_CUSTOMER_NUMBER":
                    var account_number;
                    var customerWin = isc.applicationWindow.canvas.children.find({Class: "Customer"});
                    if(customerWin){
                        account_number = customerWin.dataHandler.valueChangedCallback("get_CUSTOMER_NUMBER");
                    }
                    return account_number;
                case "setCountry":
                    this.parent.dataHandler.values.COUNTRY = newValue;
                    break;
                case "getCountry":
                    return this.parent.shipToLocationInfoHLayout.dataHandler.valueChangedCallback("COUNTRY");
                case "refreshAddress":
                    this.parent.refreshCustomerAddress();
                    return;
                case "stateFIPS":
                    return this.parent.shipToLocationInfoHLayout.dataHandler.valueChangedCallback("stateFIPS");
                case "stateAbbrv":
                    return this.parent.shipToLocationInfoHLayout.dataHandler.valueChangedCallback("stateAbbrv");
                case "valuesHaveChanged":
                    return this.parent.shipToTabSet.valuesHaveChanged();
                case "reviewField":
                    return this.parent.shipToTabSet.tabs.find({reviewField: newValue});
                case "getPane":
                    return this.parent.shipToTabSet.tabs.find({reviewField: newValue});
                case "addressValidated":
                    return this.parent.shipToLocationInfoHLayout.dataHandler.valueChangedCallback("addressValidated");
                case "get_L_OBJECT_VERSION_NUMBER":
                    return this.parent.siteHeaderDForm.getValue("L_OBJECT_VERSION_NUMBER");
                case "get_CAS_OBJECT_VERSION_NUMBER":
                    return this.parent.siteHeaderDForm.getValue("CAS_OBJECT_VERSION_NUMBER");
                case "get_CSU_OBJECT_VERSION_NUMBER":
                    return this.parent.siteHeaderDForm.getValue("CSU_OBJECT_VERSION_NUMBER");
                case "get_LOCATION_NUMBER":
                    return this.parent.siteHeaderDForm.getValue("LOCATION");
                case "approvalLevel":
                    return this.parent.shipToApprovalVLayout.dataHandler.valueChangedCallback("approvalLevel", newValue);
                case "validateSiteInfo":
                    return this.parent.shipToLocationInfoHLayout.dataHandler.valueChangedCallback("isValidated");
                case "heaflist":
                    return this.parent.heaflistDForm;
                default:
                    // Required comment.
                    break;
                }
            }
        });

        /*------------------------------------------------------------*\
        |                 Start of Site Header Section                 |
        \*------------------------------------------------------------*/
        this.siteHeaderDForm = isc.DynamicForm.create({
            parent: this,
            name: "siteHeaderDForm",
            margin: 4,
            numCols: 4,
            colWidths: ["1%", "*", "1%", "*"],
            itemHoverWidth: 300,
            wrapItemTitles: false,
            fields: [
                {name: "DBA_NAME",
                    title: lang_trans("string_location_name", isc.locale_EAS_shared_strings.string_location_name),
                    width: 200,
                    defaultValue: "Location Name",
                    prompt: lang_trans("string_location_name", isc.locale_EAS_shared_strings.string_location_name),
                    editorType: "StaticTextItem",
                    titleStyle: "bold12px",
                    disabled: true,
                    showDisabled: false
                },
                {name: "CUSTOMER_SOURCE",
                    title: lang_trans("siteHeaderDForm.CUSTOMER_SOURCE.title", "Customer Source"),
                    editorType: "StaticTextItem",
                    titleStyle: "bold12px",
                    prompt: lang_trans("siteHeaderDForm.CUSTOMER_SOURCE.title", "Customer Source"),
                    disabled: true,
                    showDisabled: false,
                    optionDataSource: isc.EASDataSources.CMR.sourceLOV,
                    displayField: "SOURCE_VAL",
                    valueField: "SOURCE_VAL"
                },
                {name: "STORE_NUMBER",
                    title: lang_trans("store_number", isc.locale_EAS_shared_strings.store_number),
                    width: 75,
                    defaultValue: "Store Number",
                    prompt: lang_trans("store_number", isc.locale_EAS_shared_strings.store_number),
                    editorType: "StaticTextItem",
                    titleStyle: "bold12px",
                    disabled: true,
                    showDisabled: false
                },
                {name: "SITE_SOURCE",
                    title: lang_trans("siteHeaderDForm.SITE_SOURCE.title", "Location Source"),
                    width: 75,
                    defaultValue: "Location Source",
                    prompt: lang_trans("siteHeaderDForm.SITE_SOURCE.title", "Location Source"),
                    editorType: "StaticTextItem",
                    titleStyle: "bold12px",
                    disabled: true,
                    showDisabled: false,
                    optionDataSource: isc.EASDataSources.CMR.sourceLOV,
                    displayField: "SOURCE_VAL",
                    valueField: "SOURCE_VAL"
                },
                {name: "LOCATION",
                    title: lang_trans("location_number", isc.locale_EAS_shared_strings.location_number),
                    width: 75,
                    defaultValue: "Not Assigned",
                    prompt: lang_trans("location_number", isc.locale_EAS_shared_strings.location_number),
                    editorType: "StaticTextItem",
                    titleStyle: "bold12px",
                    disabled: true,
                    showDisabled: false
                },
                {name: "SITE_TYPE",
                    title: lang_trans("location_type", isc.locale_EAS_shared_strings.location_type),
                    editorType: "StaticTextItem",
                    titleStyle: "bold12px",
                    disabled: true,
                    showDisabled: false,
                    optionDataSource: isc.EASDataSources.CMR.locationTypeLOV,
                    valueField: "SOURCE_VAL",
                    displayField: "SOURCE_LBL"
                },
                {name: "CUSTOMER_NAME",
                    showTitle: false,
                    editorType: "TextItem",
                    visible: false
                },
                {name: "CUSTOMER_CLASS_CODE",
                    showTitle: false,
                    editorType: "TextItem",
                    visible: false
                },
                {name: "CSU_OBJECT_VERSION_NUMBER",
                    editorType: "TextItem",
                    visible: false
                },
                {name: "CAS_OBJECT_VERSION_NUMBER",
                    editorType: "TextItem",
                    visible: false
                },
                {name: "PS_OBJECT_VERSION_NUMBER",
                    editorType: "TextItem",
                    visible: false
                },
                {name: "L_OBJECT_VERSION_NUMBER",
                    editorType: "TextItem",
                    visible: false
                },
                {name: "SITE_ID",
                    defaultValue: "SITE_ID",
                    editorType: "TextItem",
                    visible: false
                },
                {name: "CUST_ACCOUNT_ID",
                    defaultValue: "CUST_ACCOUNT_ID",
                    editorType: "TextItem",
                    visible: false
                }
            ],
            itemHoverHTML: function(item){
                return getHelpText(item);
            },
            applySourceStyle: function(){
                var siteSource = this.getField("SITE_SOURCE");
                var custSource = this.getField("CUSTOMER_SOURCE");

                if(custSource.getValue() == "LIVE"){
                    custSource.textBoxStyle = "sourceLive";
                }else{
                    custSource.textBoxStyle = "sourceStaged";
                }
                if(siteSource.getValue() == "LIVE"){
                    siteSource.textBoxStyle = "sourceLive";
                    this.getField("DBA_NAME").textBoxStyle = "sourceLive";
                    this.getField("STORE_NUMBER").textBoxStyle = "sourceLive";
                    this.getField("LOCATION").textBoxStyle = "sourceLive";
                    this.getField("SITE_TYPE").textBoxStyle = "sourceLive";
                }else{
                    siteSource.textBoxStyle = "sourceStaged";
                    this.getField("DBA_NAME").textBoxStyle = "sourceStaged";
                    this.getField("STORE_NUMBER").textBoxStyle = "sourceStaged";
                    this.getField("LOCATION").textBoxStyle = "sourceStaged";
                    this.getField("SITE_TYPE").textBoxStyle = "sourceStaged";
                }
                this.markForRedraw();
            }
        });

        this.saveAllTabsButton = isc.IButton.create({
            parent: this,
            title: lang_trans("saveAllTabsButton.title", "Save All"),
            autoFit: true,
            showHover: true,
            hoverWidth: 300,
            prompt: lang_trans("saveAllTabsButton.prompt", "Save Tab"),

            getHoverHTML: function(){
                return getHelpText(isc.appData.applicationName, "Multiple", "saveCancelToolStrip", "saveAllTabsButton");
            },

            click: function(){
                this.parent.shipToTabSet.saveFormIfDirty(true);
            }
        });

        this.cancelTabButton = isc.IButton.create({
            parent: this,
            title: lang_trans("cancelTabButton.title", "Cancel Tab"),
            autoFit: true,
            showHover: true,
            hoverWidth: 300,
            prompt: lang_trans("cancelTabButton.title", "Cancel Tab"),

            getHoverHTML: function(){
                return getHelpText(isc.appData.applicationName, "Multiple", "saveCancelToolStrip", "cancelTabButton");
            },

            click: function(){
                var tabSet = this.parent.shipToTabSet;
                var message = lang_trans("have_unsaved_changes", isc.locale_EAS_shared_strings.have_unsaved_changes);
                if(tabSet.getSelectedTab().pane.valuesHaveChanged && tabSet.getSelectedTab().pane.valuesHaveChanged()){
                    isc.ask(
                        message,
                        function(isOK){
                            if(isOK){
                                tabSet.fetchData(false);
                            }
                        }
                    );
                }else{
                    tabSet.fetchData(false);
                }
            }
        });

        this.cancelAllTabsButton = isc.IButton.create({
            parent: this,
            title: lang_trans("cancelAllTabsButton.title", "Refresh All"),
            autoFit: true,
            showHover: true,
            hoverWidth: 300,
            prompt: lang_trans("cancelAllTabsButton.title", "Refresh All"),
            getHoverHTML: function(){
                return getHelpText(isc.appData.applicationName, "Multiple", "saveCancelToolStrip", "cancelAllTabsButton");
            },
            click: function(){
                var tabSet = this.parent.shipToTabSet;
                var message = lang_trans("have_unsaved_changes", isc.locale_EAS_shared_strings.have_unsaved_changes);
                if(tabSet.valuesHaveChanged()){
                    isc.ask(
                        message,
                        function(isOK){
                            if(isOK){
                                tabSet.fetchData(true);
                            }
                        }
                    );
                }else{
                    tabSet.fetchData(false);
                }
            }
        });

        this.saveCancelToolStrip = isc.ToolStrip.create({
            padding: 4,
            align: "right",
            styleName: "whiteToolStrip",
            members: [
                this.saveAllTabsButton,
                "separator",
                this.cancelTabButton,
                "separator",
                this.cancelAllTabsButton
            ]
        });

        this.siteHeaderHLayout = isc.HLayout.create({
            height: "1%",
            margin: 4,
            members: [
                this.siteHeaderDForm,
                this.saveCancelToolStrip
            ]
        });

        /*------------------------------------------------------------*\
        |                  Start of Tab Section                        |
        \*------------------------------------------------------------*/

        // Location Tab.
        //
        this.shipToLocationInfoHLayout = isc.SiteLocationLayout.create({parentHandler: this.dataHandler});

        // Contacts Tab.
        //
        this.shipToContactsHLayout = isc.HLayout.create({
            parent: this,
            margin: 4,
            members: [isc.SiteContactLayout],
            changed: function(){/*fillter*/},
            fetchData: function(){
                isc.SiteContactLayout.parentObject = isc.Site;
                var myTabSet = this.parent.shipToTabSet;
                isc.SiteContactLayout.shipToTabSet = myTabSet;
                isc.SiteContactLayout.callingTab = "shipToContactsTab";
                isc.SiteContactLayout.shipToContactNamesListGrid.fetchData();
            },
            setPermissions: function(){
                setListGridAccess(this.parent.name, isc.SiteContactLayout.shipToContactNamesListGrid, isc.Site.getInstanceProperty("gSiteSource") + ".Update", isc.SiteContactLayout.shipToContactNamesToolStrip);
                setListGridAccess(this.parent.name, isc.SiteContactLayout.shipToPhoneListGrid, isc.Site.getInstanceProperty("gSiteSource") + ".Update", isc.SiteContactLayout.shipToPhoneToolStrip);
                setListGridAccess(this.parent.name, isc.SiteContactLayout.shipToEmailListGrid, isc.Site.getInstanceProperty("gSiteSource") + ".Update", isc.SiteContactLayout.shipToEmailToolStrip);
                setListGridAccess(this.parent.name, isc.SiteContactLayout.shipToWebsiteListGrid, isc.Site.getInstanceProperty("gSiteSource") + ".Update", isc.SiteContactLayout.shipToWebsiteToolStrip);
            },
            invalidateCache: function(){
                isc.SiteContactLayout.shipToWebsiteVLayout.invalidateCache();
            },
            initTabVisibility: function(){
                isc.SiteContactLayout.shipToContactPointTabSet.initTabVisibility();
            },

            resetFlags: function(){
                isc.SiteContactLayout.shipToContactNamesListGrid.discardAllEdits();
                isc.SiteContactLayout.shipToPhoneListGrid.discardAllEdits();
                isc.SiteContactLayout.shipToEmailListGrid.discardAllEdits();
                isc.SiteContactLayout.shipToWebsiteListGrid.discardAllEdits();
            }
        });

        // National Account Tab.
        //
        this.shipToDCTeamHLayout = isc.HLayout.create({
            parent: this,
            members: [isc.VLayout.create({members: [isc.SiteNationalAccountLayout]})],
            fetchData: function(){
                isc.SiteNationalAccountLayout.parentObject = isc.Site;
                var myTabSet = this.parent.shipToTabSet;
                isc.SiteNationalAccountLayout.shipToTabSet = myTabSet;
                isc.SiteNationalAccountLayout.callingTab = "shipToDCTeamTab";
                isc.SiteNationalAccountLayout.shipToDCTeamListGrid.fetchData();
            },
            setPermissions: function(){
                setListGridAccess(this.parent.name, isc.SiteNationalAccountLayout.shipToDCTeamListGrid, isc.Site.getInstanceProperty("gSiteSource") + ".Update", isc.SiteNationalAccountLayout.shipToDCTeamToolStrip);
            },
            changed: function(){/*fillter*/},
            invalidateCache: function(){
                if(isc.SiteNationalAccountLayout.shipToDCTeamListGrid.data && isc.SiteNationalAccountLayout.shipToDCTeamListGrid.data.invalidateCache){
                    isc.SiteNationalAccountLayout.shipToDCTeamListGrid.data.invalidateCache();
                }
            },
            resetFlags: function(){
                isc.SiteNationalAccountLayout.shipToDCTeamListGrid.discardAllEdits();
            }
        });

        // Tax Tab.
        //
        this.shipToARTeamVLayout = isc.SiteTaxLayout.create({parentHandler: this.dataHandler});

        // Marketing Tab
        //
        this.shipToMATeamHLayout = isc.VLayout.create({
            parent: this,
            margin: 4,
            members: [isc.SiteMarketingLayout],
            fetchData: function(){
                isc.SiteMarketingLayout.parentObject = isc.Site;
                var myTabSet = this.parent.shipToTabSet;
                isc.SiteMarketingLayout.shipToTabSet = myTabSet;
                isc.SiteMarketingLayout.callingTab = "shipToMATeamTab";
                isc.SiteMarketingLayout.shipToMATeamDForm.fetchData();
                isc.SiteMarketingLayout.shipToMATeamBrandsListGrid.fetchData();
                isc.SiteMarketingLayout.shipToMarketingProgramsListGrid.fetchData();
                isc.SiteMarketingLayout.shipToMarketingProgramsHistoryListGrid.fetchData();
            },
            changed: function(){
                isc.SiteMarketingLayout.shipToMATeamDForm.changed();
            },
            saveFormIfDirty: function(){
                saveFormIfDirty(isc.SiteMarketingLayout.shipToMATeamDForm);
            },
            setPermissions: function(){
                setFormFieldAccess(
                    this.parent.name,
                    isc.SiteMarketingLayout.shipToMATeamDForm,
                    isc.Site.getInstanceProperty("gSiteSource") + ".Update");
                setListGridAccess(
                    this.parent.name,
                    isc.SiteMarketingLayout.shipToMarketingProgramsListGrid,
                    isc.Site.getInstanceProperty("gSiteSource") + ".Update",
                    isc.SiteMarketingLayout.shipToMarketingProgramsToolStrip);
                setListGridAccess(
                    this.parent.name,
                    isc.SiteMarketingLayout.shipToMATeamBrandsListGrid,
                    isc.Site.getInstanceProperty("gSiteSource") + ".Update",
                    isc.SiteMarketingLayout.shipToMATeamBrandsToolStrip);
            },
            invalidateCache: function(){
                if(isc.SiteMarketingLayout.shipToMATeamDForm.data && isc.SiteMarketingLayout.shipToMATeamDForm.data.invalidateCache){
                    isc.SiteMarketingLayout.shipToMATeamDForm.data.invalidateCache();
                }
                if(isc.SiteMarketingLayout.shipToMATeamBrandsListGrid.data && isc.SiteMarketingLayout.shipToMATeamBrandsListGrid.data.invalidateCache){
                    isc.SiteMarketingLayout.shipToMATeamBrandsListGrid.data.invalidateCache();
                }
                if(isc.SiteMarketingLayout.shipToMarketingProgramsListGrid.data && isc.SiteMarketingLayout.shipToMarketingProgramsListGrid.data.invalidateCache){
                    isc.SiteMarketingLayout.shipToMarketingProgramsListGrid.data.invalidateCache();
                }
                if(isc.SiteMarketingLayout.shipToMarketingProgramsHistoryListGrid.data && isc.SiteMarketingLayout.shipToMarketingProgramsHistoryListGrid.data.invalidateCache){
                    isc.SiteMarketingLayout.shipToMarketingProgramsHistoryListGrid.data.invalidateCache();
                }
            }
        });

        // TirePros Tab
        //
        this.shipToTireProsHLayout = isc.VLayout.create({
            parent: this,
            margin: 4,
            members: [isc.SiteTireProsLayout],
            fetchData: function(){
                isc.SiteTireProsLayout.parentObject = isc.Site;
                isc.SiteTireProsLayout.parentClass = this.parent;
                isc.SiteTireProsLayout.callingTab = "SiteTireProsLayout";
                isc.SiteTireProsLayout.shipToTireProsMarketingProgramsListGrid.fetchData();
                isc.SiteTireProsLayout.shipToTireProsMarketingProgramsHistoryListGrid.fetchData();
                isc.SiteTireProsLayout.tireProsDealerDForm.fetchData();
                isc.SiteTireProsLayout.tireProsDealerListGrid.fetchData();
            },
            changed: function(){/*filler*/},
            saveFormIfDirty: function(){
                saveFormIfDirty(isc.SiteTireProsLayout.tireProsDealerDForm);
            },
            setPermissions_TirePros: function(status){
                setListGridAccess(
                    this.parent.name,
                    isc.SiteTireProsLayout.shipToTireProsMarketingProgramsListGrid,
                    isc.Site.getInstanceProperty("gSiteSource") + ".Update",
                    isc.SiteTireProsLayout.shipToTireProsMarketingProgramsToolStrip);

                // If the Customer screen | Administration Tab | Tire Pros field is
                // set to 'Y' then let permissions run as they will.  If that value
                // is not 'Y' then disable these objects.
                //
                if(status == 'Y'){
                    setListGridAccess(
                        this.parent.name,
                        isc.SiteTireProsLayout.tireProsDealerListGrid,
                        isc.Site.getInstanceProperty("gSiteSource") + ".Update",
                        isc.SiteTireProsLayout.dealerToolStrip);
                    setDformAccess(
                        this.parent.name,
                        isc.SiteTireProsLayout.tireProsDealerDForm,
                        isc.Site.getInstanceProperty("gSiteSource") + ".Update");
                }else{

                    // This permission should never be found: Update_DISABLE, thus
                    // causing the objects to be disabled normally.
                    //
                    setListGridAccess(
                        this.parent.name,
                        isc.SiteTireProsLayout.tireProsDealerListGrid,
                        isc.Site.getInstanceProperty("gSiteSource") + ".Update_DISABLE",
                        isc.SiteTireProsLayout.dealerToolStrip);
                    setDformAccess(
                        this.parent.name,
                        isc.SiteTireProsLayout.tireProsDealerDForm,
                        isc.Site.getInstanceProperty("gSiteSource") + ".Update_DISABLE");
                }
            },
            invalidateCache: function(){
                if(isc.SiteTireProsLayout.shipToTireProsMarketingProgramsListGrid.data && isc.SiteTireProsLayout.shipToTireProsMarketingProgramsListGrid.data.invalidateCache){
                    isc.SiteTireProsLayout.shipToTireProsMarketingProgramsListGrid.data.invalidateCache();
                }
                if(isc.SiteTireProsLayout.shipToTireProsMarketingProgramsHistoryListGrid.data && isc.SiteTireProsLayout.shipToTireProsMarketingProgramsHistoryListGrid.data.invalidateCache){
                    isc.SiteTireProsLayout.shipToTireProsMarketingProgramsHistoryListGrid.data.invalidateCache();
                }
            }
        });

        // Notes Tab.
        //
        this.shipToNotesLayout = isc.VLayout.create({
            parent: this,
            margin: 4,
            members: [isc.SiteNoteLayout],
            fetchData: function(){
                isc.SiteNoteLayout.parentObject = isc.Site;
                isc.SiteNoteLayout.callingTab = "shipToNotesTab";
                isc.SiteNoteLayout.shipToNotesListGrid.fetchData();
            },
            setPermissions: function(){
                setListGridAccess(
                    this.parent.name,
                    isc.SiteNoteLayout.shipToNotesListGrid,
                    isc.Site.getInstanceProperty("gSiteSource") + ".Update",
                    isc.SiteNoteLayout.shipToNotesToolStrip);
            },
            changed: function(){/*fillter*/},
            invalidateCache: function(){
                if(isc.SiteNoteLayout.shipToNotesListGrid.data &&
                    isc.SiteNoteLayout.shipToNotesListGrid.data.invalidateCache){
                    isc.SiteNoteLayout.shipToNotesListGrid.data.invalidateCache();
                }
            }
        });

        // Online Tab.
        //
        this.shipToOnlineLayout = isc.VLayout.create({
            parent: this,
            margin: 4,
            members: [isc.SiteOnlineLayout],
            fetchData: function(){
                isc.SiteOnlineLayout.parentClass = this.parent;
                isc.SiteOnlineLayout.parentObject = isc.Site;
                isc.SiteOnlineLayout.shipToTabSet = this.parent.shipToTabSet;
                isc.SiteOnlineLayout.callingTab = "shipToOnlineTab";
                isc.SiteOnlineLayout.shipToOnlineProductsListGrid.fetchData();
                isc.SiteOnlineLayout.shipToOnlineAccountsListGrid.setData([]);
            },
            setPermissions: function(){
                setListGridAccess(
                    this.parent.name,
                    isc.SiteOnlineLayout.shipToOnlineProductsListGrid,
                    isc.Site.getInstanceProperty("gSiteSource") + ".Update",
                    isc.SiteOnlineLayout.shipToOnlineProductsToolStrip);
                setListGridAccess(
                    this.parent.name,
                    isc.SiteOnlineLayout.shipToOnlineAccountsListGrid,
                    isc.Site.getInstanceProperty("gSiteSource") + ".Update",
                    isc.SiteOnlineLayout.shipToOnlineAccountsToolStrip);
            },
            changed: function(){/*filler*/},
            invalidateCache: function(){
                if(isc.SiteOnlineLayout.shipToOnlineProductsListGrid.data && isc.SiteOnlineLayout.shipToOnlineProductsListGrid.data.invalidateCache){
                    isc.SiteOnlineLayout.shipToOnlineProductsListGrid.data.invalidateCache();
                }
                if(isc.SiteOnlineLayout.shipToOnlineAccountsListGrid.data && isc.SiteOnlineLayout.shipToOnlineAccountsListGrid.data.invalidateCache){
                    isc.SiteOnlineLayout.shipToOnlineAccountsListGrid.data.invalidateCache();
                }
            }
        });

        // Hybris Tab.
        //
        this.shipToHybrisLayout = isc.SiteHybrisLayout.create({parentHandler: this.dataHandler});

        // CreditCard Tab.
        //
        this.shipToCreditCardVLayout = isc.VLayout.create({
            parent: this,
            margin: 4,
            members: [isc.SiteCreditCardLayout],
            fetchData: function(){
                isc.SiteCreditCardLayout.parentObject = isc.Site;
                isc.SiteCreditCardLayout.callingTab = "shipToCreditCardTab";
                isc.SiteCreditCardLayout.shipToCreditCardListGrid.fetchData();
            },
            changed: function(){
                this.parent.shipToCreditCardListGrid.changed();
            },
            invalidateCache: function(){
                if(isc.SiteCreditCardLayout.shipToCreditCardListGrid.data && isc.SiteCreditCardLayout.shipToCreditCardListGrid.data.invalidateCache){
                    isc.SiteCreditCardLayout.shipToCreditCardListGrid.data.invalidateCache();
                }
            },
            resetFlags: function(){
                isc.SiteCreditCardLayout.shipToCreditCardListGrid.discardAllEdits();
            },
            setPermissions: function(){
                setListGridAccess(
                    this.parent.name,
                    isc.SiteCreditCardLayout.shipToCreditCardListGrid,
                    isc.Site.getInstanceProperty("gSiteSource") + ".Update",
                    isc.SiteCreditCardLayout.shipToCreditCardToolStrip);
            }
        });

        // History Tab.
        //
        this.shipToReviewVLayout = isc.VLayout.create({
            parent: this,
            margin: 4,
            members: [isc.SiteHistoryLayout],
            fetchData: function(){
                isc.SiteHistoryLayout.parentObject = isc.Site;
                isc.SiteHistoryLayout.callingTab = "shipToReviewTab";
                isc.SiteHistoryLayout.shipToHistoryListGrid.fetchData();
            },
            changed: function(){/*filler*/},
            invalidateCache: function(){
                if(isc.SiteHistoryLayout.shipToHistoryListGrid.data && isc.SiteHistoryLayout.shipToHistoryListGrid.data.invalidateCache){
                    isc.SiteHistoryLayout.shipToHistoryListGrid.data.invalidateCache();
                }
            }
        });

        // Approvals Tab.
        //
        this.shipToApprovalVLayout = isc.SiteApprovalLayout.create({parentHandler: this.dataHandler});

        /*------------------------------------------------------------*\
        |                   Start of Layout Section                    |
        \*------------------------------------------------------------*/
        this.shipToTabSet = isc.TabSet.create({
            parent: this,
            tabs: [
                {name: "shipToCustomerInfoTab",
                    id: "shipToCustomerInfoTab",
                    title: lang_trans("location", isc.locale_EAS_shared_strings.location),
                    useNewStyleCode: true,
                    pane: this.shipToLocationInfoHLayout,
                    alwaysLoad: false,
                    formsLoaded: 0,
                    formCount: 2,
                    reviewField: "INFO_APPROVED",
                    isLoaded: false,
                    isBilltoOK: false,
                    tabSelected: function(tabSet, tabNum, tabPane, ID, tab){
                        this.parent.shipToLocationInfoHLayout.parentHandler.valueChangedCallback("fetch");
                    },
                    resetFlags: function(){
                        this.pane.resetFlags();
                    },
                    whenLoaded: function(){
                        this.isLoaded = true;
                    }
                },
                {name: "shipToContactsTab",
                    title: lang_trans("shipToTabSet.shipToContactsTab.title", "Contacts"),
                    useNewStyleCode: false,
                    pane: this.shipToContactsHLayout,
                    disableIndividualActions: true,
                    alwaysLoad: false,
                    formsLoaded: 0,
                    formCount: 1,
                    reviewField: "CONTACTS_APPROVED",
                    isLoaded: false,
                    isBilltoOK: true,
                    resetFlags: function(){
                        this.pane.resetFlags();
                    },
                    whenLoaded: function(){
                        this.isLoaded = true;
                    }
                },
                {name: "shipToDCTeamTab",
                    title: lang_trans("shipToTabSet.shipToDCTeamTab.title", "National Accounts"),
                    useNewStyleCode: false,
                    pane: this.shipToDCTeamHLayout,
                    disableIndividualActions: true,
                    alwaysLoad: false,
                    formsLoaded: 0,
                    formCount: 1,
                    reviewField: "BRANCH_APPROVED",
                    isLoaded: false,
                    isBilltoOK: false,
                    resetFlags: function(){
                        this.pane.resetFlags();
                    },
                    whenLoaded: function(){
                        this.isLoaded = true;
                    }
                },
                {name: "shipToARTeamTab",
                    title: lang_trans("shipToTabSet.shipToARTeamTab.title", "Tax"),
                    useNewStyleCode: true,
                    pane: this.shipToARTeamVLayout,
                    alwaysLoad: false,
                    formsLoaded: 0,
                    formCount: 1,
                    reviewField: "AR_APPROVED",
                    isLoaded: false,
                    isBilltoOK: false,
                    tabSelected: function(tabSet, tabNum, tabPane, ID, tab){
                        this.parent.shipToARTeamVLayout.parentHandler.valueChangedCallback("fetch");
                    },
                    resetFlags: function(){
                        this.pane.resetFlags();
                    },
                    whenLoaded: function(){
                        this.isLoaded = true;
                    }
                },
                {name: "shipToMATeamTab",
                    title: lang_trans("shipToTabSet.shipToMATeamTab.title", "Marketing"),
                    useNewStyleCode: false,
                    pane: this.shipToMATeamHLayout,
                    alwaysLoad: false,
                    formsLoaded: 0,
                    formCount: 1,
                    reviewField: "MA_APPROVED",
                    isLoaded: false,
                    isBilltoOK: false,
                    whenLoaded: function(){
                        this.isLoaded = true;
                    }
                },
                {name: "shipToTireProsTab",
                    title: lang_trans("shipToTabSet.shipToTireProsTab.title", "TirePros"),
                    useNewStyleCode: false,
                    pane: this.shipToTireProsHLayout,
                    alwaysLoad: false,
                    formsLoaded: 0,
                    formCount: 1,
                    reviewField: "MA_APPROVED",
                    isLoaded: false,
                    isBilltoOK: false,
                    whenLoaded: function(){
                        this.isLoaded = true;
                    }
                },
                {name: "shipToNotesTab",
                    title: lang_trans("string_notes", isc.locale_EAS_shared_strings.string_notes),
                    useNewStyleCode: false,
                    disableIndividualActions: true,
                    pane: this.shipToNotesLayout,
                    alwaysLoad: true,
                    formsLoaded: 0,
                    formCount: 1,
                    isLoaded: false,
                    isBilltoOK: false
                },
                {name: "shipToOnlineTab",
                    title: lang_trans("shipToTabSet.shipToOnlineTab.title", "Online"),
                    useNewStyleCode: false,
                    disableIndividualActions: true,
                    pane: this.shipToOnlineLayout,
                    alwaysLoad: true,
                    formsLoaded: 0,
                    formCount: 1,
                    isLoaded: false,
                    isBilltoOK: false
                },
                {name: "shipToHybrisTab",
                    title: lang_trans("title_hybris", isc.locale_EAS_shared_strings.title_hybris),
                    useNewStyleCode: true,
                    disableIndividualActions: true,
                    pane: this.shipToHybrisLayout,
                    alwaysLoad: true,
                    formsLoaded: 0,
                    formCount: 1,
                    isLoaded: false,
                    isBilltoOK: false,
                    tabSelected: function(tabSet, tabNum, tabPane, ID, tab){
                        this.parent.shipToHybrisLayout.parentHandler.valueChangedCallback("fetch");
                    }
                },
                {name: "shipToCreditCardTab",
                    title: lang_trans("shipToTabSet.shipToCreditCardTab.title", "Credit Cards"),
                    useNewStyleCode: false,
                    disableIndividualActions: true,
                    pane: this.shipToCreditCardVLayout,
                    alwaysLoad: true,
                    formsLoaded: 0,
                    formCount: 1,
                    isLoaded: true,
                    isBilltoOK: true,
                    resetFlags: function(){
                        this.pane.resetFlags();
                    },
                    whenLoaded: function(){
                        this.isLoaded = true;
                    }
                },
                {name: "shipToReviewTab",
                    title: lang_trans("string_history", isc.locale_EAS_shared_strings.string_history),
                    useNewStyleCode: false,
                    disableIndividualActions: true,
                    pane: this.shipToReviewVLayout,
                    alwaysLoad: true,
                    formsLoaded: 0,
                    formCount: 1,
                    isLoaded: false,
                    isBilltoOK: true
                },
                {name: "shipToApprovalTab",
                    title: lang_trans("shipToTabSet.shipToApprovalTab.title", "Approvals"),
                    useNewStyleCode: true,
                    disableIndividualActions: true,
                    pane: this.shipToApprovalVLayout,
                    alwaysLoad: true,
                    formsLoaded: 0,
                    formCount: 1,
                    isLoaded: false,
                    isBilltoOK: true,
                    resetFlags: function(){
                        this.pane.resetFlags();
                    }
                }
            ],
            initTabVisibility: function(){
                this.setPermissions(true);
                if(this.parent.siteHeaderDForm.getValue("SITE_TYPE") == "B"){
                    for(i = 0; i < this.tabs.length; i++){
                        if(!this.tabs[i].isBilltoOK){
                            this.disableTab(this.tabs[i]);
                        }
                    }
                }
                for(i = 0; i < this.tabs.length; i++){
                    if(!this.getTab(i).disabled){
                        this.selectTab(i);
                        break;
                    }
                }
            },
            tabSelected: function(tabNum, tabPane, ID, tab){
                if(tab.alwaysLoad || tab.formsLoaded < tab.formCount){
                    this.fetchData(false);
                }
                if(tab.disableIndividualActions){
                    this.parent.cancelTabButton.setDisabled(true);
                }else{
                    this.parent.cancelTabButton.setDisabled(false);
                }
            },
            manipulateTab: function(curTab, actionToTake){
                if(curTab.pane && this.parent.dataHandler.values.SITE_ID > 0){
                    switch(actionToTake){
                        case "setPermissions":
                            if(checkForPermission(this.parent.name, curTab.name, this.parent.dataHandler.values.SITE_SOURCE + ".Show")){
                                this.enableTab(curTab);
                            }else{
                                this.disableTab(curTab);
                            }
                            if(curTab.useNewStyleCode){
                                curTab.pane.dataHandler.valueChangedCallback("setPermissions");
                            }else{
                                if(curTab.pane.setPermissions){
                                    curTab.pane.setPermissions();
                                }
                            }
                            break;
                        case "resetFlags":
                            curTab.formsLoaded = 0;
                            curTab.isLoaded = false;
                            if(curTab.useNewStyleCode){
                                curTab.pane.dataHandler.valueChangedCallback("resetFlags");
                            }else{
                                if(curTab.resetFlags){
                                    curTab.resetFlags();
                                }
                            }
                            break;
                        case "fetch":
                            if(curTab.useNewStyleCode){
                                if(!curTab.pane.dataHandler.valueChangedCallback("valuesHaveChanged") || curTab.alwaysLoad){
                                    curTab.pane.dataHandler.valueChangedCallback("fetch");
                                }
                            }else{
                                if(curTab.pane.invalidateCache){
                                    curTab.pane.invalidateCache();
                                }
                                if(curTab.pane.fetchData){
                                    curTab.formsLoaded = 0;
                                    curTab.pane.fetchData({},
                                        function(){
                                            curTab.pane.changed();
                                            if(curTab.pane.rememberValues){
                                                curTab.pane.rememberValues();
                                            }
                                            curTab.formsLoaded++;
                                            if(curTab.formsLoaded > curTab.formCount){
                                                curTab.formsLoaded = curTab.formCount;
                                            }
                                            if(curTab.formsLoaded == curTab.formCount && curTab.whenLoaded){
                                                curTab.whenLoaded();
                                            }
                                        }
                                    );
                                }
                            }
                            break;
                        case "save":
                            if(curTab.useNewStyleCode){
                                curTab.pane.dataHandler.valueChangedCallback("save");
                            }else{
                                if(curTab.pane.setValidation){
                                    if(this.parent.dataHandler.values.SITE_SOURCE == "LIVE"){
                                        curTab.pane.setValidation(true);
                                    }else{
                                        curTab.pane.setValidation(false);
                                    }
                                }
                                if(curTab.pane.saveFormIfDirty){
                                    curTab.pane.saveFormIfDirty();
                                }
                            }
                            break;
                    }
                }
            },
            manipulateData: function(actionToTake, allTabs){
                var i;
                actionToTake = (typeof actionToTake == "undefined") ? "fetch" : actionToTake;
                allTabs = (typeof allTabs == "undefined") ? false : allTabs;
                if(allTabs){
                    for(i = 0; i < this.tabs.length; i++){
                        this.manipulateTab(this.tabs[i], actionToTake);
                    }
                }else{
                    this.manipulateTab(this.getSelectedTab(), actionToTake);
                }
            },
            fetchData: function(allTabs){
                this.manipulateData("fetch", allTabs);
            },
            saveFormIfDirty: function(allTabs){
                this.manipulateData("save", allTabs);
            },
            resetFlags: function(){
                this.manipulateData("resetFlags", true);
            },
            setPermissions: function(allTabs){
                this.manipulateData("setPermissions", allTabs);
            },
            valuesHaveChanged: function(){
                var retVal = false;
                for(i = 0; i < this.tabs.length; i++){
                    if(this.tabs[i].useNewStyleCode) {
                        if (this.tabs[i].pane.dataHandler.valueChangedCallback("valuesHaveChanged")){
                            retVal = true;
                            break;
                        }
                    }else{
                        if(this.tabs[i].pane.valuesHaveChanged && this.tabs[i].pane.valuesHaveChanged()){
                            retVal = true;
                            break;
                        }
                    }
                }
                return retVal;
            }
        });

        this.mainLayout = isc.VLayout.create({
            members: [
                this.siteHeaderHLayout,
                this.shipToTabSet
            ]
        });

        this.addItem(this.mainLayout);

        this.heaflistDForm = isc.DynamicForm.create({
            target: "_blank",
            canSubmit: true,
            fields: [
                {name: "f_userid"},
                {name: "f_password"},
                {name: "f_userid2"}
            ]
        });

        // The form must be visible at the time that the widgets
        // are added to the html, otherwise the form will not submit
        // so we hide it here from actually being displayed.
        //
        this.addItem(this.heaflistDForm);
    },

    /*------------------------------------------------------------*\
    |                   Start of Window Functions                  |
    \*------------------------------------------------------------*/

    windowInitialize: function(initData){
        this.initData = initData;

        // Set the global values for use everywhere by everyone.
        //
        isc.Site.setInstanceProperty("gSiteID",     initData.SITE_ID);
        isc.Site.setInstanceProperty("gSiteSource", initData.dataSource);
        isc.Site.setInstanceProperty("gSiteType",   initData.SITE_TYPE);
        isc.Site.setInstanceProperty("gCustSource", initData.custStatus);
        this.dataHandler.values.SITE_ID     = initData.SITE_ID;
        this.dataHandler.values.SITE_SOURCE = initData.dataSource;
        this.dataHandler.values.SITE_TYPE   = initData.SITE_TYPE;
        this.dataHandler.values.CUST_SOURCE = initData.custStatus;
        this.dataHandler.values.COUNTRY     = initData.COUNTRY;
        this.dataHandler.values.STATE_PROV  = initData.STATE;

        // The form must be visible at the time that the widgets
        // are added to the html, otherwise the form will not submit
        // so we hide it here from actually being displayed.
        //
        this.heaflistDForm.hide();

        // initData.custStatus will contain the customer's status
        // this can be LIVE even for a staged site. It can never be STAGED for a LIVE site.
        // initData.dataSource will contain either "LIVE" or "STAGED"
        // initData.dataKey will either be the oracle site ID or CMR_SITES.SITE_ID.
        //
        var statusMessage = lang_trans("windowInitialize.status.message", "Site.windowInitialize() was not called correctly");
        var statusCode = lang_trans("error_code_warn", isc.locale_EAS_shared_strings.error_code_warn);

        this.shipToTabSet.resetFlags();
        if( initData &&
            initData.dataSource &&
            initData.SITE_ID &&
            initData.custStatus &&
            initData.custName){
            this.loadSite(initData.dataSource, initData.SITE_ID, initData.custStatus, initData.custName, initData.location, initData.COUNTRY, initData.STATE);
            if (initData.CUST_ACCOUNT_ID) {
	            RPCManager.sendEASRPCRequest({
	                params: {CUSTOMER_ID: initData.CUST_ACCOUNT_ID, ELEMENT_NAME: "TIRE_PROS"},
	                actionURL: isc.appData.controllerStub + "?om=ValueSets.getCustSiteUseAttrsValue",
	                callback: {target: this, methodName: "getAttributesCallBack"},
	                prompt: "",
	                showPrompt: false
	            });
            }
        }else{
            handleDialogBoxStatusCodes({statusCode: statusCode, statusMessage: statusMessage});
        }
    },

    getAttributesCallBack: function(rpcResponse){
        var dealer_in_process = 'N';
        if( typeof rpcResponse !== "undefined" &&
            typeof rpcResponse.data !== "undefined" &&
            typeof rpcResponse.data[0] !== "undefined" &&
            typeof rpcResponse.data[0].ELEMENT_VALUE !== "undefined"){

            dealer_in_process = rpcResponse.data[0].ELEMENT_VALUE;
        }
        this.shipToTireProsHLayout.setPermissions_TirePros(dealer_in_process);
    },

    loadSite: function(dataSource, dataKey, custStatus, custName, location, COUNTRY, STATE_PROV){
        isc.Site.setInstanceProperty('gSiteID', dataKey);
        isc.Site.setInstanceProperty('gSiteSource', dataSource);
        this.dataHandler.values.SITE_ID     = dataKey;
        this.dataHandler.values.SITE_SOURCE = dataSource;
        this.dataHandler.values.SITE_TYPE   = isc.Site.getInstanceProperty("gSiteType");
        this.dataHandler.values.COUNTRY     = COUNTRY;
        this.dataHandler.values.STATE_PROV  = STATE_PROV;
        this.siteHeaderDForm.applySourceStyle();

        this.shipToLocationInfoHLayout.dataHandler.valueChangedCallback("invalidateCache");
        this.shipToLocationInfoHLayout.dataHandler.valueChangedCallback("fetch");

        isc.SiteTaxLayout.parentObject = isc.Site;
        this.shipToTabSet.setPermissions(true);
        this.shipToTabSet.initTabVisibility();
    },

    refreshCustomerSiteList: function(){
        // find the customer window in the application window's cache
        var customerWin = isc.applicationWindow.canvas.children.find({Class: "Customer"});
        if(customerWin){
            customerWin.dataHandler.valueChangedCallback("refreshCustomerSiteList");
        }
    },

    refreshCustomerAddress: function(){
        // find the customer window in the application window's cache
        var customerWin = isc.applicationWindow.canvas.children.find({Class: "Customer"});
        if(customerWin){
            customerWin.dataHandler.valueChangedCallback("refreshCustomerAddress");
        }
    },

    refreshCustomerInfo: function(){
        // find the customer window in the application window's cache
        var customerWin = isc.applicationWindow.canvas.children.find({Class: "Customer"});
        if(customerWin){
            customerWin.dataHandler.valueChangedCallback("refreshCustomerInfo");
        }
    },

    closeClick: function(){
        var win = this;

        if(this.shipToTabSet.valuesHaveChanged()){
            isc.ask(
                lang_trans("have_unsaved_changes_abandon", isc.locale_EAS_shared_strings.have_unsaved_changes_abandon),
                function(isOK){
                    if(isOK){
                        win.shipToTabSet.saveFormIfDirty(true);
                    }else{
                        isc.applicationWindow.closeWindow(win.name);
                    }
                }
            );
        }else{
            isc.applicationWindow.closeWindow(win.name);
        }
    }
});
