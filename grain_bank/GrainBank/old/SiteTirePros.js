//**************************************************************************
//
// File        :  SiteTirePros.js
//
// Copyright   :  Copyright 2012 American Tire Distributors, Inc.
//
// Author(s)   :  Brad Hughes - bhughes@atd-us.com
//
//                American Tire Distributors
//                12200 Herbert Wayne Ct.
//                Huntersville, NC 28078
//
//**************************************************************************
isc.SiteTireProsLayout = isc.VLayout.create({
    parent: this,
    title: "SiteTirePros",
    name: "SiteTirePros",
    initWidget: function(initData){
        this.Super("initWidget", arguments);

        this.shipToTireProsMarketingProgramsListGrid = isc.ListGrid.create({
            parent: this,
            name: "shipToTireProsMarketingProgramsListGrid",
            dataSource: isc.EASDataSources.Site.siteMarketingPrograms,
            margin: 4,
            canEdit: true,
            alternateRecordStyles: true,
            autoFetchData: false,
            selectionType: "single",
            modalEditing: true,
            fields: [
                {name: "MEMBERSHIP_TYPE",
                    title: lang_trans("string_program", isc.locale_EAS_shared_strings.string_program),
                    headerTitleStyle: "headerTitleRequired",
                    type: "text",
                    editorType: "SelectItem",
                    optionDataSource: isc.EASDataSources.Site.marketingProgramNames,
                    displayField: "MARKETING_PROGRAM_VAL",
                    valueField: "MARKETING_PROGRAM_LBL",
                    required: true,
                    editorProperties: {
                        getPickListFilterCriteria: function(){
                            return {PROGRAM_TYPE: "TIREPROS"};
                        }
                	}
                },
                {name: "MEMBERSHIP_NUMBER",
                    title: lang_trans("membership_no", isc.locale_EAS_shared_strings.membership_no),
                    headerTitleStyle: "headerTitleRequired",
                    required: true,
                    editorType: "ComboBoxItem",
                    allowEmptyValue: true,
                    autoFetchData: false,
                    optionDataSource: isc.EASDataSources.Site.marketingProgramOptions,
                    displayField: "PROGRAM_OPTION_LBL",
                    valueField: "PROGRAM_OPTION_VAL",
                    editorProperties: {
                        getPickListFilterCriteria: function(){
                            var programName = this.grid.getEditedCell(this.rowNum, "MEMBERSHIP_TYPE");
                            return {FILTER_TYPE: programName};
                        },
                        emptyPickListMessage: lang_trans("empty_pick_list_message", isc.locale_EAS_shared_strings.empty_pick_list_message)
                    }
                },
                {name: "START_DATE",
                    title: lang_trans("field_start_date", isc.locale_EAS_shared_strings.field_start_date),
                    headerTitleStyle: "headerTitleRequired",
                    width: "120",
                    editorType: "DateItem",
                    required: true,
                    datePairedField: "END_DATE",
                    validators: [{type: "isDate"},{type: "dateIsBefore"}]
                },
                {name: "END_DATE",
                    title: lang_trans("field_end_date", isc.locale_EAS_shared_strings.field_end_date),
                    width: "120",
                    editorType: "DateItem",
                    datePairedField: "START_DATE",
                    validators: [{type: "isDate"},{type: "dateIsAfter"}]
                },
                {name: "CREATION_DATE",
                    title: lang_trans("field_create_date", isc.locale_EAS_shared_strings.field_create_date),
                    width: "120",
                    detail: true,
                    canEdit: false
                },
                {name: "CREATED_BY_NAME",
                    title: lang_trans("field_created_by", isc.locale_EAS_shared_strings.field_created_by),
                    width: "120",
                    detail: true,
                    canEdit: false
                },
                {name: "DELETED_FLAG",
                    title: lang_trans("field_deleted", isc.locale_EAS_shared_strings.field_deleted),
                    align: "center",
                    width: "60",
                    detail: true,
                    canEdit: false
                }
            ],
            editComplete: function(){
                refreshData(this.parent.shipToTireProsMarketingProgramsListGrid);
                refreshData(this.parent.shipToTireProsMarketingProgramsHistoryListGrid);
            },

            fetchData: function(criteria, callback, requestProperties){
                var moreCriteria = {
                    CUSTOMER_SOURCE: this.parent.parentObject.getInstanceProperty("gSiteSource"),
                    SITE_ID: this.parent.parentObject.getInstanceProperty("gSiteID"),
                    DELETED_FLAG: "N",
                    PROGRAM_TYPE: "TIREPROS"
                };
                var newCriteria = isc.addProperties({}, criteria, moreCriteria);
                return this.Super("fetchData", [newCriteria, callback, requestProperties]);
            },
            startEditingNew: function(newValues, suppressFocus){
                var morecriteria = "";
                morecriteria = {
                    CUSTOMER_SOURCE: this.parent.parentObject.getInstanceProperty("gSiteSource"),
                    SITE_ID: this.parent.parentObject.getInstanceProperty("gSiteID")
                };
                newValues = isc.addProperties({}, newValues, morecriteria);
                return this.Super("startEditingNew", [newValues, suppressFocus]);
            },
            canEditCell: function(rowNum, colNum){
                var canEdit = false;
                var record = this.getRecord(rowNum);
                var message = lang_trans("deleted_not_edited", isc.locale_EAS_shared_strings.deleted_not_edited);
                if (typeof record == "undefined" || record === null) {
                    // adding new record
                    canEdit = this.Super("canEditCell", arguments);
                }else if(typeof record.DELETED_FLAG != "undefined" && record.DELETED_FLAG == "Y"){
                    // current record has been marked deleted
                    isc.say(message);
                    canEdit = false;
                }else if (typeof record.ID !== "undefined" && record.ID){
                    // updates are only allowed to START_DATE and END_DATE
                    if(this.getFieldName(colNum) == "START_DATE" || this.getFieldName(colNum) == "END_DATE"){
                        canEdit = true;
                    }else{
                        canEdit = false;
                    }
                }else{
                    canEdit = this.Super("canEditCell", arguments);
                }
                return canEdit;
            },
            rowDoubleClick: function(){
                if(!this.parent.shipToTireProsMarketingProgramsToolStrip.editButton.disabled){
                    this.parent.shipToTireProsMarketingProgramsToolStrip.editButtonAction();
                }
            }
        });

        this.shipToTireProsMarketingProgramsToolStrip = isc.CEGridBoundToolStrip.create({
            parent: this,
            listGrid: this.shipToTireProsMarketingProgramsListGrid,

            addButtonAction: function(){
                var message = lang_trans("correct_abandon_pending_changes", isc.locale_EAS_shared_strings.correct_abandon_pending_changes);
                if(this.listGrid.hasChanges()){
                    isc.say(message);
                }else{
                    this.listGrid.startEditingNew();
                }
            },

            removeButtonAction: function(){
                var grid = this.listGrid;
                var message;
                var messageAreYouSure = lang_trans("remove_selected_record", isc.locale_EAS_shared_strings.remove_selected_record);
                var messageAdjustment = lang_trans(
                    "SiteMarketingLayout.shipToMarketingProgramsToolStrip.removeButtonAction.adjustment.message",
                    "Removing a Marketing Program Participation record completely removes the marketing program from this customer site and may result in monetary adjustments to the customer\'s account.<br><br>If you want to end the program participation edit the record and set an end date.");
                var messageDeleted = lang_trans("deleted.record", isc.locale_EAS_shared_strings.deleted_record);
                var messageRemove = lang_trans("select_record_remove", isc.locale_EAS_shared_strings.select_record_remove);

                if (this.parent.parentObject.getInstanceProperty("gSiteSource") === "LIVE"){
                    message = messageAdjustment + "<br><br>" + message;
                }
                if(grid){
                    if(grid.anySelected()){
                        if (grid.getSelectedRecord().DELETED_FLAG == "Y"){
                            isc.say(messageDeleted);
                        }else{
                            isc.ask(message, function(value){
                              if(value){
                                  grid.removeSelectedData(function(dsResponse){
                                    if(dsResponse.status === 0) {
                                        grid.data.invalidateCache();
                                        grid.fetchData();
                                        grid.parent.shipToTireProsMarketingProgramsHistoryListGrid.data.invalidateCache();
                                        grid.parent.shipToTireProsMarketingProgramsHistoryListGrid.fetchData();
                                    }
                                });
                                }
                            });
                        }
                    }else{
                        isc.say(messageRemove);
                    }
                }
            }
        });

        this.shipToTireProsMarketingProgramsHistoryListGrid = isc.ListGrid.create({
            parent: this,
            name: "shipToTireProsMarketingProgramsHistoryListGrid",
            dataSource: isc.EASDataSources.Site.siteMarketingPrograms,
            margin: 4,
            canEdit: false,
            alternateRecordStyles: true,
            autoFetchData: false,
            selectionType: "single",
            fields: [
                {name: "MEMBERSHIP_TYPE",
                    title: lang_trans("string_program", isc.locale_EAS_shared_strings.string_program)
                },
                {name: "MEMBERSHIP_NUMBER",
                    title: lang_trans("membership_no", isc.locale_EAS_shared_strings.membership_no)
                },
                {name: "START_DATE",
                    title: lang_trans("field_start_date", isc.locale_EAS_shared_strings.field_start_date),
                    width: "120"
                },
                {name: "END_DATE",
                    title: lang_trans("field_end_date", isc.locale_EAS_shared_strings.field_end_date),
                    width: "120"
                },
                {name: "CREATION_DATE",
                    title: lang_trans("field_create_date", isc.locale_EAS_shared_strings.field_create_date),
                    width: "120"
                },
                {name: "CREATED_BY_NAME",
                    title: lang_trans("field_created_by", isc.locale_EAS_shared_strings.field_created_by),
                    width: "120"
                },
                {name: "LAST_UPDATE_DATE",
                    title: lang_trans("field_removed_date", isc.locale_EAS_shared_strings.field_removed_date),
                    width: "120"
                },
                {name: "LAST_UPDATED_BY_NAME",
                    title: lang_trans("field_removed_by", isc.locale_EAS_shared_strings.field_removed_by),
                    width: "120"
                },
                {name: "DELETED_FLAG",
                    title: lang_trans("field_deleted", isc.locale_EAS_shared_strings.field_deleted),
                    align: "center",
                    width: "60",
                    detail: true
                }
            ],
            fetchData: function(criteria, callback, requestProperties){
                var moreCriteria = {
                    CUSTOMER_SOURCE: this.parent.parentObject.getInstanceProperty("gSiteSource"),
                    SITE_ID: this.parent.parentObject.getInstanceProperty("gSiteID"),
                    DELETED_FLAG: "Y",
                    PROGRAM_TYPE: "TIREPROS"
                };
                var newCriteria = isc.addProperties({}, criteria, moreCriteria);
                return this.Super("fetchData", [newCriteria, callback, requestProperties]);
            }
        });

        this.shipToTireProsMarketingProgramsVLayout = isc.VLayout.create({
            parent: this,
            members: [
                this.shipToTireProsMarketingProgramsToolStrip,
                this.shipToTireProsMarketingProgramsListGrid
            ]
        });

        this.shipToTireProsMarketingProgramsHistoryVLayout = isc.VLayout.create({
            parent: this,
            members: [
                this.shipToTireProsMarketingProgramsHistoryListGrid
            ]
        });

        this.TireProsMarketingProgramsTabSet = isc.TabSet.create({
            parent: this,
            height: "60%",
            tabs: [
                {name: "activeTireProsMarketingPrograms",
                    title: lang_trans("active", isc.locale_EAS_shared_strings.active),
                    pane: this.shipToTireProsMarketingProgramsVLayout
                },
                {name: "pastTireProsMarketingPrograms",
                    title: lang_trans("string_history", isc.locale_EAS_shared_strings.string_history),
                    pane: this.shipToTireProsMarketingProgramsHistoryVLayout
                }
            ]
        });

        this.tireProsDealerListGrid = isc.ListGrid.create({
            parent: this,
            name: "tireProsDealerListGrid",
            dataSource: isc.EASDataSources.Site.dealerOptional,
            canEdit: true,
            alternateRecordStyles: true,
            selectionType: "single",
            height: "100%",
            canSort: false,
            fields: [
                {name: "ELEMENT_NAME",
                    title: lang_trans("string_name", isc.locale_EAS_shared_strings.string_name),
                    type: "text",
                    editorType: "SelectItem",
                    optionDataSource: isc.EASDataSources.EAS.genericValueSet,
                    optionCriteria: {VALUESET: "XXATDTPR_TIREPRO_MKT_FLEX_ATTR"},
                    displayField: "DESCRIPTION",
                    valueField: "FLEX_VALUE",
                    required: true
                },
                {name: "ELEMENT_VALUE",
                    title: lang_trans("tireProsDealerListGrid.ELEMENT_VALUE.title", "Value"),
                    editorType: "TextItem"
                },
                {name: "CREATION_DATE",
                    title: lang_trans("field_create_date", isc.locale_EAS_shared_strings.field_create_date),
                    width: "120",
                    detail: true
                },
                {name: "CREATED_BY_NAME",
                    title: lang_trans("field_created_by", isc.locale_EAS_shared_strings.field_created_by),
                    width: "120",
                    detail: true
                },
                {name: "LAST_UPDATE_DATE",
                    title: lang_trans("field_last_update_date", isc.locale_EAS_shared_strings.field_last_update_date),
                    width: "120",
                    detail: true
                },
                {name: "LAST_UPDATED_BY_NAME",
                    title: lang_trans("field_last_updated_by", isc.locale_EAS_shared_strings.field_last_updated_by),
                    width: "120",
                    detail: true
                }
            ],
            fetchData: function(criteria, callback, requestProperties){
                var moreCriteria = {
                    CUSTOMER_SOURCE: this.parent.parentObject.getInstanceProperty("gSiteSource"),
                    SITE_ID: this.parent.parentObject.getInstanceProperty("gSiteID")
                };
                var newCriteria = isc.addProperties({}, criteria, moreCriteria);
                return this.Super("fetchData", [newCriteria, callback, requestProperties]);
            },
            saveData: function(callback, requestProperties){
                this.setValue("CUSTOMER_SOURCE", this.parent.parentObject.getInstanceProperty("gSiteSource"));
                this.setValue("SITE_ID", this.parent.parentObject.getInstanceProperty("gSiteID"));
                return this.Super("saveData", [{target: this, methodName: "fetchComplete"}, requestProperties]);
            },
            editComplete: function(rowNum, colNum, newValues, oldValues, editCompletionEvent, dsResponse){
                this.invalidateCache();
                this.fetchData();
            },
            rowDoubleClick: function(record, recordNum, fieldNum, keyboardGenerated){
                if(!this.parent.dealerToolStrip.editButton.disabled){
                    this.parent.dealerToolStrip.editButtonAction();
                }
            }
        });

        this.dealerToolStrip = isc.CEGridBoundToolStrip.create({
            parent: this,
            listGrid: this.tireProsDealerListGrid,

            addButtonAction: function(){
                var moreCriteria = {};
                var message = lang_trans("correct_abandon_pending_changes", isc.locale_EAS_shared_strings.correct_abandon_pending_changes);
                if(this.listGrid.hasChanges()){
                    isc.say(message);
                }else{
                    moreCriteria = {
                        CUSTOMER_SOURCE: this.parent.parentObject.getInstanceProperty("gSiteSource"),
                        SITE_ID: this.parent.parentObject.getInstanceProperty("gSiteID")
                    };
                    this.listGrid.getField("ELEMENT_NAME").canEdit = true;
                    this.listGrid.startEditingNew(moreCriteria);
                }
            },

            editButtonAction: function(){
                var editRow = this.listGrid.getAllEditRows();
                var focusRow = this.listGrid.getFocusRow();
                var hasEditRow = (typeof editRow[0] != "undefined" && editRow[0] !== null);
                var recordNum = hasEditRow ? editRow[0] : focusRow;
                this.listGrid.setEditValue(recordNum, "SITE_ID", this.parent.parentObject.getInstanceProperty("gSiteID"));
                this.listGrid.setEditValue(recordNum, "CUSTOMER_SOURCE", this.parent.parentObject.getInstanceProperty("gSiteSource"));
                this.listGrid.getField("ELEMENT_NAME").canEdit = false;
                this.listGrid.startEditing(recordNum);
            },

            removeButtonAction: function(){
                var grid = this.listGrid;
                var message = lang_trans("remove_selected_record", isc.locale_EAS_shared_strings.remove_selected_record);
                var messageRemove = lang_trans("select_record_remove", isc.locale_EAS_shared_strings.select_record_remove);
                if(grid){
                    if(grid.anySelected()){
                        isc.ask(message, function(value){
                          if(value){
                              grid.removeSelectedData(function(dsResponse){
                                if(dsResponse.status === 0) {
                                    grid.data.invalidateCache();
                                    grid.fetchData();
                                }
                            });
                            }
                        });
                    }else{
                        isc.say(messageRemove);
                    }
                }
            }
        });

        this.tireProsDealerDForm = isc.DynamicForm.create({
            parent: this,
            name: "tireProsDealerDForm",
            validateForm: true,
            dataSource: isc.EASDataSources.Site.dealerCore,
            wrapItemTitles: false,
            canEdit: false,
            fields: [
                {name: "TIREPROS_BUSINESS_NAME",
                    title: lang_trans("tireProsDealerDForm.TIREPROS_BUSINESS_NAME.title", "TirePros Business Name"),
                    width: 250,
                    editorType: "TextItem",
                    validators: [{type: "lengthRange", max: 240}]
                },
                {name: "TIREPROS_OWNER_NAME",
                    title: lang_trans("tireProsDealerDForm.TIREPROS_OWNER_NAME.title", "TirePros Owner(s)"),
                    width: 250,
                    editorType: "TextItem",
                    validators: [{type: "lengthRange", max: 240}]
                },
                {name: "STORE_ID",
                    title: lang_trans("tireProsDealerDForm.STORE_ID.title", "TirePros Center ID"),
                    width: 200,
                    editorType: "TextItem",
                    validators: [{type: "lengthRange", max: 20}]
                },
                {name: "NEW_DEALER_DATE",
                    title: lang_trans("tireProsDealerDForm.NEW_DEALER_DATE.title", "New Dealer/Re-Sign Year"),
                    editorType: "DateItem",
                    useTextField: true,
                    width: 100,
                    validators: [{type: "isDate"}]
                },
                {name: "TIREPROS_EFFECT_CONTRACT_DATE",
                    title: lang_trans("tireProsDealerDForm.TIREPROS_EFFECT_CONTRACT_DATE.title", "TirePros Effective Contract Date"),
                    editorType: "DateItem",
                    useTextField: true,
                    width: 100,
                    validators: [{type: "isDate"}]
                },
                {name: "CONTRACT_END_DATE",
                    title: lang_trans("tireProsDealerDForm.CONTRACT_END_DATE.title", "Contract Expiration Date"),
                    editorType: "DateItem",
                    useTextField: true,
                    width: 100,
                    validators: [{type: "isDate"}]
                },
                {name: "ORIGINAL_PROGRAM_START_DATE",
                    title: lang_trans("tireProsDealerDForm.ORIGINAL_PROGRAM_START_DATE.title", "Original Program Start Date"),
                    editorType: "DateItem",
                    useTextField: true,
                    width: 100,
                    validators: [{type: "isDate"}]
                },
                {name: "ENROLLMENT_TRACKING_DATE",
                    title: lang_trans("tireProsDealerDForm.ENROLLMENT_TRACKING_DATE.title", "Enrollment Tracking Date"),
                    editorType: "DateItem",
                    useTextField: true,
                    width: 100,
                    validators: [{type: "isDate"}]
                },
                {name: "BILLING_START_DATE",
                    title: lang_trans("tireProsDealerDForm.BILLING_START_DATE.title", "Billing Start Date"),
                    editorType: "DateItem",
                    useTextField: true,
                    width: 100,
                    validators: [{type: "isDate"}]
                },
                {name: "TP_FRANCHISE_FEE",
                    title: lang_trans("tireProsDealerDForm.TP_FRANCHISE_FEE.title", "TirePros Franchise Fee"),
                    width: 100,
                    editorType: "TextItem",
                    validators: [{type: "lengthRange", max: 240}]
                },
                {name: "INITIAL_LICENSE_FEE",
                    title: lang_trans("tireProsDealerDForm.INITIAL_LICENSE_FEE.title", "Intial License Fee"),
                    editorType: "TextItem",
                    validators: [{type: "lengthRange", max: 240}],
                    width: 100,
                    changed: function(form, item, value){
                        if(trim(value) !== ""  && value !== null && !form.getField("INITIAL_LICENSE_FEE").disabled){
                            form.getField("DATE_INITIAL_LICENSE_FEE_REC").setDisabled(false);
                            form.getField("INITIAL_LICENSE_FEE_NOTE").setDisabled(false);
                        }else{
                            form.setValue("INITIAL_LICENSE_FEE_NOTE", "");
                            form.getField("INITIAL_LICENSE_FEE_NOTE").setDisabled(true);
                            form.setValue("DATE_INITIAL_LICENSE_FEE_REC", "");
                            form.getField("DATE_INITIAL_LICENSE_FEE_REC").setDisabled(true);
                        }
                    }
                },
                {name: "INITIAL_LICENSE_FEE_NOTE",
                    title: lang_trans("tireProsDealerDForm.INITIAL_LICENSE_FEE_NOTE.title", "Initial License Fee Note"),
                    width: 250,
                    height: 75,
                    editorType: "textArea",
                    validators: [{type: "lengthRange", max: 240}],
                    change: function(form, item, value, oldValue) {
                        var chars = 0;
                        var newHint = "";
                        var tooLong = lang_trans("tireProsDealerDForm.INITIAL_LICENSE_FEE_NOTE.change.TooLong", "Too long");
                        var spaceLeft = lang_trans("tireProsDealerDForm.INITIAL_LICENSE_FEE_NOTE.change.SpaceLeft", "Space left");
                        if(value){
                            chars = (240-value.length);
                            if (chars < 0) {
                                newHint = tooLong + ': ' + (value.length-240);
                            }else{
                                newHint = spaceLeft + ': ' + (240-value.length);
                            }
                        }
                        item.setHint(newHint);
                    }
                },
                {name: "DATE_INITIAL_LICENSE_FEE_REC",
                    title: lang_trans("tireProsDealerDForm.DATE_INITIAL_LICENSE_FEE_REC.title", "Date Intial License Fee Received"),
                    editorType: "DateItem",
                    useTextField: true,
                    linkedField: "INITIAL_LICENSE_FEE",
                    width: 100,
                    validators: [
                        {type: "isDate"},
                        {type: "custom",
                            condition: function(item, validator, value){
                                var msgPart1 = lang_trans("tireProsDealerDForm.DATE_INITIAL_LICENSE_FEE_REC.condition.part1", "When ");
                                var msgPart2 = lang_trans("tireProsDealerDForm.DATE_INITIAL_LICENSE_FEE_REC.condition.part2", " is specified ");
                                var msgPart3 = lang_trans("tireProsDealerDForm.DATE_INITIAL_LICENSE_FEE_REC.condition.part3", " is a required field.");

                                var retVal = true;
                                if( item.form.validateForm && trim(item.form.getValue(item.linkedField)) !== "" && (trim(value) === "" || value === null) ){
                                    retVal = false;
                                    validator.errorMessage = msgPart1 + item.form.getField(item.linkedField).title + msgPart2 + item.title + msgPart3;
                                }
                                return retVal;
                            }
                        }
                    ]
                },
                {name: "DATE_FRANCHISE_AGREEMENT_SENT",
                    title: lang_trans("tireProsDealerDForm.DATE_FRANCHISE_AGREEMENT_SENT.title", "Date Franchise Agreement Sent"),
                    editorType: "DateItem",
                    useTextField: true,
                    width: 100,
                    validators: [{type: "isDate"}]
                },
                {name: "INITIAL_ID_FUNDS",
                    title: lang_trans("tireProsDealerDForm.INITIAL_ID_FUNDS.title", "Initial ID Funds"),
                    width: 75,
                    isProtected: true,
                    editorType: "TextItem",
                    validators: [{type: "isInteger"}, {type: "lengthRange", max: 240}]
                },
                {name: "INSURANCE_CERT",
                    title: lang_trans("tireProsDealerDForm.INSURANCE_CERT.title", "Insurance Certification"),
                    width: 250,
                    editorType: "TextItem",
                    validators: [{type: "lengthRange", max: 240}]
                },
                {name: "COUNCIL_ZONE",
                    title: lang_trans("tireProsDealerDForm.COUNCIL_ZONE.title", "TirePros Council Zone"),
                    width: 100,
                    editorType: "SelectItem",
                    valueMap: {
                        "Midwest": lang_trans("COUNCIL_ZONE.Midwest", "Midwest"),
                        "Northeast": lang_trans("COUNCIL_ZONE.Northeast", "Northeast"),
                        "Southeast": lang_trans("COUNCIL_ZONE.Southeast", "Southeast"),
                        "Western": lang_trans("COUNCIL_ZONE.Western", "Western")
                    }
                },
                {name: "REGIONAL_COUNCIL_PHONE_TREE",
                    title: lang_trans("tireProsDealerDForm.REGIONAL_COUNCIL_PHONE_TREE.title", "Regional Council Phone Tree Contact"),
                    width: 250,
                    editorType: "TextItem",
                    validators: [{type: "lengthRange", max: 240}]
                },
                {name: "TIRE_PROS_MARKET_DIRECTOR",
                    title: lang_trans("tireProsDealerDForm.TIRE_PROS_MARKET_DIRECTOR.title", "Market Director"),
                    width: 250,
                    wrapTitle: false,
                    editorType: "SelectItem",
                    optionDataSource: isc.EASDataSources.EAS.employees,
                    displayField: "EMPLOYEE_NAME",
                    valueField: "EMPLOYEE_NUM",
                    allowEmptyValue: true,
                    pickListWidth: 300,
                    pickListFields: [
                        {name: "EMPLOYEE_NUM", title: lang_trans("string_number", isc.locale_EAS_shared_strings.string_number), width: 50},
                        {name: "EMPLOYEE_NAME", title: lang_trans("string_name", isc.locale_EAS_shared_strings.string_name)}
                    ]
                },
                {name: "TIRE_PROS_ACCOUNT_DIRECTOR",
                    title: lang_trans("tireProsDealerDForm.TIRE_PROS_ACCOUNT_DIRECTOR.title", "Account Director"),
                    width: 250,
                    wrapTitle: false,
                    editorType: "SelectItem",
                    optionDataSource: isc.EASDataSources.EAS.employees,
                    displayField: "EMPLOYEE_NAME",
                    valueField: "EMPLOYEE_NUM",
                    allowEmptyValue: true,
                    pickListWidth: 300,
                    pickListFields: [
                        {name: "EMPLOYEE_NUM", title: lang_trans("string_number", isc.locale_EAS_shared_strings.string_number), width: 50},
                        {name: "EMPLOYEE_NAME", title: lang_trans("string_name", isc.locale_EAS_shared_strings.string_name)}
                    ]
                },
                {name: "TIRE PROS CUST SERVICE",
                    title: lang_trans("tireProsDealerDForm.TIRE PROS CUST SERVICE.title", "TirePros Director"),
                    width: 250,
                    wrapTitle: false,
                    editorType: "SelectItem",
                    optionDataSource: isc.EASDataSources.EAS.employees,
                    displayField: "EMPLOYEE_NAME",
                    valueField: "EMPLOYEE_NUM",
                    allowEmptyValue: true,
                    pickListWidth: 300,
                    pickListFields: [
                        {name: "EMPLOYEE_NUM", title: lang_trans("string_number", isc.locale_EAS_shared_strings.string_number), width: 50},
                        {name: "EMPLOYEE_NAME", title: lang_trans("string_name", isc.locale_EAS_shared_strings.string_name)}
                    ]
                },
                {name: "TIRE PROS SALES",
                    title: lang_trans("tireProsDealerDForm.TIRE PROS SALES.title", "TirePros Retail Business Consultant"),
                    width: 250,
                    wrapTitle: false,
                    editorType: "SelectItem",
                    optionDataSource: isc.EASDataSources.EAS.employees,
                    displayField: "EMPLOYEE_NAME",
                    valueField: "EMPLOYEE_NUM",
                    allowEmptyValue: true,
                    pickListWidth: 300,
                    pickListFields: [
                        {name: "EMPLOYEE_NUM", title: lang_trans("string_number", isc.locale_EAS_shared_strings.string_number), width: 50},
                        {name: "EMPLOYEE_NAME", title: lang_trans("string_name", isc.locale_EAS_shared_strings.string_name)}
                    ]
                },
                {name: "TIRE PROS MARKETING",
                    title: lang_trans("tireProsDealerDForm.TIRE PROS MARKETING.title", "TirePros Marketing Manager"),
                    width: 250,
                    wrapTitle: false,
                    editorType: "SelectItem",
                    optionDataSource: isc.EASDataSources.EAS.employees,
                    displayField: "EMPLOYEE_NAME",
                    valueField: "EMPLOYEE_NUM",
                    allowEmptyValue: true,
                    pickListWidth: 300,
                    pickListFields: [
                        {name: "EMPLOYEE_NUM", title: lang_trans("string_number", isc.locale_EAS_shared_strings.string_number), width: 50},
                        {name: "EMPLOYEE_NAME", title: lang_trans("string_name", isc.locale_EAS_shared_strings.string_name)}
                    ]
                },
                {name: "DEALER_PURCH_FORM_REC",
                    title: lang_trans("tireProsDealerDForm.DEALER_PURCH_FORM_REC.title", "Dealer Purchase Incentive Form Received"),
                    width: 250,
                    editorType: "TextItem",
                    validators: [{type: "lengthRange", max: 240}]
                },
                {name: "FIVE_DIAMOND_RATING",
                    title: lang_trans("tireProsDealerDForm.FIVE_DIAMOND_RATING.title", "Five Diamond Rating"),
                    width: 75,
                    editorType: "TextItem",
                    validators: [{type: "isInteger"}, {type: "lengthRange", max: 240}]
                },
                {name: "POS_SYSTEM",
                    title: lang_trans("tireProsDealerDForm.POS_SYSTEM.title", "POS System"),
                    width: 200,
                    editorType: "TextItem",
                    validators: [{type: "lengthRange", max: 20}]
                },
                {name: "TP_ROADSIDE_ASSISTANCE_FEE",
                    title: lang_trans("tireProsDealerDForm.TP_ROADSIDE_ASSISTANCE_FEE.title", "TirePros Roadside Assistance Fee"),
                    width: 75,
                    editorType: "TextItem",
                    validators: [{type: "isInteger"}, {type: "lengthRange", max: 240}]
                },
                {name: "TOTAL_RETAIL_SALES",
                    title: lang_trans("tireProsDealerDForm.TOTAL_RETAIL_SALES.title", "Total Retail Sales"),
                    width: 75,
                    editorType: "TextItem",
                    validators: [{type: "isInteger"}, {type: "lengthRange", max: 240}]
                },
                {name: "SOA_EST",
                    title: lang_trans("tireProsDealerDForm.SOA_EST.title", "Share of Account"),
                    width: 50,
                    editorType: "TextItem",
                    validators: [{type: "isFloat"}, {type: "lengthRange", max: 240}],
                    hint: "%"
                },
                {name: "SALES_MIX_PERCENT_TIRES",
                    title: lang_trans("tireProsDealerDForm.SALES_MIX_PERCENT_TIRES.title", "Sales Mix Percent of Tires"),
                    width: 75,
                    editorType: "TextItem",
                    validators: [{type: "isInteger"}, {type: "lengthRange", max: 240}]
                },
                {name: "SALES_MIX_PERCENT_SERVICE",
                    title: lang_trans("tireProsDealerDForm.SALES_MIX_PERCENT_SERVICE.title", "Sales Mix Percent of Service/Labor/All"),
                    width: 75,
                    editorType: "TextItem",
                    validators: [{type: "isInteger"}, {type: "lengthRange", max: 240}],
                    hint: "%"
                },
                {name: "NO_SERVICE_BAY",
                    title: lang_trans("tireProsDealerDForm.NO_SERVICE_BAY.title", "No. of Service Bays"),
                    width: 50,
                    editorType: "TextItem",
                    validators: [{type: "isFloat"}, {type: "lengthRange", max: 240}]
                },
                {name: "BENCHMARK_MPI",
                    title: lang_trans("tireProsDealerDForm.BENCHMARK_MPI.title", "Benchmark MPI"),
                    width: 50,
                    editorType: "SelectItem",
                    optionDataSource: isc.EASDataSources.EAS.YNLOV,
                    displayField: "YES_NO_LBL",
                    valueField: "YES_NO_VAL"
                },
                {name: "CURRENT_YEAR_AA",
                    title: lang_trans("tireProsDealerDForm.CURRENT_YEAR_AA.title", "Current Year Approved Ad Agreement"),
                    width: 75,
                    editorType: "TextItem",
                    validators: [{type: "isInteger"}, {type: "lengthRange", max: 240}]
                },
                {name: "TP_POS_KITS_FEE",
                    title: lang_trans("tireProsDealerDForm.TP_POS_KITS_FEE.title", "TirePros POS Kit Fee"),
                    width: 75,
                    editorType: "TextItem",
                    validators: [{type: "isInteger"}, {type: "lengthRange", max: 240}]
                },
                {name: "POS_KIT_START_DATE",
                    title: lang_trans("tireProsDealerDForm.POS_KIT_START_DATE.title", "TirePros POS Kit Start Date"),
                    editorType: "DateItem",
                    useTextField: true,
                    disabled: true,
                    width: 100,
                    validators: [
                        {type: "isDate"},
                        {type: "custom",
                            condition: function(item, validator, value){
                                var retVal = true;
                                if( item.form.validateForm && trim(item.form.getValue(item.linkedField)) !== "" && (trim(value) === "" || value === null) ){
                                    retVal = false;
                                    validator.errorMessage = "When " + item.form.getField(item.linkedField).title + " is specified " + item.title + " is a required field.";
                                }
                                return retVal;
                            }
                        }
                    ]
                },
                {name: "NEW_DEALER_ORIENTATION",
                    title: lang_trans("tireProsDealerDForm.NEW_DEALER_ORIENTATION.title", "New Dealer Orientation"),
                    width: 250,
                    editorType: "TextItem",
                    validators: [{type: "lengthRange", max: 240}]
                },
                {name: "TIREBUYER_COM_PROFILE",
                    title: lang_trans("tireProsDealerDForm.TIREBUYER_COM_PROFILE.title", "TireBuyer.com Profile"),
                    width: 50,
                    editorType: "SelectItem",
                    optionDataSource: isc.EASDataSources.EAS.YNLOV,
                    displayField: "YES_NO_LBL",
                    valueField: "YES_NO_VAL"
                }
            ],
            fetchData: function(criteria, callback, requestProperties){
                var moreCriteria = {};
                var newCriteria = {};
                if(this.parent.parentObject){
                    moreCriteria = {
                        CUSTOMER_SOURCE: this.parent.parentObject.getInstanceProperty("gSiteSource"),
                        SITE_ID: this.parent.parentObject.getInstanceProperty("gSiteID")
                    };
                    newCriteria = isc.addProperties({}, criteria, moreCriteria);
                    return this.Super("fetchData", [newCriteria, {target: this, methodName: "showHide"}, requestProperties]);
                }
            },
            saveData: function(callback, requestProperties){
                this.setValue("CUSTOMER_SOURCE", this.parent.parentObject.getInstanceProperty("gSiteSource"));
                this.setValue("SITE_ID", this.parent.parentObject.getInstanceProperty("gSiteID"));
                return this.Super("saveData", [{target: this, methodName: "showHide"}, requestProperties]);
            },
            itemHoverHTML: function(item){
                return getHelpText(item);
            },
            saveFormIfDirty: function(){
                saveFormIfDirty(this);
            },
            changed: function(){
                this.showHide();
            },
            showHide: function() {
                if(trim(this.getValue("INITIAL_LICENSE_FEE")) !== "" && this.getValue("INITIAL_LICENSE_FEE") !== null && !this.getField("INITIAL_LICENSE_FEE").disabled) {
                    this.getField("INITIAL_LICENSE_FEE_NOTE").setDisabled(false);
                    this.getField("DATE_INITIAL_LICENSE_FEE_REC").setDisabled(false);
                    this.getField("INITIAL_LICENSE_FEE_NOTE").change(
                        this,
                        this.getField("INITIAL_LICENSE_FEE_NOTE"),
                        this.getValue("INITIAL_LICENSE_FEE_NOTE")
                    );
                }else{
                    this.getField("INITIAL_LICENSE_FEE_NOTE").setDisabled(true);
                    this.getField("DATE_INITIAL_LICENSE_FEE_REC").setDisabled(true);
                }
            },
            invalidateCache: function(){
                if(this.data && this.data.invalidateCache){
                    this.data.invalidateCache();
                }
            },
            setValidation: function(shouldValidate){
                this.validateForm = shouldValidate;
            },
            setErrors: function(errors){
                var tab;
                var errorMsg;
                var titleMessage = lang_trans("form_not_saved", isc.locale_EAS_shared_strings.form_not_saved);
                if(this.parent.parentObject){
                    tab = isc.applicationWindow.canvas.children.find({Class: this.parent.parentObject.getClassName()}).shipToTabSet.tabs.find({name: "shipToTireProsTab"});
                    for(var field in errors){
                        errorMsg = errors[field];
                        if (this.getField(field).title && errorMsg.indexOf(this.getField(field).title) === -1) {
                            errorMsg = this.getField(field).title + " - " + errorMsg;
                        }
                        isc.warn(errorMsg, null, {title: titleMessage});
                        isc.applicationWindow.canvas.children.find({Class: this.parent.parentObject.getClassName()}).shipToTabSet.selectTab(tab);
                        break;
                    }
                }
            }
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
                    contents: lang_trans("tabLayout.contents_SiteTirePros", "TirePros Marketing Program Participation"),
                    baseStyle: "headerItem"
                }),
                this.TireProsMarketingProgramsTabSet
            ]
        });

        this.dealerLayout = isc.VLayout.create({
            parent: this,
            membersMargin: 0,
            members: [
                isc.Label.create({
                    margin: 4,
                    align: "center",
                    height: 1,
                    contents: lang_trans("dealerLayout.contents", "TirePros Dealer Information"),
                    baseStyle: "headerItem"
                }),
                isc.HLayout.create({
                    parent: this,
                    margin: 4,
                    members: [
                        isc.VLayout.create({
                            parent: this,
                            margin: 4,
                            width: "50%",
                            members: [this.tireProsDealerDForm]
                        }),
                        isc.VLayout.create({
                            parent: this,
                            margin: 4,
                            width: "50%",
                            height: "100%",
                            members: [this.dealerToolStrip, this.LayoutSpacer, this.tireProsDealerListGrid]
                        })
                    ]
                })
            ]
        });

        this.mainTabSet = isc.TabSet.create({
            parent: this,
            height: "60%",
            tabs: [
                {name: "tabDealer",
                    title: lang_trans("mainTabSet.tabDealer.contents", "Dealer"),
                    pane: this.dealerLayout
                },
                {name: "tabMarketing",
                    title: lang_trans("mainTabSet.tabMarketing.contents", "Marketing"),
                    pane: this.tabLayout
                }
            ]
        });

        this.addMember(this.mainTabSet);
    }
});
