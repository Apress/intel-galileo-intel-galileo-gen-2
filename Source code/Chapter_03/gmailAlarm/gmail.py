# based in the discussion in:
# http://stackoverflow.com/questions/953561/check-unread-count-of-gmail-messages-with-python
# by manoel.c.ramon@intel.com

import imaplib
import sys

obj = imaplib.IMAP4_SSL('imap.gmail.com','993')
obj.login(sys.argv[1],sys.argv[2])
obj.select()
obj.search(None,'UnSeen')
print len(obj.search(None,'UnSeen')[1][0].split())
