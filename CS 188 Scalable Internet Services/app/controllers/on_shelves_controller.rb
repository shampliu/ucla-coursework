class OnShelvesController < ApplicationController
  before_action :set_on_shelf, only: [:show, :edit, :update, :destroy]

  # GET /on_shelves
  # GET /on_shelves.json
  def index
    @on_shelves = OnShelf.all
  end

  # GET /on_shelves/1
  # GET /on_shelves/1.json
  def show
  end

  # GET /on_shelves/new
  def new
    @on_shelf = OnShelf.new
  end

  # GET /on_shelves/1/edit
  def edit
  end

  # POST /on_shelves
  # POST /on_shelves.json
  def create
    @on_shelf = OnShelf.new(:book_id => params[:book_id], :shelf_id => params[:shelf_id])
    @book = Book.find_by(id: params[:book_id])
    respond_to do |format|
      if @on_shelf.save
        format.html { redirect_to @book, notice: 'On shelf was successfully created.' }
        format.json { render :show, status: :created, location: @on_shelf }
      else
        format.html { render :new }
        format.json { render json: @on_shelf.errors, status: :unprocessable_entity }
      end
    end
  end

  # PATCH/PUT /on_shelves/1
  # PATCH/PUT /on_shelves/1.json
  def update
    respond_to do |format|
      if @on_shelf.update(on_shelf_params)
        format.html { redirect_to @on_shelf, notice: 'On shelf was successfully updated.' }
        format.json { render :show, status: :ok, location: @on_shelf }
      else
        format.html { render :edit }
        format.json { render json: @on_shelf.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /on_shelves/1
  # DELETE /on_shelves/1.json
  def destroy
    @on_shelf.destroy
    respond_to do |format|
      format.html { redirect_to on_shelves_url, notice: 'On shelf was successfully destroyed.' }
      format.json { head :no_content }
    end
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_on_shelf
      @on_shelf = OnShelf.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def on_shelf_params
      
      params.require(:book_id).permit(:book_id, :shelf_id)
    end
end
