import React, { useState } from 'react';

function Uploads() {
  const [files, setFiles] = useState([]);

  const handleFileChange = (event) => {
    setFiles(event.target.files);
  };

  const handleSubmit = async (event) => {
    event.preventDefault();
    const formData = new FormData();
    for (let file of files) {
      formData.append('files[]', file);
    }

    // Assuming you have an endpoint in your backend to handle the uploads
    try {
      const response = await fetch('/api/upload', {  // Adjust the API URL based on your backend setup
        method: 'POST',
        body: formData,
      });
      const result = await response.json();
      console.log(result);
      alert('Files uploaded successfully!');
    } catch (error) {
      console.error('Error:', error);
      alert('Error uploading files');
    }
  };

  return (
    <form onSubmit={handleSubmit}>
      <input type="file" onChange={handleFileChange} multiple />
      <button type="submit">Upload</button>
    </form>
  );
}

export default Uploads;
