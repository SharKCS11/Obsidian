import cv2
import sys
import time

cascPath = sys.argv[1]
faceCascade = cv2.CascadeClassifier(cascPath)

video_capture = cv2.VideoCapture(1)

while True:
    # Capture frame-by-frame
    ret, frame = video_capture.read()

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    faces = faceCascade.detectMultiScale(
        gray,
        scaleFactor=1.1,
        minNeighbors=5,
        minSize=(30, 30)
    )

    if not len(faces) == 0:
        # find and crop largest face
        max_face = faces[0]
        max_area = faces[0][2] * faces[0][3]

        for (x, y, w, h) in faces:
            area = w * h
            max_face = (x, y, w, h) if area > max_area else max_face
            max_area = area if area > max_area else max_area
            cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)

        crop_img = frame[max_face[1]:max_face[1] + max_face[3], max_face[0]:max_face[0] + max_face[2]]
        cv2.imshow('cropped', crop_img)

    cv2.imshow('video', frame)

    # break on esc
    if cv2.waitKey(1) & 0xFF == 27:
        break

    time.sleep(0.5)

video_capture.release()
cv2.destroyAllWindows()
