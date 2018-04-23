

function createClientPage(pageView, obj) {
    var component = Qt.createComponent("qrc:/ClientPage.qml")
	if( component.status === Component.Error )
		   console.debug("Error:"+ component.errorString() );

	var incubator = component.incubateObject(pageView,{"connectionObject":obj})

    if ( incubator.status !== Component.Ready )
    {
        component.statusChanged.connect(function()
        {           
            pageView.addItem(incubator.object)
        })
    }
    else
    {
        pageView.addItem(incubator.object)
    }
}

