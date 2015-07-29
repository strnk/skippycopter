class MessageProxyEntry:
    obj = None
    messages = []

    def __init__(self, obj, messages):
        self.obj = obj
        self.messages = messages

        newMessage = getattr(obj, "newMessage", None)
        if not callable(newMessage):
            raise Exception("Object must implement newMessage()")

class MessageProxy:
    serial = None
    objects = []
    messages = set([])

    def __init__(self, serial):
        self.serial = serial

    def addObject(self, obj, messages):
        proxyEntry = MessageProxyEntry(obj, messages)
        self.objects.append(proxyEntry)
        self.updateFlags()
        return proxyEntry

    def removeObject(self, obj):
        for entry in self.objects:
            if entry.obj == obj:
                self.objects.remove(entry)
                self.updateFlags()
                return

    def relayMessage(self, message, data):
        for entry in self.objects:
            if message in entry.messages or entry.messages[0] == '*':
                entry.obj.newMessage(message, data)

    def calculateNewMessageSet(self):
        msgSet = set()
        for entry in self.objects:
            msgSet |= set(entry.messages)

        return msgSet - set(['*'])

    def updateFlags(self):
        newSet = self.calculateNewMessageSet()

        toDisable = self.messages - newSet
        toEnable = newSet - self.messages

        for message in toDisable:
            self.serial.write("fd msg_%s\n" % message.lower())

        for message in toEnable:
            self.serial.write("fe msg_%s\n" % message.lower())

        self.messages = newSet