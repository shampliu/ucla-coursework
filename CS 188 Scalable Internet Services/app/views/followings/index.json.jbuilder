json.array!(@followings) do |following|
  json.extract! following, :id, :person_id, :follower_id
  json.url following_url(following, format: :json)
end
