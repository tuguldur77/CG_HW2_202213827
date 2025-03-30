# 🎨 Ray Tracer - Computer Graphics Assignment 2  

## 🎯 Objective  
This project extends the ray tracer from Assignment 1 by adding **shadows, gamma correction, and antialiasing** for more realistic rendering.  

### 🛠 Implemented Features  
1️⃣ **Shadows** 🌑  
   - Added shadow rays to check if objects block the light source.  
   - If a point is in shadow, only ambient lighting is applied.  

2️⃣ **Gamma Correction** 🎛  
   - Applied **gamma correction (γ = 2.2)** to enhance brightness and color accuracy.  

3️⃣ **Antialiasing** 🎨  
   - Reduced jagged edges using **supersampling antialiasing**.  
   - Generated **64 random samples per pixel** and applied a **box filter** for smoother rendering.  

---

## 🚀 Features  
✔ **Ray Intersection** (Spheres & Plane)  
✔ **Phong Shading** for realistic lighting  
✔ **Shadows** for better depth perception  
✔ **Gamma Correction** for perceptual brightness  
✔ **Antialiasing** (64 samples per pixel)  
✔ **Rendering using OpenGL & GLFW**  

---

## 📦 Installation & Compilation  

### 🔹 Prerequisites  
✅ Visual Studio 2022  
✅ C++ Compiler with OpenGL Support  
✅ Git (for cloning the repository)  

_No need to install OpenGL, GLFW, or GLM separately! They are included in the project folder._  

### 🔹 How to Compile & Run  

1️⃣ **Clone the Repository**  
```bash
git clone https://github.com/tuguldur77/CG_HW2_202213827.git
```
Open "CG_HW2_202213827" folder

Load the solution file (.sln)

2️⃣ **Open in Visual Studio**

Launch Visual Studio 2022

Load the solution file (.sln)

3️⃣ **Set the Active Project**

The project contains 3 parts (HW2Q1, HW2Q2, HW2Q3).

To run a specific part:

  1. Right-click on the project (HW2Q1, HW2Q2, or HW2Q3).

  2. Select "Set as Startup Project".

  3. Click Start (F5) to run.

4️⃣ **Build & Run**

Click Build > Build Solution (Ctrl + Shift + B)

Click Start (F5) to run</br></br>

📢 Notice: Q3. Antialiasing may take ~5 seconds to render because it traces 64 rays per pixel for smoother edges.</br></br>

📸 Screenshots:</br>	
Q1. Phong Shading output:</br>	
![output_Q1_202213827_Tuguldur](https://github.com/user-attachments/assets/3eb77f9f-e2eb-4f8a-bec0-d31e342db641)

Q2. Gamma Correction output:</br>	
![output_Q2_202213827_Tuguldur](https://github.com/user-attachments/assets/e917b7a5-7388-4bc6-a458-e3a0350e9e4b)

Q3. Antialiasing output:</br>
![output_Q3_202213827_Tuguldur](https://github.com/user-attachments/assets/ed95136b-310e-4a7e-aab3-814828b15159)</br></br>

📝 References</br>
📌 Class Notes – Used for implementing shadow calculations and antialiasing.</br>
🤖 GitHub Copilot – Helped optimize matrix transformations and debug errors.</br>
👨‍💻 chatGPT - Used for compute Phong shading.</br>

👨‍💻 Author
GitHub: [Tuguldur77](https://github.com/tuguldur77)
