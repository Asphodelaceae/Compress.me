import React from 'react';

function App() {
    const handleFileUpload = (action) => {
        const fileInput = document.createElement('input');
        fileInput.type = 'file';
        fileInput.onchange = (e) => {
            const file = e.target.files[0];
            if (file) {
                const formData = new FormData();
                formData.append('file', file);

                const url = action === 'compress' ? '/compress' : '/decompress';

                fetch(url, {
                    method: 'POST',
                    body: formData,
                })
                .then(response => response.text())
                .then(result => {
                    alert('File processing complete.');
                    console.log(result);
                })
                .catch(error => {
                    console.error('Error:', error);
                });
            }
        };
        fileInput.click();
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
                    <div className="feature">
                        <h3>User-Friendly</h3>
                        <p>Enjoy our easy-to-use interface, accessible to everyone.</p>
                    </div>
                    <div className="feature">
                        <h3>Secure Encryption</h3>
                        <p>Ensure your data's safety with military-grade encryption.</p>
                    </div>
                    <div className="feature">
                        <h3>Fast Compression</h3>
                        <p>Compress your files quickly and efficiently with our advanced algorithms.</p>
                    </div>
                </div>
            </section>
        </main>
    );
}

export default App;