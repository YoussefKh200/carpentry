import argparse
import json
import os
import sys
import time

import cv2


def load_labels(labels_path):
    mapping = {}
    with open(labels_path, "r", encoding="utf-8", errors="ignore") as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            parts = line.split(" ", 1)
            if len(parts) != 2:
                continue
            try:
                mapping[int(parts[0])] = parts[1].strip()
            except ValueError:
                pass
    return mapping


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--repo", required=True)
    parser.add_argument("--threshold", type=float, default=65.0)
    parser.add_argument("--timeout", type=int, default=10)
    args = parser.parse_args()

    repo = args.repo
    cascade_path = os.path.join(repo, "cascades", "haarcascade_frontalface_default.xml")
    model_path = os.path.join(repo, "recognizer", "embeddings.xml")
    labels_path = os.path.join(repo, "recognizer", "labels.txt")

    if not os.path.exists(cascade_path):
        print(json.dumps({"ok": False, "error": "Cascade introuvable"}))
        return
    if not os.path.exists(model_path):
        print(json.dumps({"ok": False, "error": "Modele introuvable (embeddings.xml)"}))
        return
    if not os.path.exists(labels_path):
        print(json.dumps({"ok": False, "error": "labels.txt introuvable"}))
        return
    if not hasattr(cv2, "face"):
        print(json.dumps({"ok": False, "error": "cv2.face indisponible (installer opencv-contrib-python)"}))
        return

    labels = load_labels(labels_path)
    detector = cv2.CascadeClassifier(cascade_path)
    # Match the original project training/inference parameters.
    recognizer = cv2.face.LBPHFaceRecognizer_create(1, 10, 8, 8, 100.0)
    recognizer.read(model_path)

    cap = cv2.VideoCapture(0)
    if not cap.isOpened():
        print(json.dumps({"ok": False, "error": "Camera indisponible"}))
        return

    start = time.time()
    best_name = ""
    best_conf = 9999.0

    while time.time() - start < args.timeout:
        ok, frame = cap.read()
        if not ok:
            continue
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        faces = detector.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=6, minSize=(80, 80))
        for (x, y, w, h) in faces:
            roi = gray[y:y + h, x:x + w]
            roi = cv2.resize(roi, (100, 100))
            label, conf = recognizer.predict(roi)
            if conf < best_conf:
                best_conf = conf
                best_name = labels.get(label, "")
        # allow closing camera window with q if shown externally by some backends
        if cv2.waitKey(1) & 0xFF == ord("q"):
            break

    cap.release()
    cv2.destroyAllWindows()

    if best_name and best_conf <= args.threshold:
        print(json.dumps({"ok": True, "name": best_name, "confidence": float(best_conf)}))
    else:
        debug_error = "Visage non reconnu (best_name='{}', confidence={:.2f}, seuil={:.2f})".format(
            best_name, float(best_conf), float(args.threshold)
        )
        print(json.dumps({"ok": False, "error": debug_error, "confidence": float(best_conf)}))


if __name__ == "__main__":
    main()
