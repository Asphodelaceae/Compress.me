from django.shortcuts import render
from django.http import JsonResponse
import subprocess
import os
from .forms import UploadFileForm
from django.views.decorators.csrf import csrf_exempt

@csrf_exempt
def upload_file_encrypt(request):
    if request.method == 'POST':
        form = UploadFileForm(request.POST, request.FILES)
        if not form.is_valid():
            print('Invalid form: ', form.errors)
            return JsonResponse({'error': 'Invalid request'}, status=400)
        
        file = request.FILES['file']
        upload_dir = 'media'
        if not os.path.exists(upload_dir):
            os.makedirs(upload_dir)
        file_path = os.path.join(upload_dir, file.name)
        encrypted_file_out_path = f'{file_path}_encrypted.txt'
        intermediary_file_path = f'{file_path}_cat.txt'
        final_output_path = f'{file_path}_encoded.txt'
        '''
        First, generate key pair using the keygen binary
        Then, encrypt the uploaded file using encrypt binary w/ public key
        Then, cat the encrypted file and the private key to generate the intermediary file. 
        This needs to be done in such a way that they can be de-concatenated later once they are
        ran through huffman decoding, so that the encrypted file can be decrypted using the private key

        Then, compress the encrypted file with Huffman encode binary, and output the final result
        '''

        # Step 1: Generate Key Pair
        keygen_result = subprocess.run(['/usr/bin/keygen'], capture_output=True, text=True)
        if keygen_result.returncode != 0:
            return JsonResponse({'error': 'Key generation failed'}, status=500)

        # Assuming keygen outputs two files: public.key and private.key
        # TODO: figure out where the pub and priv keys will actually go so we can find and pull them. 
        # Step 2: Encrypt the File
        # TODO: Add args for encrypting the file
        encrypt_args = ['/usr/bin/encrypt', 
                        '-i', file_path,
                        '-o', encrypted_file_out_path]
        encrypt_result = subprocess.run(encrypt_args, capture_output=True, text=True)
        if encrypt_result.returncode != 0:
            return JsonResponse({'error': 'File encryption failed'}, status=500)

        # Step 3: Concatenate Encrypted File and Private Key
        # TODO: make sure that this cats the files correctly and in a way that can be de-concat'd later
        with open(intermediary_file_path, 'wb') as intermediary_file:
            # Concatenate encrypted file
            with open(encrypted_file_out_path, 'rb') as encrypted_file:
                intermediary_file.write(encrypted_file.read())
            intermediary_file.write(b'--END--ENCRYPTED--FILE--\n')
            # Concatenate private key
            with open('rsa.priv', 'rb') as private_key_file:
                intermediary_file.write(private_key_file.read())

        # TODO: figure out args for to encode the cat'd file above. Will need to pass its path to the encode binary
        # Step 4: Compress the Intermediary File
        huffman_args = ['/usr/bin/encode',
                        '-i', intermediary_file_path,
                        '-o', final_output_path]
        huffman_result = subprocess.run(huffman_args, capture_output=True, text=True)
        if huffman_result.returncode != 0:
            return JsonResponse({'error': 'Huffman encoding failed'}, status=500)

        # Return the path to the final output file
        return JsonResponse({'result': final_output_path})


@csrf_exempt
def upload_file_decrypt(request):
    if request.method == 'POST':
        form = UploadFileForm(request.POST, request.FILES)
        if not form.is_valid():
            print('Invalid form: ', form.errors)
            return JsonResponse({'error': 'Invalid request'}, status=400)
        
        # Step 1: Handle the uploaded file
        file = request.FILES['file']
        upload_dir = 'media'
        if not os.path.exists(upload_dir):
            os.makedirs(upload_dir)
        
        # Define file paths
        file_path = os.path.join(upload_dir, file.name)
        intermediary_file_path = f'{file_path}_cat.txt'  # Output of decompression
        encoded_file_path = f'{file_path}_encoded.txt'  # Input file for decoding
        decrypted_file_path = f'{file_path}_decrypted.txt'  # Final output after decryption
        private_key_path = f'{file_path}_private_key.txt'  # Extracted private key

        # Save the uploaded file (encoded Huffman file)
        with open(encoded_file_path, 'wb+') as destination:
            for chunk in file.chunks():
                destination.write(chunk)

        # Step 2: Decode the Encoded File
        decode_args = ['/usr/bin/decode', 
                       '-i', encoded_file_path,
                       '-o', intermediary_file_path]
        decode_result = subprocess.run(decode_args, capture_output=True, text=True)
        if decode_result.returncode != 0:
            return JsonResponse({'error': 'Huffman decoding failed'}, status=500)

        # Step 3: De-concatenate Encrypted File and Private Key
        # Read the intermediary file and split based on the delimiter
        try:
            with open(intermediary_file_path, 'rb') as f:
                content = f.read()
                encrypted_content, private_key_content = content.split(b'--END--ENCRYPTED--FILE--\n', 1)

            # Write the encrypted content to a file
            with open(f'{file_path}_encrypted.txt', 'wb') as encrypted_file:
                encrypted_file.write(encrypted_content)
            
            # Write the private key content to a file
            with open(private_key_path, 'wb') as private_key_file:
                private_key_file.write(private_key_content)
        except Exception as e:
            print('De-concatenation failed: ', e)
            return JsonResponse({'error': 'De-concatenation failed'}, status=500)

        # Step 4: Decrypt the File using the extracted private key
        decrypt_args = ['/usr/bin/decrypt', 
                        '-k', private_key_path, 
                        '-i', f'{file_path}_encrypted.txt',
                        '-o', decrypted_file_path]
        decrypt_result = subprocess.run(decrypt_args, capture_output=True, text=True)
        if decrypt_result.returncode != 0:
            return JsonResponse({'error': 'File decryption failed'}, status=500)

        # Return the path to the decrypted file
        return JsonResponse({'result': decrypted_file_path})

    return JsonResponse({'error': 'Invalid request method'}, status=400)