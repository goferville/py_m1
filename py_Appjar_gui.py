# download appJar.zip from official site, unzip->project directory
# import the library
# appJar is the directory name
from appJar import gui

# handle button events
def press(button):
    if button == "Cancel":
        app.stop()
    elif button == "Koala Kick":
        print("Kick u!")
    else:
        usr = app.getEntry("Username")
        pwd = app.getEntry("Password")
        print("User:", usr, "Pass:", pwd)

# create a GUI variable called app
app = gui("Login Window", "400x200")
app.setBg("orange")
app.setFont(18)

# add & configure widgets - widgets get a name, to help referencing them later
app.addLabel("title", "Welcome to appJar")
app.setLabelBg("title", "blue")
app.setLabelFg("title", "orange")

app.addLabelEntry("Username")
app.addLabelSecretEntry("Password")

# link the buttons to the function called press
app.addButtons(["Submit", "Cancel", "Koala Kick"], press)
app.setFocus("Username")

# start the GUI
app.go()