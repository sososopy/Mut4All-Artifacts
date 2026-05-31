//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Lambda_With_Customized_Struct_250
 */ 
class MutatorFrontendAction_250 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(250)

private:
    class MutatorASTConsumer_250 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_250(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        std::vector<const clang::FunctionDecl *> cur_functions;
    };
};

//source file
#include "../include/replace_lambda_with_customized_struct_250.h"

// ========================================================================================================
#define MUT250_OUTPUT 1

void MutatorFrontendAction_250::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LM = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambdas")) {
      if (!LM || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LM->getBeginLoc()))
        return;
      auto lambda_name = "Lambda";
      if (!cur_functions.empty()) {
        auto cur_func = cur_functions.back();
        lambda_name += cur_func->getNameAsString();
      }
      llvm::outs() << lambda_name << '\n';
      auto capture = LM->capture_begin();
      std::string capture_list = "";
      while (capture != LM->capture_end()) {
        if (capture->capturesThis()) {
          capture_list += "this";
        } else if (capture->capturesVariable()) {
          capture_list += capture->getCapturedVar()->getNameAsString();
        }
        capture++;
        if (capture != LM->capture_end())
          capture_list += ",";
      }
      if (capture_list != "")
        capture_list = ":" + capture_list;
      llvm::outs() << capture_list << '\n';
      auto lambda_text = stringutils::rangetoStr(*(Result.SourceManager),
                                                 LM->getSourceRange());
      auto pos = lambda_text.find('[');
      if (pos != string::npos)
        lambda_text.replace(pos, 1, "/*mut250*/");
      pos = lambda_text.find('{');
      if (pos != string::npos)
        lambda_text.replace(pos, 1, capture_list + "{");
      llvm::outs() << lambda_text << '\n';
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(LM->getSourceRange()), lambda_text);
    } else if (auto *FL = Result.Nodes.getNodeAs<clang::FunctionDecl>(
                   "Functions")) {
      if (!FL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FL->getBeginLoc()))
        return;
      if (!FL->hasBody())
        return;
      cur_functions.push_back(FL);
      auto func_name = FL->getNameAsString();
      auto lambda_name = "Lambda" + func_name;
      auto captures = FL->captures();
      std::string capture_list = "";
      for (auto capture : captures) {
        if (capture.capturesThis()) {
          capture_list += "this";
        } else if (capture.capturesVariable()) {
          capture_list += capture.getCapturedVar()->getNameAsString();
        }
        capture_list += ",";
      }
      llvm::outs() << capture_list << '\n';
      std::string struct_definition = "struct " + lambda_name + "{\n";
      if (capture_list != "")
        struct_definition += capture_list;
      struct_definition += "int operator()";
      auto params = FL->parameters();
      std::string param_list = "(";
      for (auto param : params) {
        param_list += param->getOriginalType().getAsString() + " " +
                      param->getNameAsString() + ",";
      }
      if (param_list.back() == ',')
        param_list.pop_back();
      param_list += ")";
      struct_definition += param_list;
      if (FL->isConstexpr())
        struct_definition += " const";
      struct_definition += "{\n";
      auto body = FL->getBody();
      auto body_text = stringutils::rangetoStr(*(Result.SourceManager),
                                               body->getSourceRange());
      struct_definition += body_text.substr(1, body_text.size() - 2);
      struct_definition += "\n}\n";
      struct_definition += "};\n";
      llvm::outs() << struct_definition << '\n';
      Rewrite.ReplaceText(FL->getBeginLoc(), 0, "/*mut250*/" + struct_definition);
    } else if (auto *MN =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("Main")) {
      if (!MN || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MN->getBeginLoc()))
        return;
      if (!MN->hasBody())
        return;
      cur_functions.push_back(MN);
      auto lambda_name = "LambdaMain";
      auto captures = MN->captures();
      std::string capture_list = "";
      for (auto capture : captures) {
        if (capture.capturesThis()) {
          capture_list += "this";
        } else if (capture.capturesVariable()) {
          capture_list += capture.getCapturedVar()->getNameAsString();
        }
        capture_list += ",";
      }
      llvm::outs() << capture_list << '\n';
      std::string struct_definition = "struct " + lambda_name + "{\n";
      if (capture_list != "")
        struct_definition += capture_list;
      struct_definition += "int operator()";
      auto params = MN->parameters();
      std::string param_list = "(";
      for (auto param : params) {
        param_list += param->getOriginalType().getAsString() + " " +
                      param->getNameAsString() + ",";
      }
      if (param_list.back() == ',')
        param_list.pop_back();
      param_list += ")";
      struct_definition += param_list;
      if (MN->isConstexpr())
        struct_definition += " const";
      struct_definition += "{\n";
      auto body = MN->getBody();
      auto body_text = stringutils::rangetoStr(*(Result.SourceManager),
                                               body->getSourceRange());
      struct_definition += body_text.substr(1, body_text.size() - 2);
      struct_definition += "\n}\n";
      struct_definition += "};\n";
      llvm::outs() << struct_definition << '\n';
      Rewrite.ReplaceText(MN->getBeginLoc(), 0, "/*mut250*/" + struct_definition);
    }
}
  
void MutatorFrontendAction_250::MutatorASTConsumer_250::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto lambda_matcher = lambdaExpr().bind("Lambdas");
    auto func_matcher = functionDecl(unless(cxxMethodDecl())).bind("Functions");
    auto main_matcher = functionDecl(isMain()).bind("Main");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambda_matcher, &callback);
    matchFinder.addMatcher(func_matcher, &callback);
    matchFinder.addMatcher(main_matcher, &callback);
    matchFinder.matchAST(Context);
}