//header file
#pragma once
#include "Mutator_base.h"

/**
 * Static_Constexpr_Member_Function_And_Usage_205
 */ 
class MutatorFrontendAction_205 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(205)

private:
    class MutatorASTConsumer_205 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_205(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXMethodDecl *> cur_methods;
        std::vector<const clang::VarDecl *> cur_vars;
    };
};

//source file
#include "../include/Static_Constexpr_Member_Function_And_Usage_205.h"

// ========================================================================================================
#define MUT205_OUTPUT 1

void MutatorFrontendAction_205::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MD = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("StaticConstexprMethods")) {
      if (!MD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MD->getLocation()))
        return;
      if (MD->isStatic() && MD->isConstexpr()) {
        cur_methods.push_back(MD);
      }
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vars")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      cur_vars.push_back(VD);
    } else if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("Calls")) {
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;
      for (auto method : cur_methods) {
        if (CE->getDirectCallee() == method) {
          for (auto var : cur_vars) {
            if (CE->getBeginLoc() == var->getBeginLoc()) {
              if (CE->getNumArgs() == 0) {
                auto content = stringutils::rangetoStr(
                    *(Result.SourceManager), CE->getSourceRange());
                content.insert(content.rfind(')'), "<int>");
                Rewrite.ReplaceText(
                    CharSourceRange::getTokenRange(CE->getSourceRange()),
                    content);
              }
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_205::MutatorASTConsumer_205::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxMethodDecl().bind("StaticConstexprMethods");
    auto var_matcher = varDecl().bind("Vars");
    auto call_matcher = callExpr().bind("Calls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(var_matcher, &callback);
    matchFinder.addMatcher(call_matcher, &callback);
    matchFinder.matchAST(Context);
}