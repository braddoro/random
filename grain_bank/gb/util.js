function setButtons(action, strip){
    switch(action) {
        case 'rowEditorEnter':
            strip.setInsertButtonDisabled(true);
            strip.setRefreshButtonDisabled(true);
            strip.setCancelButtonDisabled(false);
            break;
        case 'rowEditorExit':
            strip.setInsertButtonDisabled(false);
            strip.setRefreshButtonDisabled(false);
            strip.setCancelButtonDisabled(true);
            break;
        default:
            break;
    }
}

isc.defineClass('gbDataSource', 'DataSource');
isc.gbDataSource.addProperties({
	dataFormat: "json",
	fields: [],
    handleError: function(response, request) {
        return true;
    }
});

isc.defineClass('gbToolStrip', 'ToolStrip');
isc.gbToolStrip.addProperties({
    listGrid: null,
    height: 24,
    spacerWidth: 0,
//    showShadow: true,
//    shadowOffset: 1,
//    shadowSoftness: 5,
    layoutTopMargin: 5,
    membersMargin: 5,
    styleName: 'whiteToolStripBorder',
    insertButtonDisabled: false,
    refreshButtonDisabled: false,
    cancelButtonDisabled: false,
    initWidget: function(){
        this.Super('initWidget', arguments);

        this.insertButton = isc.IButton.create({
            id: 'insertButton',
            parent: this,
            title: 'Add',
            autoFit: true,
//        	showShadow: true,
//            shadowOffset: this.shadowOffset,
//            shadowSoftness: this.shadowSoftness,
            disabled: this.insertButtonDisabled,
            click: function(){
                if(this.parent.insertButtonAction){
                    this.parent.insertButtonAction();
                }
            }
        });
        this.cancelButton = isc.IButton.create({
            id: 'cancelButton',
            parent: this,
            title: 'Cancel',
            autoFit: true,
//        	showShadow: true,
//            shadowOffset: this.shadowOffset,
//            shadowSoftness: this.shadowSoftness,
            disabled: this.cancelButtonDisabled,
            click: function(){
                if(this.parent.cancelButtonAction){
                    this.parent.cancelButtonAction();
                }
            }
        });
        this.refreshButton = isc.IButton.create({
            id: 'refreshButton',
            parent: this,
            title: 'Refresh',
            autoFit: true,
//        	showShadow: true,
//            shadowOffset: this.shadowOffset,
//            shadowSoftness: this.shadowSoftness,
            disabled: this.refreshButtonDisabled,
            click: function(){
                if(this.parent.refreshButtonAction){
                    this.parent.refreshButtonAction();
                }
            }
        });

        this.addMembers([
            isc.LayoutSpacer.create({width: this.spacerWidth}),
            this.insertButton,
            this.cancelButton,
            this.refreshButton,
            isc.LayoutSpacer.create({width: this.spacerWidth})
        ]);
    },

    // insertButton
    //
    insertButtonAction: function(){
        if(this.listGrid){
            this.listGrid.startEditingNew();
        }
    },
    setInsertButtonDisabled: function(disabled){
        this.insertButton.setDisabled(disabled);
    },

    // refreshButton
    //
    refreshButtonAction: function(){
        var grid = this.listGrid;
        if(grid.hasChanges()){
            isc.ask('Refreshing will discard your unsaved edits. Do you wish to continue?', function(value){
                if(value){
                    grid.discardEdits(editRow, 0, false);
                    if(grid.data && grid.data.invalidateCache){
                        grid.data.invalidateCache();
                    }
                    grid.fetchData(grid.getCriteria());
                }
            });
        }else{
            if(grid.data && grid.data.invalidateCache){
                grid.data.invalidateCache();
            }
            if(grid.data){
                grid.fetchData(grid.getCriteria());
            }
        }
    },
    setRefreshButtonDisabled: function(disabled){
        this.refreshButton.setDisabled(disabled);
    },

    // cancelButton
    //
    cancelButtonAction: function(){
        var grid = this.listGrid;
        editRow = grid.getAllEditRows()[0];
        if(grid.hasChanges()){
            grid.discardEdits(editRow, 0, false);
        }
    },
    setCancelButtonDisabled: function(disabled){
        this.cancelButton.setDisabled(disabled);
    }
});

isc.defineClass('gbListGrid', 'ListGrid');
isc.gbListGrid = isc.ListGrid.addProperties({
    parent: this,
	width: 700,
	height: 250,
    alternateRecordStyles: true,
    autoFetchData: true,
//	showShadow: true,
//    shadowOffset: 1,
//    shadowSoftness: 5,
    fields: [],
    rowEditorEnter: function(record, editValues, rowNum){
        setButtons('rowEditorEnter', this.parent.toolStrip);
        this.Super('rowEditorEnter', [record, editValues, rowNum]);
    },
    rowEditorExit: function(editCompletionEvent, record, newValues, rowNum){
        setButtons('rowEditorExit', this.parent.toolStrip);
        this.Super('rowEditorExit', [editCompletionEvent, record, newValues, rowNum]);
    }
});

isc.defineClass('gbLabel', 'Label');
isc.gbLabel = isc.Label.addProperties({
    align: 'left',
    autofit: true,
    height: 30,
    width: 750,
    contents: null,
    baseStyle: 'headerItem'
});

isc.defineClass('gbVLayout', 'VLayout');
isc.gbVLayout = isc.VLayout.addProperties({
    margin: 5,
	parent: this,
	members: []
});
