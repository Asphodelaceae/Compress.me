from django.shortcuts import render
from django.http import JsonResponse
import subprocess
import os
from .forms import UploadFileForm

def upload_file(request):
    if request.method == 'POST':
        form = UploadFileForm(request.POST, request.FILES)
        if form.is_valid():
            file = request.FILES['file']
            file_path = os.path.join('media', file.name)
            
            # Save the uploaded file
            with open(file_path, 'wb+') as destination:
                for chunk in file.chunks():
                    destination.write(chunk)

            '''
            First, generate key pair using the keygen binary
            Then, encrypt the uploaded file using encrypt binary w/ public key
            Then, cat the encrypted file and the private key to generate the intermediary file. 
            This needs to be done in such a way that they can be de-concatenated later once they are
            ran through huffman decoding, so that the encrypted file can be decrypted using the private key

            Then, compress the encrypted file with Huffman encode binary, and output the final result
            '''

            # Step 1: Generate Key Pair
            # TODO: put args for keygen in the rest of the array below
            keygen_result = subprocess.run(['../../../encryption/binaries/keygen', "???"], capture_output=True, text=True)
            if keygen_result.returncode != 0:
                return JsonResponse({'error': 'Key generation failed'}, status=500)

            # Assuming keygen outputs two files: public.key and private.key
            # TODO: figure out where the pub and priv keys will actually go so we can find and pull them. 
            public_key_path = 'public.key'
            private_key_path = 'private.key'

            # Step 2: Encrypt the File
            # TODO: Add args for encrypting the file
            encrypted_file_path = f'{file_path}.txt'
            encrypt_result = subprocess.run(['../../../encryption/binaries/encrypt', '???'], capture_output=True, text=True)
            if encrypt_result.returncode != 0:
                return JsonResponse({'error': 'File encryption failed'}, status=500)

            # Step 3: Concatenate Encrypted File and Private Key
            # TODO: make sure that this cats the files correctly and in a way that can be de-concat'd later
            intermediary_file_path = f'{file_path}.txt'
            with open(intermediary_file_path, 'wb') as intermediary_file:
                # Concatenate encrypted file
                with open(encrypted_file_path, 'rb') as encrypted_file:
                    intermediary_file.write(encrypted_file.read())
                # Concatenate private key
                with open(private_key_path, 'rb') as private_key_file:
                    intermediary_file.write(private_key_file.read())

            # TODO: figure out args for to encode the cat'd file above. Will need to pass its path to the encode binary
            # Step 4: Compress the Intermediary File
            final_output_path = f'{file_path}.txt'
            huffman_result = subprocess.run(['../../../compression/binaries/encode', '???'], capture_output=True, text=True)
            if huffman_result.returncode != 0:
                return JsonResponse({'error': 'Huffman encoding failed'}, status=500)

            # Return the path to the final output file
            return JsonResponse({'result': final_output_path})

    return JsonResponse({'error': 'Invalid request'}, status=400)
