//**************************************************************************
//
// File        :  SiteHybris.js
//
// Copyright   :  Copyright 2014 American Tire Distributors, Inc.
//
// Author(s)   :  Brad Hughes - bhughes@atd-us.com
//
//                American Tire Distributors
//                12200 Herbert Wayne Ct.
//                Huntersville, NC 28078
//
//**************************************************************************
isc.defineClass("SiteHybrisLayout", "Layout");

isc.SiteHybrisLayout.addProperties({
    title: "SiteHybris",
    name: "SiteHybris",
    initWidget: function(initData){
    	this.parentHandler = initData.parentHandler;

    	this.Super("initWidget", arguments);
        this.dataHandler = isc.Class.create({
            parent: this,
            name: "dataHandler",
            values: {
                "CLASS_NAME": this.Class
            },
            valueChangedCallback: function(item, newValue){
                switch(item){
                case "invalidateCache":
                    if(this.parent.shipToHybrisOnlineProductsListGrid.data && this.parent.shipToHybrisOnlineProductsListGrid.data.invalidateCache){
                    	this.parent.shipToHybrisOnlineProductsListGrid.data.invalidateCache();
                    }
                    if(this.parent.shipToHybrisOnlineAccountsListGrid.data && this.parent.shipToHybrisOnlineAccountsListGrid.data.invalidateCache){
                    	this.parent.shipToHybrisOnlineAccountsListGrid.data.invalidateCache();
                    }
                    break;
                case "setPermissions":
                	setListGridAccess(this.parent.parentHandler.values.CLASS_NAME, this.parent.shipToHybrisOnlineProductsListGrid, this.parent.parentHandler.values.SITE_SOURCE.toUpperCase() + ".Update", this.parent.shipToOnlineProductsToolStrip);
              		setListGridAccess(this.parent.parentHandler.values.CLASS_NAME, this.parent.shipToHybrisOnlineAccountsListGrid, this.parent.parentHandler.values.SITE_SOURCE.toUpperCase() + ".Update", this.parent.shipToOnlineAccountsToolStrip);
                    break;
                case "fetch":
                	this.parent.shipToHybrisOnlineProductsListGrid.fetchData();
                	this.parent.shipToHybrisOnlineAccountsListGrid.setData([]);
                    break;
                default:
                    // Required comment.
                    break;
                }
            }
        });

        this.shipToHybrisOnlineProductsListGrid = isc.ListGrid.create({
            parent: this,
            name: "shipToHybrisOnlineProductsListGrid",
            height: 135,
            margin: 4,
            modalEditing: true,
            alternateRecordStyles: true,
            dataSource: isc.EASDataSources.Site.siteOnlineProduct,
            autoFetchData: true,
            showFilterEditor: true,
            selectionType: "single",
            sortFieldNum: 0,
            sortDirection: "ascending",
            fields: [
                {title: lang_trans("string_product", isc.locale_EAS_shared_strings.string_product),
                    name: "PRODUCT_VARIANT_ID",
                    width: 200,
                    required: true,
                    editorType: "SelectItem",
                    optionDataSource: isc.EASDataSources.CMR.onlineProductVariants,
                    valueField: "ID",
                    displayField: "VARIANT_NAME"
                },
                {title: lang_trans("string_status", isc.locale_EAS_shared_strings.string_status),
                    name: "STATUS",
                    width: 90,
                    required: true,
                    editorType: "SelectItem",
                    optionDataSource: isc.EASDataSources.CMR.statusActiveDisabled,
                    valueField: "STATUS_VAL",
                    displayField: "STATUS_LBL"
                },
                {name: "CREATION_DATE",
                    title: lang_trans("field_activation_date", isc.locale_EAS_shared_strings.field_activation_date),
                    width: 140,
                    prompt: lang_trans("field_activation_date", isc.locale_EAS_shared_strings.field_activation_date),
                    editorType: "DateItem",
                    validators: [{type: "isDate"}]
                }
            ],
            fetchData: function(criteria, callback, requestProperties){
                var moreCriteria = {SITE_ID: this.parent.parentHandler.values.SITE_ID};
                var newCriteria = isc.addProperties({}, criteria, moreCriteria);
                this.Super("fetchData", [newCriteria, {target: this, methodName: "fetchComplete"}, requestProperties]);
            },
            fetchComplete: function(rpcResponse){
                this.setFieldProperties("PRODUCT_VARIANT_ID", {
                    optionCriteria: {SITE_ID: this.parent.parentHandler.values.SITE_ID, HYBRIS_PRODUCT_FLAG: 'Y'}
                });
            },
            editComplete: function(){
                this.invalidateCache();
                this.fetchData({SITE_ID: this.parent.parentHandler.values.SITE_ID});
            },
            selectionChanged: function(record, state){
                if(state){
                    refreshData(this.parent.shipToHybrisOnlineAccountsListGrid);
                }else{
                    clearData(this.parent.shipToHybrisOnlineAccountsListGrid);
                }
            },
            rowDoubleClick: function(){
                if(!this.parent.shipToOnlineProductsToolStrip.editButton.disabled){
                    this.parent.shipToOnlineProductsToolStrip.editButtonAction();
                }
            },
            startEditingNew: function(newValues, suppressFocus){
                this.Super("startEditingNew", [newValues, suppressFocus]);
            }
        });

        this.shipToOnlineProductsToolStrip = isc.CEGridBoundToolStrip.create({
            parent: this,
            listGrid: this.shipToHybrisOnlineProductsListGrid,
            refreshButtonAction: function(){
                this.Super("refreshButtonAction", arguments);
                clearData(this.parent.shipToHybrisOnlineAccountsListGrid);
            },
            editButtonAction: function(){
                var editRow = this.listGrid.getAllEditRows();
                var focusRow = this.listGrid.getFocusRow();
                var recordNum;
                var hasEditRow = (typeof editRow[0] != "undefined" && editRow[0] !== null);

                // If this is an edit of an already comitted record,
                // don't allow the user to change the product
                // else allow it.
                //
                if(hasEditRow){
                    this.listGrid.fields.find({name: "PRODUCT_VARIANT_ID"}).canEdit = true;
                    this.listGrid.fields.find({name: "CREATION_DATE"}).canEdit = true;
                }else{
                    this.listGrid.fields.find({name: "PRODUCT_VARIANT_ID"}).canEdit = false;
                    this.listGrid.fields.find({name: "CREATION_DATE"}).canEdit = false;
                }
                recordNum = hasEditRow ? editRow[0] : this.listGrid.getFocusRow();

                this.listGrid.setEditValue(recordNum, "SITE_ID", this.parent.parentHandler.values.SITE_ID);
                this.listGrid.setEditValue(recordNum, "CUSTOMER_SOURCE", this.parent.parentHandler.values.SITE_SOURCE);
                this.listGrid.startEditing(recordNum);
            },

            addButtonAction: function(){
                if(this.listGrid.hasChanges()){
                    isc.warn(lang_trans("correct_abandon_pending_changes", isc.locale_EAS_shared_strings.correct_abandon_pending_changes));
                }else{
                    this.listGrid.fields.find({name: "PRODUCT_VARIANT_ID"}).canEdit = true;
                    this.listGrid.startEditingNew({CUSTOMER_SOURCE: this.parent.parentHandler.values.SITE_SOURCE, SITE_ID: this.parent.parentHandler.values.SITE_ID});
                }
            }
        });

        this.shipToHybrisOnlineAccountsListGrid = isc.ListGrid.create({
            parent: this,
            name: "shipToHybrisOnlineAccountsListGrid",
            dataSource: isc.EASDataSources.CMR.hybrisUsersPCL,
            showFilterEditor: true,
            autoFetchData: false,
            alternateRecordStyles: true,
            modalEditing: true,
            selectionType: "single",
            sortFieldNum: 1,
            fields: [
                {name: "PRODUCT",
                    title: lang_trans("string_product", isc.locale_EAS_shared_strings.string_product),
                    canEdit: false,
                    detail: true,
                    optionDataSource: isc.EASDataSources.CMR.onlineProducts,
                    displayField: "PRODUCT_CODE",
                    valueField: "PRODUCT_CODE"
                },
                {name: "LOGIN_NAME",
                    title: lang_trans("login_name", isc.locale_EAS_shared_strings.login_name)
                },
                {name: "USER_TYPE",
                    title: lang_trans("user_type", isc.locale_EAS_shared_strings.user_type),
                    valueMap: {
                    	"LOCATION_EMPLOYEE": lang_trans("location.employee", "Location Employee"),
                    	"LOCATION_OWNER": lang_trans("location.owner", "Location Owner")
                    	}
                },
                {name: "PASSWD",
                    title: lang_trans("string_password", isc.locale_EAS_shared_strings.string_password),
                    width: 90
                },
                {name: "CONTACT_NAME",
                    title: lang_trans("contact_name", isc.locale_EAS_shared_strings.contact_name)
                },
                {name: "EMAIL",
                    title: lang_trans("email_address", isc.locale_EAS_shared_strings.email_address)
                },
                {name: "FAX",
                    title: lang_trans("fax_number", isc.locale_EAS_shared_strings.fax_number)
                },
                {name: "PHONE",
                    title: lang_trans("string_phone_number", isc.locale_EAS_shared_strings.string_phone_number)
                },
                {name: "IS_STORE_SWITCHABLE",
                    title: lang_trans("can_switch_stores", isc.locale_EAS_shared_strings.can_switch_stores),
                    canFilter: false,
                    optionDataSource: isc.EASDataSources.EAS.YNLOV,
                    displayField: "YES_NO_LBL",
                    valueField: "YES_NO_VAL"
                },
                {name: "ACTIVE",
                    title: lang_trans("is_active", isc.locale_EAS_shared_strings.is_active),
                    canFilter: false,
                    optionDataSource: isc.EASDataSources.EAS.YNLOV,
                    displayField: "YES_NO_LBL",
                    valueField: "YES_NO_VAL"
                }
            ],
            recordDoubleClick: function(viewer, record, recordNum, field, fieldNum, value, rawValue) {
                this.parent.shipToOnlineAccountsToolStrip.editButtonAction();
            },
            fetchData: function(criteria, callback, requestProperties){
                var product;
                var newCriteria;
                if(this.parent.parentHandler){
                    if(this.parent.shipToHybrisOnlineProductsListGrid.getSelectedRecord()){
                        if(criteria && criteria.PRODUCT){
                            product = criteria.PRODUCT;
                        }else{
                            product = this.parent.shipToHybrisOnlineProductsListGrid.getSelectedRecord().PRODUCT_CODE;
                        }
                        newCriteria = isc.addProperties({}, criteria, {
                            LOCATION_NUMBER: this.parent.parentHandler.valueChangedCallback("get_LOCATION_NUMBER"),
                            SITE_ID: this.parent.parentHandler.values.SITE_ID,
                            PRODUCT: product,
                            ACCOUNT_NUMBER: this.parent.parentHandler.valueChangedCallback("get_CUSTOMER_NUMBER")
                            });
                        this.Super("fetchData", [newCriteria, callback, requestProperties]);
                    }
                }
            },
            filterData: function(criteria, callback, requestProperties){
                var product;
                var newCriteria;
                if(this.parent.shipToHybrisOnlineProductsListGrid.getSelectedRecord()){
                    if(criteria && criteria.PRODUCT){
                        product = criteria.PRODUCT;
                    }else{
                        product = this.parent.shipToHybrisOnlineProductsListGrid.getSelectedRecord().PRODUCT_CODE;
                    }
                    newCriteria = isc.addProperties({}, criteria, {
                        LOCATION_NUMBER: this.parent.parentHandler.valueChangedCallback("get_LOCATION_NUMBER"),
                        SITE_ID: this.parent.parentHandler.values.SITE_ID,
                        PRODUCT: product,
                        ACCOUNT_NUMBER: this.parent.parentHandler.valueChangedCallback("get_CUSTOMER_NUMBER")
                        });
                    this.Super("filterData", [newCriteria, callback, requestProperties]);
                }
            },
            cellContextClick: function(record, rowNum, colNum){
                this.parent.shipToOnlineAccountsContextMenu.showContextMenu();
                return false;
            },
            editComplete: function(rowNum, colNum, newValues, oldValues, editCompletionEvent, dsResponse) {
                refreshData(this.parent.shipToHybrisOnlineAccountsListGrid);
                this.selectSingleRecord(rowNum);
            }
        });

        this.shipToOnlineAccountsToolStrip = isc.CEGridBoundToolStrip.create({
            parent: this,
            listGrid: this.shipToHybrisOnlineAccountsListGrid,
            addButtonAction: function(){
                if(this.parent.shipToHybrisOnlineProductsListGrid.anySelected()){
                    if(this.listGrid.hasChanges()){
                        isc.say(lang_trans("correct_abandon_pending_changes", isc.locale_EAS_shared_strings.correct_abandon_pending_changes));
                    }else{
                        this.listGrid.startEditingNew({
                            LISTGRID: this.listGrid.name,
                            LOCATION_NUMBER: this.parent.parentHandler.valueChangedCallback("get_LOCATION_NUMBER"),
                            PRODUCT: this.parent.shipToHybrisOnlineProductsListGrid.getSelectedRecord().PRODUCT_CODE,
                            SITE_ID: this.parent.parentHandler.values.SITE_ID,
                            ACCOUNT_NUMBER: this.parent.parentHandler.valueChangedCallback("get_CUSTOMER_NUMBER")
                        });
                    }
                }else{
                    isc.say(lang_trans("please_select_a_product", isc.locale_EAS_shared_strings.please_select_a_product));
                }
            }
        });

        this.shipToOnlineAccountsContextMenu = isc.Menu.create({
            parent: this,
            shadowDepth: 10,
            defaultWidth: "100",
            cellHeight:16,
            data: [
                {title: lang_trans("login_as_this_user", isc.locale_EAS_shared_strings.login_as_this_user),
                    click: function(target, item, menu, colNum){
                        openHybrisUser(menu.parent.shipToHybrisOnlineAccountsListGrid.getSelectedRecord());
                    }
                }
            ]
        });

        // Layout.
        //
        this.tabLayout = isc.VLayout.create({
            parent: this,
            membersMargin: 0,
            members: [
                isc.Label.create({
                    margin: 4,
                    align: "center",
                    height: 1,
                    contents: lang_trans("hybris.access", "Hybris Access"),
                    baseStyle: "headerItem"
                }),
                this.shipToOnlineProductsToolStrip,
                this.shipToHybrisOnlineProductsListGrid,
                isc.Label.create({
                    margin: 4,
                    align: "center",
                    height: 1,
                    contents: lang_trans("login_accounts", isc.locale_EAS_shared_strings.login_accounts),
                    baseStyle: "headerItem"
                }),
                this.shipToOnlineAccountsToolStrip,
                this.shipToHybrisOnlineAccountsListGrid
            ]
        });
        this.addMember(this.tabLayout);
    }
});
