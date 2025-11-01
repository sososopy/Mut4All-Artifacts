//header file
#pragma once
#include "Mutator_base.h"

/**
 * Duplicate_Access_Specifier_313
 */ 
class MutatorFrontendAction_313 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(313)

private:
    class MutatorASTConsumer_313 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_313(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Duplicate_Access_Specifier_313.h"

// ========================================================================================================
#define MUT313_OUTPUT 1

void MutatorFrontendAction_313::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("FunctionDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      auto accessSpecifier = FD->getAccess();
      std::string accessStr;
      switch (accessSpecifier) {
        case clang::AS_public:
          accessStr = "public";
          break;
        case clang::AS_protected:
          accessStr = "protected";
          break;
        case clang::AS_private:
          accessStr = "private";
          break;
        default:
          return;
      }

      auto startLoc = FD->getBeginLoc();
      auto endLoc = FD->getEndLoc();
      std::string funcDecl = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());

      if (funcDecl.find(accessStr) != std::string::npos) {
        funcDecl.insert(funcDecl.find(accessStr), accessStr + " ");
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), funcDecl);
      }
    }
}
  
void MutatorFrontendAction_313::MutatorASTConsumer_313::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl().bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}