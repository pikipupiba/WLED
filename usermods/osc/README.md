The Plan:
1. Recieve UDP Packet
2. Look for a "#", and if found
    a. Read digits at end
    b. Parse animation parameter at that position
    c. Set animation parameter to value
3. Look for a "#", and if NOT found
    a. interpret as a single input
    b. Parse animation parameter
    c. Set animation parameter to value