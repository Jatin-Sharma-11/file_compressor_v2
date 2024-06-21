import os, sys
import glob
from flask import Flask, render_template, request, send_file
app = Flask(__name__)

global filename
global ftype

@app.route("/")
def home():
    # Delete old files
    filelist = glob.glob('uploads/*')
    for f in filelist:
        os.remove(f)
    filelist = glob.glob('downloads/*')
    for f in filelist:
        os.remove(f)
    return render_template("premain.html")

# Set the file uploads directory (relative path)
app.config["FILE_UPLOADS"] = os.path.join("uploads")

@app.route("/hcd")
def hcd():
    return render_template("hcd.html")

@app.route("/lzwcd")
def lzwcd():
    return render_template("lzwcd.html")

@app.route("/lzwabout")
def lzwabout():
    return render_template("lzwabout.html")




@app.route("/hcompress", methods=["GET", "POST"])
def hcompress():
    if request.method == "GET":
        return render_template("hcompress.html", check=0)

    else:
        up_file = request.files["file"]

        if len(up_file.filename) > 0:
            global filename
            global ftype
            filename = up_file.filename
            print(up_file.filename)
            up_file.save(os.path.join(app.config["FILE_UPLOADS"], filename))
            os.system('c.exe uploads\\{}'.format(filename))
            filename = filename[:filename.index(".",1)]
            ftype = "-compressed.bin"
            while True:
                if os.path.exists(os.path.join('uploads', '{}-compressed.bin'.format(filename))):
                    os.system('move uploads\\{}-compressed.bin downloads\\'.format(filename))
                    break

            return render_template("hcompress.html", check=1)

        else:
            print("ERROR")
            return render_template("hcompress.html", check=-1)
        


@app.route("/hdecompress", methods=["GET", "POST"])
def hdecompress():
    if request.method == "GET":
        return render_template("hdecompress.html", check=0)

    else:
        up_file = request.files["file"]

        if len(up_file.filename) > 0:
            global filename
            global ftype
            filename = up_file.filename
            print(up_file.filename)
            up_file.save(os.path.join(app.config["FILE_UPLOADS"], filename))
            os.system('d.exe uploads\\{}'.format(filename))
            f = open(os.path.join('uploads', filename), 'rb')
            ftype = "-decompressed." + (f.read(int(f.read(1)))).decode("utf-8")
            filename = filename[:filename.rfind("-")]

            print(filename)

            while True:
                if os.path.exists(os.path.join('uploads', '{}'.format(filename)+'{}'.format(ftype))):
                    os.system('move uploads\\'+'{}'.format(filename)+'{}'.format(ftype)+' downloads\\')
                    break
                # if os.path.exists(os.path.join('uploads', '{}-decompressed.txt'.format(filename))):
                #     os.system('move uploads\\{}-decompressed.txt downloads\\'.format(filename))
                #     break

            return render_template("hdecompress.html", check=1)

        else:
            print("ERROR")
            return render_template("hdecompress.html", check=-1)
        


@app.route("/lzwcompress", methods=["GET", "POST"])
def lzwcompress():
    if request.method == "GET":
        return render_template("lzwcompress.html", check=0)

    else:
        up_file = request.files["file"]

        if len(up_file.filename) > 0:
            global filename
            global ftype
            filename = up_file.filename
            print(up_file.filename)
            up_file.save(os.path.join(app.config["FILE_UPLOADS"], filename))
            os.system('lzwCompression.exe uploads\\{}'.format(filename))
            filename = filename[:filename.index(".",1)]
            ftype = "-compressed.bin"
            while True:
                if os.path.exists(os.path.join('uploads', '{}-compressed.bin'.format(filename))):
                    os.system('move uploads\\{}-compressed.bin downloads\\'.format(filename))
                    break

            return render_template("lzwcompress.html", check=1)

        else:
            print("ERROR")
            return render_template("lzwcompress.html", check=-1)
        


@app.route("/lzwdecompress", methods=["GET", "POST"])
def lzwdecompress():
    if request.method == "GET":
        return render_template("lzwdecompress.html", check=0)

    else:
        up_file = request.files["file"]

        if len(up_file.filename) > 0:
            global filename
            global ftype
            filename = up_file.filename
            print(up_file.filename)
            up_file.save(os.path.join(app.config["FILE_UPLOADS"], filename))
            os.system('lzwDecompress.exe uploads\\{}'.format(filename))
            f = open(os.path.join('uploads', filename), 'rb')
            ftype = "-decompressed." + (f.read(int(f.read(1)))).decode("utf-8")
            filename = filename[:filename.rfind("-")]

            print(filename)

            while True:
                if os.path.exists(os.path.join('uploads', '{}'.format(filename)+'{}'.format(ftype))):
                    os.system('move uploads\\'+'{}'.format(filename)+'{}'.format(ftype)+' downloads\\')
                    break

            return render_template("lzwdecompress.html", check=1)

        else:
            print("ERROR")
            return render_template("lzwdecompress.html", check=-1)
        




@app.route("/download")
def download_file():
    global filename
    global ftype
    path = os.path.join("downloads", filename + ftype)
    return send_file(path, as_attachment=True)


if __name__ == "__main__":
    app.run(host='0.0.0.0', port=5000)