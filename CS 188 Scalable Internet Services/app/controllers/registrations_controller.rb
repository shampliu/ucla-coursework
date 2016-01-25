class RegistrationsController < Devise::RegistrationsController
  protected

  def after_sign_up_path_for(resource)
    '/?user_id=' + resource.id.to_s
  end
end