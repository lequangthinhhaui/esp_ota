# import shutil
# from fastapi import FastAPI, UploadFile, File

# app = FastAPI()


# @app.post("/")
# async def root(file: UploadFile = File(...)):
#     with open("test.ino", "wb") as buffer:
#         shutil.copyfileobj(file.file, buffer)
#     return{"file_name": file.filename}

# from fastapi import FastAPI, File, UploadFile

# app = FastAPI()


# @app.post("/files/")
# async def create_file(file: bytes = File()):
#     return {"file_size": len(file)}


# @app.post("/uploadfile/")
# async def create_upload_file(file: UploadFile):
#     refrom fastapi import FastAPI, File, Form, UploadFile

# app = FastAPI()


# @app.post("/files/")
# async def create_file(
#     file: bytes = File(), fileb: UploadFile = File(), token: str = Form()
# ):
#     return {
#         "file_size": len(file),
#         "token": token,
#         "fileb_content_type": fileb.content_type,
#     }turn {"filename": file.filename}

# 

# from fastapi import FastAPI

# app = FastAPI()


# @app.get("/")
# async def root():
#     return {"message": "Hello World"}


import shutil
from fastapi import FastAPI, File, UploadFile, Form
from fastapi.responses import HTMLResponse
from fastapi.responses import FileResponse

app = FastAPI()

@app.get("/")
def main():
    return FileResponse(path="files/factory_reset_02.bin", filename="factory_reset_02.bin", media_type='text/mp4')

@app.post("/upload-file/")
async def create_upload_file(uploaded_file: UploadFile = File(...)):    
    file_location = f"files/{uploaded_file.filename}"
    with open(file_location, "wb+") as file_object:
            shutil.copyfileobj(uploaded_file.file, file_object)    
    return {"info": f"file '{uploaded_file.filename}' saved at '{file_location}'"}


@app.post("/submit_form")
async def handle_form(files: UploadFile = File(...)):
    print('hello')
    file_location = f"files/{files.filename}"
    with open(file_location, "wb+") as file_object:
            shutil.copyfileobj(files.file, file_object)    
    return {"info": f"file '{files.filename}' saved at '{file_location}'"}



@app.get('/upload/form')
async def upload_form_file():
    return HTMLResponse(content="""
        <form method="post" action="/submit_form" enctype="multipart/form-data">
            <input name="files" type="file" multiple>
            <input type="submit">
        </form>""")
