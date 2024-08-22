from django.http import JsonResponse
from django.views.decorators.http import require_http_methods
from django.views.decorators.csrf import csrf_exempt

@csrf_exempt  # Optionally, use CSRF token in production
@require_http_methods(["POST"])
def upload_file(request):
    files = request.FILES.getlist('files[]')
    for f in files:
        handle_file(f)
    return JsonResponse({'message': 'Files processed successfully'})

def handle_file(f):
    with open(f.name, 'wb+') as destination:
        for chunk in f.chunks():
            destination.write(chunk)
