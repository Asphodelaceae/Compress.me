import React, { useState } from 'react';
import axios from 'axios';

interface UploadFormProps {
  action: 'compress' | 'decompress'; // Prop to specify the action
}

const UploadForm: React.FC<UploadFormProps> = ({ action }) => {
  const [file, setFile] = useState<File | null>(null);
  const [result, setResult] = useState<string | null>(null);

  const handleFileChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    if (e.target.files) {
      setFile(e.target.files[0]);
    }
  };

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    if (file) {
      const formData = new FormData();
      formData.append('file', file);

      // API endpoint changes based on the action
      const url = action === 'compress' ? 'http://compress.me/upload/encrypt' : 'http://compress.me/upload/decrypt';

      try {
        const response = await axios.post(url, formData);
        setResult(response.data.result);
      } catch (error) {
        console.error(`Error ${action}ing file:`, error);
      }
    }
  };

  return (
    <div>
      <form onSubmit={handleSubmit}>
        <input type="file" onChange={handleFileChange} />
        <button type="submit">{action === 'compress' ? 'Compress File' : 'Decompress File'}</button>
      </form>
      {result && <div>{result}</div>}
    </div>
  );
};

export default UploadForm;
