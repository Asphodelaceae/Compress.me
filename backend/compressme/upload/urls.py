from django.urls import path
from . import views

urlpatterns = [
    path('', views.upload_file_encrypt, name='upload_file_encrypt'),
]
