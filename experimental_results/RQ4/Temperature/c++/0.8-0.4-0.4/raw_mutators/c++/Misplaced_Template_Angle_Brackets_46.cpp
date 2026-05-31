//header file
#pragma once
#include "Mutator_base.h"

/**
 * misplaced_template_angle_brackets_46
 */ 
class MutatorFrontendAction_46 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(46)

private:
    class MutatorASTConsumer_46 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_46(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/misplaced_template_angle_brackets_46.h"

// ========================================================================================================
#define MUT46_OUTPUT 1

void MutatorFrontendAction_46::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithTemplate")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->getReturnType()->isDependentType() || FD->getNumParams() > 0) {
        std::string returnTypeStr = FD->getReturnType().getAsString();
        size_t pos = returnTypeStr.find(">>");
        if (pos == std::string::npos) {
          pos = returnTypeStr.find(">");
          if (pos != std::string::npos) {
            returnTypeStr.insert(pos + 1, ">");
          }
        }

        std::string funcDecl = returnTypeStr + " " + FD->getNameAsString() + "(";
        for (unsigned i = 0; i < FD->getNumParams(); ++i) {
          if (i > 0) funcDecl += ", ";
          funcDecl += FD->getParamDecl(i)->getType().getAsString();
        }
        funcDecl += ");";

        Rewrite.ReplaceText(FD->getSourceRange(), funcDecl);
      }
    }
}
  
void MutatorFrontendAction_46::MutatorASTConsumer_46::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasReturnType(hasDescendant(templateSpecializationType()))).bind("FunctionWithTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}