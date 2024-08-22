import React from 'react';
import FileUpload from './FileUpload';
import './App.css';  // Custom styles

function App() {
  return (
    <div className="App">
      <header className="App-header">
        <h1>File Upload Service</h1>
      </header>
      <main>
        <FileUpload />
      </main>
      <footer>
        <p>© 2024 Your Company</p>
      </footer>
    </div>
  );
}

export default App;
