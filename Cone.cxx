#include <vtkConeSource.h>
#include <vtkNew.h>

#include <vtkActor.h>
#include <vtkBoxWidget.h>
#include <vtkNamedColors.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkTransform.h>

namespace {
	class vtkBoxWidgetCallback : public vtkCommand
	{
	public:
		static vtkBoxWidgetCallback* New()
		{
			return new vtkBoxWidgetCallback;
		}
		void Execute(vtkObject* caller, unsigned long, void*) override
		{
			vtkNew<vtkTransform> t;
			auto widget = reinterpret_cast<vtkBoxWidget*>(caller);
			widget->GetTransform(t);
			widget->GetProp3D()->SetUserTransform(t);
		}
	};
}


int main(int, char*[])
{
  vtkNew<vtkNamedColors> colors;

  // Create a cone
  vtkNew<vtkConeSource> coneSource;
  coneSource->Update();

  // Create a mapper and actor
  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputConnection(coneSource->GetOutputPort());

  vtkNew<vtkProperty> coneProperty;
  coneProperty->GetDiffuseColor(colors->GetColor3d("brique").GetData());

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);
  actor->SetProperty(coneProperty);

  vtkNew<vtkActor> actor2;
  actor2->SetMapper(mapper);
  //actor2->SetProperty(coneProperty);
  actor2->SetPosition(0, 1, 0);
  actor2->GetProperty()->SetDiffuseColor(colors->GetColor3d("LightSeaGreen").GetData());
  

  // Create a renderer, render window, and interactor
  vtkNew<vtkRenderer> renderer;
  vtkNew<vtkRenderer> renderer2;
  vtkNew<vtkRenderWindow> renderWindow;
  renderWindow->AddRenderer(renderer);
  renderWindow->AddRenderer(renderer2);
  renderWindow->SetSize(640, 480);

  vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
  renderWindowInteractor->SetRenderWindow(renderWindow);

  // Box Widget
  vtkNew<vtkBoxWidget> boxWidget;
  boxWidget->SetInteractor(renderWindowInteractor);
  boxWidget->SetPlaceFactor(1.25);
  boxWidget->GetOutlineProperty()->SetColor(colors->GetColor3d("Gold").GetData());

  boxWidget->SetProp3D(actor2);
  boxWidget->PlaceWidget();

  vtkNew<vtkBoxWidgetCallback> callback;
  boxWidget->AddObserver(vtkCommand::InteractionEvent, callback);
  boxWidget->On();


  // Add the actors to the scene
  renderer->AddActor(actor);
  renderer2->AddActor(actor2);
  renderer->SetBackground(colors->GetColor3d("Salmon").GetData());

  renderer2->AddActor(actor);
  renderer2->SetBackground(colors->GetColor3d("DarkBlue").GetData());

  renderer->SetViewport(0.0, 0.0, 0.5, 1.0);
  renderer2->SetViewport(0.5, 0.0, 1.0, 1.0);

  // Render and interact
  renderWindow->SetWindowName("Cone");
  renderWindow->Render();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}
