#include "handler.hpp"

#include <models/models.hpp>
#include <models/requests.hpp>
#include <common/exceptions.hpp>

namespace {
    using ParseArgException = deli_auth::common::exceptions::ParseArgException;
    using UserTypeViews = deli_auth::views::UserType;
    using UserTypeModels = deli_auth::models::UserType;

    UserTypeViews ParseUserType (const UserTypeModels type){
        switch(type){
            case UserTypeModels::kUserTypeCustomer:
                return UserTypeViews::kUserTypeCustomer;
            case UserTypeModels::kUserTypeCourier:
                return UserTypeViews::kUserTypeCourier;
            case UserTypeModels::kUserTypeAdmin:
                return UserTypeViews::kUserTypeAdmin;
        }
    }

    struct RequestData{
        int64_t id;
        std::string login;
    };

    RequestData ParseUserGetRequestArg(const userver::server::http::HttpRequest &request,
                                       const std::string& arg_name_id ,const std::string& arg_name_login){
        bool request_id_has_arg = request.HasArg("id");
        bool request_login_has_arg = request.HasArg("login");
        RequestData res{
                .id = -1,
                .login = "",
        };

        if (!(request_id_has_arg ^ request_login_has_arg)){
            throw ParseArgException("Wrong request: Invalid parameters were passed, you need to pass id or login");
        }

        if(request_id_has_arg){
            try{
                res.id = std::stoll(request.GetArg(arg_name_id));
            } catch (std::invalid_argument& exception)   {
                throw ParseArgException("Wrong request: error converting parameter '" + arg_name_id + "'");
            } catch (std::out_of_range &exception) {
                throw ParseArgException("Wrong request: parameter '" + arg_name_id + "' is too large");
            }
            if (res.id < 0){
                throw ParseArgException("Wrong request: parameter '" + arg_name_id + "' must be greater than or equal to zero");
            }
        }
        else{
            try{
                res.login = request.GetArg(arg_name_login);
            } catch (std::invalid_argument& exception){
                throw ParseArgException("Wrong request: error converting parameter '" + arg_name_id + "'");
            }
        }

        return res;
    }
}


namespace deli_auth::views::v1::user::get {

    Handler::Handler(const userver::components::ComponentConfig &config,
                     const userver::components::ComponentContext &component_context) :
            userver::server::handlers::HttpHandlerJsonBase(config, component_context),
            requester_(component_context.FindComponent<components::Requester>()) {}

    userver::formats::json::Value Handler::HandleRequestJsonThrow(
            const userver::server::http::HttpRequest &request, const userver::formats::json::Value &json,
            userver::server::request::RequestContext &) const try {

        LOG_INFO() << "START MAKE REQUEST";
        const auto request_data = ParseUserGetRequestArg(request,"id","login");

        models::User user_response;
        LOG_INFO() << "REQUEST DONE, REQUEST data in"<< request_data.id;
        if (request_data.id != -1) {
            LOG_INFO() << "START MAKE RESPONSE ID" << request_data.id;
            user_response = requester_.DoDBQuery(models::requests::GetUserById, request_data.id);
        }
        else if (request_data.login != "") {
            LOG_INFO() << "START MAKE RESPONSE LOGIN" << request_data.login;
            user_response = requester_.DoDBQuery(models::requests::GetUserByLogin, request_data.login);
        }
        Response200 response200{
                .login = user_response.login,
                .user_type = ParseUserType(user_response.user_type),
        };
        LOG_DEBUG() << "RESPONSE DONE";
        request.SetResponseStatus(userver::server::http::HttpStatus::kOk);
        return Serialize(
                response200,
                userver::formats::serialize::To<userver::formats::json::Value>());

    }catch (deli_auth::common::exceptions::ParseArgException &exception ) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kBadRequest);
        return Serialize(
                Response400{
                        .message = exception.what()
                },
                userver::formats::serialize::To<userver::formats::json::Value>());
    }catch(deli_auth::common::exceptions::UserNotFound &exception){
        request.SetResponseStatus(userver::server::http::HttpStatus::kNotFound);
        return Serialize(
                Response404{
                        .message = exception.what()
                },
                userver::formats::serialize::To<userver::formats::json::Value>());
    }


} // namespace deli_main::views::v1::order::post
