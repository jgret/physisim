if (Test-Path -Path .\raylib-master\) {

	$title    = 'Raylib is already present'
	$question = 'Reinstall?'
	$choices = New-Object Collections.ObjectModel.Collection[Management.Automation.Host.ChoiceDescription]
	$choices.Add((New-Object Management.Automation.Host.ChoiceDescription -ArgumentList '&Yes'))
	$choices.Add((New-Object Management.Automation.Host.ChoiceDescription -ArgumentList '&No'))
	$decision = $Host.UI.PromptForChoice($title, $question, $choices, 1)
	if ($decision -eq 0) {
		Remove-Item .\raylib-master\ -Recurse -Force -Confirm:$false
		.\setup.ps1
	}
}
else 
{
	"Downloading raylib-master.zip"
	$ProgressPreference = 'SilentlyContinue'
	Invoke-WebRequest -uri "https://github.com/raysan5/raylib/archive/refs/heads/master.zip" -Method "GET"  -Outfile "raylib-master.zip" 
	$ProgressPreference = 'SilentlyContinue'
	Expand-Archive raylib-master.zip -DestinationPath .
	"Deleting raylib-master.zip"
	rm raylib-master.zip	
	"Done"
	""
	"Thank You!"
	
}	

