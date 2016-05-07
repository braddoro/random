//**************************************************************************
//
// File        :  BillToAccess.js
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
isc.defineClass("BillToAccess", "CEWindow");

isc.applicationWindow.registeredObjects.add({
    className: "BillToAccess",
    isProtected: true,
    singleton: true,
    module: "BillToAccess",
    resource: "Window",
    operation_type: "Show"
});

isc.BillToAccess.addProperties({
    title: lang_trans("BillToAccess.title", "Bill To Access"),
    name: "BillToAccess",
    height: "100%",
    width: "100%",

    initWidget: function(){
        this.Super("initWidget", arguments);

        this.nationalAccountAccessDS = isc.JSONDataSource.create({
            parent: this,
            dataURL: isc.appData.controllerStub + "?om=BillToAccess.nationalAccountAccess",

            fields: [
                {primaryKey: true, name: "ID", type: "integer"},
                {name: "NATIONAL_ACCOUNT_CUST_ID", type: "integer"},
                {name: "PRODUCT_GROUP_ID", type: "integer"},
                {name: "PRECEDENCE", type: "integer"},
                {name: "MANDATORY_FLAG", type: "text"},
                {name: "COMMENTS", type: "text"},
                {name: "LAST_UPDATED_BY", type: "text"},
                {name: "LAST_UPDATE_DATE", type: "date"},
                {name: "CREATED_BY", type: "text"},
                {name: "CREATION_DATE", type: "date"}
            ],

            transformRequest: function(dsRequest){
                var superClassArguments = this.Super("transformRequest", dsRequest);

                var baseArgs = {
                    SORT_BY: dsRequest.sortBy,
                    START: dsRequest.startRow,
                    END: dsRequest.endRow
                };

                var requestArguments = (typeof dsRequest.oldValues != "undefined") ? dsRequest.oldValues : {};

                var retVal = isc.addProperties({}, superClassArguments, baseArgs, requestArguments, dsRequest.data);

                for(fieldName in this.fields){
                    if(typeof retVal[fieldName] == "undefined" || retVal[fieldName] === null){
                        retVal[fieldName] = "";
                    }
                }

                return retVal;
            },

            transformResponse: function(dsResponse, dsRequest, data){

                var createdDSResponse = {
                    totalRows: data.DSResponse.totalRows,
                    startRow: data.DSResponse.startRow,
                    endRow: data.DSResponse.endRow,
                    clientContext: dsRequest.clientContext,
                    httpResponseCode: dsResponse.httpResponseCode,
                    transactionNum: dsResponse.transactionNum,
                    status: data.DSResponse.status ? data.DSResponse.status : isc.RPCResponse.STATUS_SUCCESS,
                    errors: data.DSResponse.errors,
                    errorMessage: data.DSResponse.errorMessage
                };

                return isc.addProperties({}, {data: data.data}, createdDSResponse);
            },

            handleError: function(response, request){
                isc.warn(response.errorMessage, null, {title: lang_trans("error_code_error", isc.locale_EAS_shared_strings.error_code_error)});
            }

        });

        this.nationalAccountAccessListGrid = isc.ListGrid.create({
            parent: this,
            name: "nationalAccountAccessListGrid",
            showFilterEditor: true,
            dataSource: this.nationalAccountAccessDS,
            autoFetchData: true,
            selectionType: "single",
            selectOnEdit: true,
            canEdit: true,
            modalEditing: true,
            sortFieldNum: 0,
            sortDirection: "ascending",
            margin: "4",

            fields: [
                {name: "NATIONAL_ACCOUNT_CUST_ID",
                    title: lang_trans("ProgramName", isc.locale_EAS_shared_strings.ProgramName),
                    headerTitleStyle: "headerTitleRequired",
                    width: 195,
                    align: "left",
                    optionDataSource: isc.EASDataSources.CMR.nationalAccounts,
                    displayField: "NATIONAL_ACCOUNT_NAME",
                    valueField: "NATIONAL_ACCOUNT_CUST_ID",
                    required: true
                },
                {name: "PRODUCT_GROUP_ID",
                    title: lang_trans("product_group", isc.locale_EAS_shared_strings.product_group),
                    headerTitleStyle: "headerTitleRequired",
                    width: 150,
                    align: "left",
                    optionDataSource: isc.EASDataSources.CMR.productGroups,
                    displayField: "NAME",
                    valueField: "ID",
                    required: true
                },
                {name: "PRECEDENCE",
                    title: lang_trans("precedence", isc.locale_EAS_shared_strings.precedence),
                    width: 80,
                    headerTitleStyle: "headerTitleRequired",
                    required: true
                },
                {name: "MANDATORY_FLAG",
                    title: lang_trans("nationalAccountAccessListGrid.MANDATORY_FLAG.title", "Mandatory"),
                    headerTitleStyle: "headerTitleRequired",
                    width: 65,
                    optionDataSource: isc.EASDataSources.EAS.YNLOV,
                    displayField: "YES_NO_LBL",
                    valueField: "YES_NO_VAL",
                    defaultValue: "N",
                    required: true
                },
                {name: "COMMENTS",
                    title: lang_trans("string_comments", isc.locale_EAS_shared_strings.string_comments)
                },
                {name: "LAST_UPDATED_BY",
                    title: lang_trans("field_last_updated_by", isc.locale_EAS_shared_strings.field_last_updated_by),
                    detail: true
                },
                {name: "LAST_UPDATE_DATE",
                    title: lang_trans("field_last_update_date", isc.locale_EAS_shared_strings.field_last_update_date),
                    detail: true
                },
                {name: "CREATED_BY",
                    title: lang_trans("field_created_by", isc.locale_EAS_shared_strings.field_created_by),
                    detail: true
                },
                {name: "CREATION_DATE",
                    title: lang_trans("field_creation_date", isc.locale_EAS_shared_strings.field_creation_date),
                    detail: true
                }
            ]
        });

        this.nationalAccountAccessToolStrip = isc.CEGridBoundToolStrip.create({
            parent: this,
            listGrid: this.nationalAccountAccessListGrid
        });

        this.nationalAccountAccessToolStrip.addMembers([
            isc.ToolStripSeparator.create({width: 8, height: 12, vertical: true}),
            isc.IButton.create({
                parent: this,
                title: lang_trans("product_groups_elipsis", isc.locale_EAS_shared_strings.product_groups_elipsis),
                autoFit: true,
                click: function(){
                    dbug("nationalAccountAccessToolStrip.productGroup.click()", "+");

                    isc.applicationWindow.showWindow("ProductGroups",{visibility_name: this.parent.name});

                    dbug("nationalAccountAccessToolStrip.productGroup.click()", "-");
                }
            })
        ]);

        this.nationalAccountAccessVLayout = isc.VLayout.create({
            parent: this,
            members: [
                isc.Label.create({
                    margin: 4,
                    align: "center",
                    height: 1,
                    contents: lang_trans("nationalAccountAccessVLayout.contents", "Bill To Access"),
                    baseStyle: "headerItem"
                }),
                this.nationalAccountAccessToolStrip,
                this.nationalAccountAccessListGrid
            ]
        });

        this.addItem(this.nationalAccountAccessVLayout);
    },

    windowInitialize: function(initData){
        dbug("windowInitialize()", "+");

        this.initData = initData;

        dbug("windowInitialize()", "-");
    },

    closeClick: function(){
        dbug("closeClick()", "+");

        isc.applicationWindow.closeWindow(this.name);

        dbug("closeClick()", "-");
    }
});
