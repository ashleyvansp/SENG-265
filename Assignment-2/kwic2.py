import sys
import string

def main():
    storage=[]
    keys=[]
    phrases=[]
    idx_words=[]
    init_arrays(storage,keys,phrases,idx_words)
    print_idx(phrases, idx_words)


# Initializes the arrays as follows:
#	1) storage: contains every line of the input file
#	2) keys: stores the exclusion words
#	3) phrases: stores the lines-for-indexing
#	4) words: contains the indexing words (words that aren't keys)
def init_arrays(storage, keys, phrases, idx_words):
    storage[:] = [line.strip() for line in sys.stdin.readlines()[2:] if line.strip() != ""]
    keys[:] = [key.lower() for key in storage[:storage.index("::")]]
    phrases[:] = storage[storage.index("::")+1:]
    temp=[]
    for phrase in phrases:
        temp+= [word.lower() for word in phrase.split() if not(word in keys or word.lower() in keys)]  
    idx_words[:] = list(set(sorted(temp)))
    idx_words[:] = [word.lower() for word in idx_words]
    idx_words.sort()

# Goes through each key word in context in alphabetical order
# Prints any phrases containing the current key word in context
def print_idx(phrases,idx_words):
    for idx_word in idx_words:
        for phrase in phrases:
            if idx_word in [word.lower() for word in phrase.split()]:
                print_phrase(phrase, idx_word)

# Capitalizes the key word in context in its sentence
# Prints the phrase in accordance with the formatting specified in the assignment description                
def print_phrase(phrase, idx_word):
    index = [word.lower() for word in phrase.split()].index(idx_word)
    new_phrase = [line.strip() for line in phrase.split()]
    new_phrase[index] = new_phrase[index].upper()
    start = ''
    for word in reversed(new_phrase[:index]):
        if (len(word) + len(start)) < 20:
            start += word + " "
        else:
            break
    start = (" ").join(reversed(start.split()))
    end = ''
    for word in new_phrase[index+1:]:
        if (len(word) + len(end)) <= (30-len(idx_word)):
            end += " " +word
        else:
            break
    space = " "
    length = len(idx_word)
    if len(start) == 0:     
        print("{:>28}" .format(space*29), end='')
        print(new_phrase[index] + end[:31-length])
    else:
        print("{:>28} " .format(start), end = "")
        print(new_phrase[index] + end[:31-length])
   
      
if __name__ == "__main__":
    main()