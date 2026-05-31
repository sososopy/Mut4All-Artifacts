//header file
#pragma once
#include "Mutator_base.h"

/**
 * Specialize_Variadic_Template_Function_196
 */ 
class MutatorFrontendAction_196 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(196)

private:
    class MutatorASTConsumer_196 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_196(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl *> target_functions;
    };
};

//source file
#include "../include/specialize_variadic_template_function_196.h"

// ========================================================================================================
#define MUT196_OUTPUT 1

void MutatorFrontendAction_196::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionDecl>("VariadicTemplate")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (MT->isVariadic() == false)
        return;
      auto decl = stringutils::rangetoStr(*(Result.SourceManager),
                                          MT->getSourceRange());
      if (decl.find("template") != string::npos &&
          decl.find("typename...") != string::npos) {
        target_functions.push_back(MT);
      }
    } else if (auto *CL = Result.Nodes.getNodeAs<clang::CallExpr>("CallExpr")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getBeginLoc()))
        return;
      auto callee = CL->getCalleeDecl();
      if (callee == nullptr)
        return;
      for (auto target : target_functions) {
        if (target->getNameAsString() == callee->getNameAsString()) {
          auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                                 CL->getSourceRange());
          if (content.find('<') != string::npos &&
              content.find('>') != string::npos) {
            content.insert(content.find('>'), ", int");
          }
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(CL->getSourceRange()), content);
        }
      }
    }
}
  
void MutatorFrontendAction_196::MutatorASTConsumer_196::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("VariadicTemplate");
    auto call_matcher = callExpr().bind("CallExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(call_matcher, &callback);
    matchFinder.matchAST(Context);
}