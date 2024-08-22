// Inside your appComp.jsx or a similar file
import React from 'react';
import './style.css'; // Make sure the path is correct based on your project structure

function AppComp() {
  return (
    <div className="App">
      <header>
        <h1>Welcome to Compress.me!</h1>
      </header>
      <main>
        <button onClick={() => alert("Button clicked!")}>Click me!</button>
        <button onClick={() => alert("Another action!")}>Another Button</button>
      </main>
      <footer>
        <p>© 2024 Compress.me - All rights reserved</p>
      </footer>
    </div>
  );
}

export default AppComp;
