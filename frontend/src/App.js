import React, { useState } from 'react';
import './App.css';
import UploadForm from './UploadForm.tsx';

function App() {
  const [expanded, setExpanded] = useState(null);
  const [showUploadForm, setShowUploadForm] = useState(false);  // State to toggle the UploadForm visibility

  const toggleExpand = (featureId) => {
    setExpanded(expanded === featureId ? null : featureId);
  };

  return (
    <>
      <header id="home">
        <h1>Welcome to Compress.me!</h1>
        <p>Your one-stop solution for file compression and encryption.</p>
      </header>
      <main>
        <div className="button-container">
          <button className="upload-button" onClick={() => setShowUploadForm(true)}>Compress</button>
          <button className="upload-button" onClick={() => alert('Decompress functionality here')}>Decompress</button>
        </div>
        {showUploadForm && (
          <UploadForm />
        )}
        <section id="features" className="feature-section">
          <h2>Features</h2>
          <div className="features">
            {['userFriendly', 'secureEncryption', 'fastCompression'].map(feature => (
              <div key={feature} className={`feature ${expanded === feature ? 'expanded' : ''}`} onClick={() => toggleExpand(feature)}>
                <h3>{feature}</h3>
                <p>{`Details about ${feature}. Click to expand for more info.`}</p>
                {expanded === feature && <div className="detailed-content">More details about {feature}...</div>}
              </div>
            ))}
          </div>
        </section>
      </main>
      <footer>
        <p>© 2024 Compress.me - All rights reserved</p>
      </footer>
    </>
  );
}

export default App;
