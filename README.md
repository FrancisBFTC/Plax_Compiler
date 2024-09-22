# Compilador Plax v0.2.8

Plax (**P**redefined **L**anguage of **A**utomated e**X**ecution) é uma linguagem de programação compilada, multi-paradigma e dinâmica no intuito de ser flexível como JavaScript/Python e veloz como C/Assembly, a linguagem contém junções de paradigmas e diferentes sintaxes.

<img src="https://imgur.com/huBWl6V.png">

# ✔️ Requisitos do Projeto
  
**Para compilar o projeto de compilador PLAX:**
 
 • Possuir o compilador **G++**<br>
 • Configurar no C++ a API **nlohmann/json** (Busque em um dos meus Forks)
    
**Requisitos Gerais:**
      
 • Possuir máquina Windows x86 ou x64
    
# 💻 Instalação

**Clone o repositório em sua máquina Windows (Por exemplo: No Disco Local C:):**
```bat
git clone https://github.com/FrancisBFTC/Plax_Compiler.git
```

**Para adicionar as variáveis de ambiente, abra o CMD como Administrador e execute:**
```bat
setx PATH "%PATH%;C:\Plax_Compiler\PlaxLang;"
```

**Para compilar o projeto de compilador em C++, execute no CMD (ou clique 2 vezes em build.bat):**
```bat
cd C:\Plax_Compiler\PlaxLang
build.bat
```

# 📜 Como Compilar Código em Plax

**Crie um arquivo com extensão .plax (Exemplo: prog.plax), crie algoritmos PLAX** vendo <a href="#tutorial">Tutorial da Linguagem</a> **e execute:**
```bat
plax prog.plax prog.exe
```

<a name="tutorial"></a>
# Tutorial da Linguagem

  ## Crie seu primeiro Hello World
  
  ```cs
    use * of <io_sys>
    
    Show['Hello World!!']
  ```




