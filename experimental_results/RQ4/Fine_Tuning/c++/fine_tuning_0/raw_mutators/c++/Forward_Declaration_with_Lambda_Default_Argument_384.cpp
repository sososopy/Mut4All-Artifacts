//header file
#pragma once
#include "Mutator_base.h"

/**
 * Forward_Declaration_with_Lambda_Default_Argument_384
 */ 
class MutatorFrontendAction_384 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(384)

private:
    class MutatorASTConsumer_384 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_384(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
        std::vector<const clang::CXXRecordDecl *> fwd_decls;
    };
};

//source file
#include "../include/Forward_Declaration_with_Lambda_Default_Argument_384.h"

// ========================================================================================================
#define MUT384_OUTPUT 1

void MutatorFrontendAction_384::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->isThisDeclarationADefinition())
        return;
      if (!FD->hasBody())
        return;
      if (FD->getNumParams() == 0)
        return;
      auto params = FD->parameters();
      for (auto param : params) {
        if (param->hasDefaultArg()) {
          auto default_expr = param->getDefaultArg();
          if (default_expr == nullptr)
            return;
          auto default_expr_str = stringutils::rangetoStr(
              *(Result.SourceManager), default_expr->getSourceRange());
          llvm::outs() << default_expr_str << '\n';
          if (fwd_decls.empty())
            return;
          int index = getrandom::getRandomIndex(fwd_decls.size() - 1);
          auto fwd_decl = fwd_decls[index];
          auto fwd_decl_name = fwd_decl->getNameAsString();
          auto lambda_expr = "[=]()->" + fwd_decl_name + "{return " +
                             default_expr_str + ";}";
          llvm::outs() << lambda_expr << '\n';
          Rewrite.ReplaceText(
              CharSourceRange::getTokenRange(default_expr->getSourceRange()),
              lambda_expr);
        }
      }
    } else if (auto *CD =
                   Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;
      if ((!CD->isStruct() && !CD->isClass()) || CD->isLambda())
        return;
      if (CD->isCompleteDefinition() == false)
        fwd_decls.push_back(CD);
    }
  }
  
void MutatorFrontendAction_384::MutatorASTConsumer_384::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("Functions");
    auto class_matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.matchAST(Context);
}