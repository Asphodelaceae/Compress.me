import React, { useState } from 'react';

function App() {
    const [expanded, setExpanded] = useState(null);  // This state will track the currently expanded feature

    const handleFileUpload = (action) => {
        // File upload functionality remains unchanged
    };

    const toggleExpand = (featureId) => {
        setExpanded(expanded === featureId ? null : featureId);  // Toggles the expanded feature
    };

    return (
        <main>
            <div className="button-container">
                <button className="upload-button" onClick={() => handleFileUpload('compress')}>Compress</button>
                <button className="upload-button" onClick={() => handleFileUpload('decompress')}>Decompress</button>
            </div>
            <section id="features" className="feature-section">
                <h2>Features</h2>
                <div className="features">
                    <div className={`feature ${expanded === 'userFriendly' ? 'expanded' : ''}`} onClick={() => toggleExpand('userFriendly')}>
                        <h3>User-Friendly</h3>
                        <p>Enjoy our easy-to-use interface, accessible to everyone.</p>
                        {expanded === 'userFriendly' && <div className="detailed-content">More in-depth information about user-friendly features...</div>}
                    </div>
                    <div className={`feature ${expanded === 'secureEncryption' ? 'expanded' : ''}`} onClick={() => toggleExpand('secureEncryption')}>
                        <h3>Secure Encryption</h3>
                        <p>Ensure your data's safety with military-grade encryption.</p>
                        {expanded === 'secureEncryption' && <div className="detailed-content">Detailed information about our encryption techniques...</div>}
                    </div>
                    <div className={`feature ${expanded === 'fastCompression' ? 'expanded' : ''}`} onClick={() => toggleExpand('fastCompression')}>
                        <h3>Fast Compression</h3>
                        <p>Compress your files quickly and efficiently with our advanced algorithms.</p>
                        {expanded === 'fastCompression' && <div className="detailed-content">Additional details about compression methods...</div>}
                    </div>
                </div>
            </section>
        </main>
    );
}

export default App;
