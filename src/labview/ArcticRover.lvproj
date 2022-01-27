<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="20008000">
	<Item Name="My Computer" Type="My Computer">
		<Property Name="server.app.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.control.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.tcp.enabled" Type="Bool">false</Property>
		<Property Name="server.tcp.port" Type="Int">0</Property>
		<Property Name="server.tcp.serviceName" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.tcp.serviceName.default" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.vi.callsEnabled" Type="Bool">true</Property>
		<Property Name="server.vi.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="specify.custom.address" Type="Bool">false</Property>
		<Item Name="Motor Decoder.vi" Type="VI" URL="../Motor Decoder.vi"/>
		<Item Name="Status Decoder.vi" Type="VI" URL="../Status Decoder.vi"/>
		<Item Name="Dependencies" Type="Dependencies"/>
		<Item Name="Build Specifications" Type="Build">
			<Item Name="Motor Decoder" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{8E9FF785-2983-4A0F-A4CE-D61836750E3F}</Property>
				<Property Name="App_INI_GUID" Type="Str">{CE912EC2-2ABA-49EC-BFD5-4D7C9A97CD57}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="App_serverType" Type="Int">0</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{710B606D-ED71-4E68-B04A-FD587172162B}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Motor Decoder</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../labview builds/full builds</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{98EBAC1C-8B65-4D4A-AB5D-82F32ADCABBE}</Property>
				<Property Name="Bld_version.build" Type="Int">1</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Motor Decoder.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../labview builds/full builds/Motor Decoder.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../labview builds/full builds/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Source[0].itemID" Type="Str">{4080D593-B2F5-4E35-B26D-AD43AEDF7DB8}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/Motor Decoder.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Aarhus University</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Motor Decoder</Property>
				<Property Name="TgtF_internalName" Type="Str">Motor Decoder</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2022 Aarhus University</Property>
				<Property Name="TgtF_productName" Type="Str">Motor Decoder</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{C68951F9-4F16-4AD2-B7D3-3EA25EEDD41B}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Motor Decoder.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Status Decoder" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{3D0E8005-F224-4E8C-AEFD-538CEFF19F8B}</Property>
				<Property Name="App_INI_GUID" Type="Str">{D4D651E5-EED8-4EEB-AD28-D9BDEAD64183}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="App_serverType" Type="Int">0</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{825CC47C-5852-4665-B31F-58AE979E2D52}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Status Decoder</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../labview builds/full builds</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{31343BE2-8A61-439E-81FF-727592F594FA}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Status Decoder.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../labview builds/full builds/Status Decoder.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../labview builds/full builds/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Source[0].itemID" Type="Str">{314552C6-D5BA-49EB-BAAC-FE383FFDD14F}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/Status Decoder.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Aarhus University</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Status Decoder</Property>
				<Property Name="TgtF_internalName" Type="Str">Status Decoder</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2021 Aarhus University</Property>
				<Property Name="TgtF_productName" Type="Str">Status Decoder</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{5BC823F9-21DC-4B7E-9620-D12CB4B4896A}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Status Decoder.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
		</Item>
	</Item>
</Project>
