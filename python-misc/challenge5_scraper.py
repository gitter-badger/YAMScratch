from Queue import Queue, Empty
from threading import Thread
import threading
import time

import urllib2
from bs4 import BeautifulSoup

GLOBAL_PAGE_VIEW_COUNT = 1
page_view_lock = threading.Lock()

def compute_location(path_str):
    #assume only valid chars in path str for efficiency
    X = 0
    Y = 0
    for char in path_str:
        if char == 'U':
            Y += 1
        elif char == 'D':
            Y -= 1
        elif char == 'R':
            X += 1
        elif char == 'L':
            X -= 1
    print (X,Y)
    return (X,Y)

def explore_path(work_queue, dead_end_queue, sol_queue, retry_queue):
    while True:
        try:
            #print "starting to wait"
            path_str = work_queue.get(True, 3)
        except Empty:
            break
        #test for loops a cheap way, if the length is greater than 75 then
        #put it in the retry queue
        if len(path_str) > 100:
            retry_queue.put(path_str)
            work_queue.task_done()
            continue

        base_path = 'http://0xd09eeffec7.dogemit.party'
        target_url = base_path + path_str
        req = urllib2.Request(target_url)
        try:
            page_response = urllib2.urlopen(req)
        except urllib2.HTTPError as e:
            print "HTTP errro"
            #bad request
            work_queue.task_done()
            continue
        except urllib2.URLError as e:
            #we could not reach the server, so push
            #the thing back onto the queue so that
            # we try again later
            print "Error: " + path_str
            work_queue.put(path_str)
            work_queue.task_done()
            continue

        if ((page_response.getcode() != 200) and (page_response.getcode() != 201)):
            print "error, code =", page_response.getcode()
            #push this into the retry queue
            retry_queue.put(path_str)

        #compute the last move to prevent backtracking
        previous_move = path_str[-1]
        page_data = page_response.read()
        soup = BeautifulSoup(page_data, 'lxml')

        for row_data in soup.body.find_all(class_ = 'row'):
            #find each href
            for link in row_data.find_all('a', href = True):
                #detect if the link is special
                if link.has_attr('class'):
                    sol_queue.put(link['href'])
                    continue
                #below is a gamble since it expects the formatting to be 
                # rigidly followed
                possible_next = link['href']
                next_move = possible_next[-1]
                #check that we are not backtracking
                if next_move == 'U':
                    if previous_move == 'D':
                        #this will backtrack
                        #so dont put it in queue
                        #print "not backtracking"
                        continue
                elif next_move == 'D':
                    if previous_move == 'U':
                        continue
                elif next_move == 'L':
                    if previous_move == 'R':
                        continue
                elif next_move == 'R':
                    if previous_move == 'L':
                        continue
                #if we are still here, push the string onto the stack
                print "putting:", possible_next
                global GLOBAL_PAGE_VIEW_COUNT
                with page_view_lock:
                    tmp_count = GLOBAL_PAGE_VIEW_COUNT
                    GLOBAL_PAGE_VIEW_COUNT += 1

                if tmp_count > 1000:
                    #put the page into retry
                    retry_queue.put(possible_next)
                    return
                else: 
                    work_queue.put(possible_next)

        #at very end
        work_queue.task_done()



paths = Queue(maxsize = 0)
dead_ends = Queue(maxsize = 0)
sol_queue = Queue(maxsize = 0)
retry_queue = Queue(maxsize = 0)

paths.put('/U')

explore_path(paths, dead_ends, sol_queue, retry_queue)

num_threads = 8

for ii in range(num_threads):
  worker = Thread(target= explore_path, args = (paths, dead_ends, sol_queue, retry_queue))
  worker.setDaemon(True)
  worker.start()



#at very end, print out every thing in the solution queue
print '=========Solutions============='
while True:
    try:
        print sol_queue.get(False)
    except Empty:
        print "\tempty"
        break

print '=========Dead Ends============='
while True:
    try:
        print dead_ends.get(False)
    except Empty:
        print "\tempty"
        break

print '=========Retry Later============='
while True:
    try:
        print retry_queue.get(False)
    except Empty:
        print "\tempty"
        break

print '==========Try again=========='
#at very end, print out every thing in the retry queue
while True:
    try:
        print retry_queue.get(False)
    except Empty:
        print "\tempty"
        break
