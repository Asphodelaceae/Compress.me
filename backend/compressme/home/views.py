from django.shortcuts import render
from django.views.decorators.csrf import csrf_exempt

# Create your views here.
@csrf_exempt
def home_page(request):
    return render(request, 'page.html')  # Rendering 'home.html' template
