require 'test_helper'

class OnShelvesControllerTest < ActionController::TestCase
  setup do
    @on_shelf = on_shelves(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:on_shelves)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create on_shelf" do
    assert_difference('OnShelf.count') do
      post :create, on_shelf: { book_id: @on_shelf.book_id, shelf_id: @on_shelf.shelf_id }
    end

    assert_redirected_to on_shelf_path(assigns(:on_shelf))
  end

  test "should show on_shelf" do
    get :show, id: @on_shelf
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @on_shelf
    assert_response :success
  end

  test "should update on_shelf" do
    patch :update, id: @on_shelf, on_shelf: { book_id: @on_shelf.book_id, shelf_id: @on_shelf.shelf_id }
    assert_redirected_to on_shelf_path(assigns(:on_shelf))
  end

  test "should destroy on_shelf" do
    assert_difference('OnShelf.count', -1) do
      delete :destroy, id: @on_shelf
    end

    assert_redirected_to on_shelves_path
  end
end
