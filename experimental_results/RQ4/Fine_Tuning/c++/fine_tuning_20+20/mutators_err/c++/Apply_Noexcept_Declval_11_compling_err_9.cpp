//header file
#pragma once
#include "Mutator_base.h"

/**
 * Apply_Noexcept_Declval_11
 */ 
class MutatorFrontendAction_11 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(11)

private:
    class MutatorASTConsumer_11 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_11(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        std::vector<const clang::FunctionDecl *> cur_funs;
    };
};

//source file
#include "../include/mutator_11.h"

// ========================================================================================================
#define MUT11_OUTPUT 1

void MutatorFrontendAction_11::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FL = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FL->getLocation()))
        return;
      if (FL->isDefaulted())
        return;
      if (FL->isDeleted())
        return;
      if (FL->isOverloadedOperator())
        return;
      if (FL->isFunctionTemplateSpecialization())
        return;
      if (FL->isTemplateInstantiation())
        return;
      if (FL->isDependentContext())
        return;
      cur_funs.push_back(FL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>( "Classes")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      for (auto cur_fun : cur_funs) {
        if (llvm::isa<clang::CXXMethodDecl>(cur_fun) &&
            llvm::cast<clang::CXXMethodDecl>(cur_fun)->isLambdaStaticInvoker()) {
          auto lambda = llvm::cast<clang::CXXMethodDecl>(cur_fun)
                            ->getParent()
                            ->getLambdaContextDecl();
          auto lambda_content = stringutils::rangetoStr(
              *(Result.SourceManager), lambda->getSourceRange());
          auto lambda_decl = lambda;
          auto params = lambda_decl->fields();
          if (params.begin() == params.end())
            return;
          auto param = params.begin();
          std::advance(param, getrandom::getRandomIndex(std::distance(
                                  params.begin(), params.end()) -
                              1));
          auto type = param->getType().getAsString();
          auto noexcept_expr = "noexcept(std::declval<" + type + ">())";
          if (lambda_content.find("noexcept") != string::npos) {
            auto pos = lambda_content.find("noexcept");
            auto end_pos = lambda_content.find(')', pos);
            lambda_content.replace(pos, end_pos - pos + 1, noexcept_expr);
          } else {
            lambda_content.insert(lambda_content.find('[') + 1,
                                  "/*mut11*/");
            lambda_content.insert(lambda_content.find(']'), noexcept_expr);
          }
          content.replace(content.find(lambda_content), lambda_content.size(),
                          lambda_content);
        } else {
          auto params = cur_fun->parameters();
          if (params.empty())
            return;
          auto param = params[getrandom::getRandomIndex(params.size() - 1)];
          auto type = param->getType().getAsString();
          auto noexcept_expr = "noexcept(std::declval<" + type + ">())";
          if (cur_fun->getType().getAsString().find("noexcept") !=
              string::npos) {
            auto expanded = cur_fun->getType().getAsString();
            if (expanded.find("noexcept") != string::npos) {
              auto pos = expanded.find("noexcept");
              auto end_pos = expanded.find(')', pos);
              expanded.replace(pos, end_pos - pos + 1, noexcept_expr);
            } else
              expanded.insert(expanded.find(')'), " " + noexcept_expr);
            content.replace(content.find(expanded), expanded.size(), expanded);
          } else {
            auto decl = stringutils::rangetoStr(*(Result.SourceManager),
                                                cur_fun->getSourceRange());
            if (decl.find("noexcept") != string::npos) {
              auto pos = decl.find("noexcept");
              auto end_pos = decl.find(')', pos);
              decl.replace(pos, end_pos - pos + 1, noexcept_expr);
            } else
              decl.insert(decl.rfind(')'), " " + noexcept_expr);
            content.replace(content.find(decl), decl.size(), decl);
          }
        }
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_11::MutatorASTConsumer_11::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("Functions");
    auto class_matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.matchAST(Context);
}