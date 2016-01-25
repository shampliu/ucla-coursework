class HomeController < ApplicationController
  before_action :load_activities

  def index
  end

  private 
  
  def load_activities
    @activities = PublicActivity::Activity.order('created_at DESC').limit(20)
  end
end

