

import serial
import time as sleep
import mediapipe as mp
import cv2 as cv
import pyttsx3

# Engine setup for speech
engine = pyttsx3.init('sapi5')
voices = engine.getProperty('voices')
engine.setProperty('voice', voices[1].id)
engine.setProperty('rate', 170)

# Function to speak
def speak(text):
    engine.say(text)
    engine.runAndWait()

speak("Good Morning sir !")
speak("Sleep Detection System Initialized. Please ensure the Bluetooth device is connected.")

# Bluetooth Setup
BLUETOOTH_PORT = 'COM4'  # Replace with the correct port if needed
BAUD_RATE = 9999 # Standard baud rate for HC-05

try:
    bt_serial = serial.Serial(BLUETOOTH_PORT, BAUD_RATE, timeout=1)
    sleep.sleep(2)  # Allow time for the connection to initialize
    print(f"Bluetooth connected on {BLUETOOTH_PORT}.")
    speak("Bluetooth connection established successfully.")
except Exception as e:
    print(f"Bluetooth connection failed: {e}")
    speak("Bluetooth connection failed. Please check the device and try again.")
    bt_serial = None
    # We don't exit, to allow testing without bluetooth
    # exit()

# --- SLEEP DETECTION VARIABLES ---
SLEEP_THRESHOLD_SEC = 1.5 # 1.5 seconds to trigger sleep
is_sleeping = False       # Tracks the sleep state

# Setup OpenCV and Mediapipe
cam = cv.VideoCapture(0)
face_mesh = mp.solutions.face_mesh.FaceMesh(refine_landmarks=True)

prev_eye_state = "open"  # Tracks whether the eye was previously open
blink_start_time = None  # Tracks when an eye-close event started

while True:
    success, frame = cam.read()
    if not success:
        continue

    frame = cv.flip(frame, 1)
    frame_h, frame_w, _ = frame.shape
    rgb_frame = cv.cvtColor(frame, cv.COLOR_BGR2RGB)
    result = face_mesh.process(rgb_frame)

    # Define eye color based on sleep status
    eye_color = (0, 255, 0) # Green for awake
    if is_sleeping:
        eye_color = (0, 0, 255) # Red for sleeping

    if result.multi_face_landmarks:
        landmarks = result.multi_face_landmarks[0].landmark

        # --- SLEEP DETECTION ---
        # Get Y coordinates for top and bottom of both eyes
        r_top = landmarks[159].y * frame_h
        r_bottom = landmarks[145].y * frame_h
        r_height = abs(r_bottom - r_top)

        l_top = landmarks[386].y * frame_h
        l_bottom = landmarks[374].y * frame_h
        l_height = abs(l_bottom - l_top)

        # Calculate the average eye opening height
        avg_eye_height = (r_height + l_height) / 2.0
        
        # --- Eye Closed Logic ---
        # avg_eye_height < 8 is a threshold for closed eyes
        if avg_eye_height < 8:
            # If eyes were just closed, start the timer
            if prev_eye_state == "open":
                blink_start_time = sleep.time()
            
            prev_eye_state = "closed"

            # Check if eyes have been closed long enough for sleep
            if blink_start_time and (sleep.time() - blink_start_time) > SLEEP_THRESHOLD_SEC and not is_sleeping:
                is_sleeping = True
                speak("Sleep Alert! Wake up!")
                print("--- SLEEP DETECTED! ---")
                
                # Send '1' for SLEEP
                if bt_serial:
                    try:
                        bt_serial.write("1".encode())
                        print("Sent '1' (SLEEP) to Bluetooth.")
                    except Exception as e:
                        print(f"Bluetooth send error (sleep): {e}")

        # --- Eye Open Logic ---
        else:
            # Check if eyes were *just* opened
            if prev_eye_state == "closed":
                
                if is_sleeping:
                    # User was sleeping and just woke up
                    is_sleeping = False
                    speak("Awake")
                    print("--- AWAKE ---")
                    
                    # Send '0' for AWAKE
                    if bt_serial:
                        try:
                            bt_serial.write("0".encode())
                            print("Sent '0' (AWAKE) to Bluetooth.")
                        except Exception as e:
                            print(f"Bluetooth send error (awake): {e}")

            prev_eye_state = "open"
            blink_start_time = None # Reset timer

        # --- Iris Markers with Dynamic Color ---
        # Right eye iris
        r_iris_x = int(landmarks[468].x * frame_w)
        r_iris_y = int(landmarks[468].y * frame_h)
        cv.circle(frame, (r_iris_x, r_iris_y), 5, eye_color, -1) # Radius 5

        # Left eye iris
        l_iris_x = int(landmarks[473].x * frame_w)
        l_iris_y = int(landmarks[473].y * frame_h)
        cv.circle(frame, (l_iris_x, l_iris_y), 5, eye_color, -1) # Radius 5


        # --- Overlay Text ---
        # Display Sleep Status
        sleep_text = "AWAKE"
        status_color = (0, 255, 0) # Green
        if is_sleeping:
            sleep_text = "SLEEPING"
            status_color = (0, 0, 255) # Red
            
        cv.putText(frame, f"STATUS: {sleep_text}", (frame_w - 250, 50),
                   cv.FONT_HERSHEY_SIMPLEX, 1, status_color, 2)

    cv.imshow("Sleep Detection System", frame)

    # Exit if the ESC key is pressed
    if cv.waitKey(1) & 0xFF == 27:
        break

cam.release()
cv.destroyAllWindows()
if bt_serial:
    bt_serial.close()
print("Script terminated.")

