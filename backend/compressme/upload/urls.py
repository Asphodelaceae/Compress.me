from django.urls import path
from . import views

urlpatterns = [
    path('encrypt', views.upload_file_encrypt, name='upload_file_encrypt'),
    path('decrypt', views.upload_file_decrypt, name='upload_file_decrypt')
]