#include "handler.hpp"

#include <models/models.hpp>
#include <models/requests.hpp>

namespace {
    class ParseArgException : public std::exception {
    public:
        explicit ParseArgException(std::string msg) : msg_(std::move(msg)) {}

        const char *what() const

        noexcept final{return msg_.c_str();}

    private:
        std::string msg_;
    };
}

namespace deli_auth::views::v1::auth::login::post {

  Handler::Handler(const userver::components::ComponentConfig &config,
                   const userver::components::ComponentContext &component_context) :
    userver::server::handlers::HttpHandlerJsonBase(config, component_context),
    requester_(component_context.FindComponent<components::Requester>()) {}

  userver::formats::json::Value Handler::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest &request, const userver::formats::json::Value &json,
    userver::server::request::RequestContext &) const try {

    const auto request_data = json.As<Request>();

    // получаем логин
    models::User user{
      .login = request_data.login
    };
    // получаем пароль из хедера
    const auto& password_header = request.GetHeader("password"); //http::headers::kAuthorization);

    // сравниваем с данными из бд. по логину ищем юзера и получаем его пароль. Далее сравниваем пароли
    // Todo: понять, что возвращает DoDBQuery, если введенного логина не существует
    const auto password_bd = requester_.DoDBQuery(models::requests::SelectPassword, user);

    if (password_header != password_bd){
      throw ParseArgException("Password is wrong");
    }

    // если все ок, генерируем токен, добавляем в бд и возвращаем в хедере
//    const auto user_id = requester_.DoDBQuery(models::requests::SelectUserID, user);
//    models::BearerTokens bearer_tokens{
//        .user_id = user_id
//    };
//    const std::vector<models::BearerTokens> tokens_obj = requester_.DoDBQuery(models::requests::SelectBearerTokens, bearer_tokens);

    Response200 response200{
      .is_auth = true
    };

    request.SetResponseStatus(userver::server::http::HttpStatus::kOk);
    // Todo: вернуть токен через хедер

    return Serialize(
      response200,
      userver::formats::serialize::To<userver::formats::json::Value>());

  } catch (const userver::formats::json::ParseException &exception) {
    request.SetResponseStatus(userver::server::http::HttpStatus::kBadRequest);
    return Serialize(
      Response400{
        .message = exception.what()
      },
      userver::formats::serialize::To<userver::formats::json::Value>());
  } catch (std::string err_message) {
    request.SetResponseStatus(userver::server::http::HttpStatus::kBadRequest);
    return Serialize(
        Response400{
          .message = err_message
        },
        userver::formats::serialize::To<userver::formats::json::Value>()
            );
  }


} // namespace deli_auth::views::v1::auth::login::post
