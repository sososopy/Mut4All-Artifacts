//header file
#pragma once
#include "Mutator_base.h"

/**
 * replace_default_argument_with_template_parameter_24
 */ 
class MutatorFrontendAction_24 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(24)

private:
    class MutatorASTConsumer_24 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_24(Rewriter &R) : TheRewriter(R) {}
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
        const FunctionDecl *originalFunction;
        std::string templatizedFunction;
    };
};

//source file
#include "../include/replace_default_argument_with_template_parameter_24.h"

// ========================================================================================================
#define MUT24_OUTPUT 1

void MutatorFrontendAction_24::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithDefaultArg")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
        return;

      if (FD->getNumParams() > 0) {
          for (auto *param : FD->parameters()) {
              if (param->hasDefaultArg()) {
                  originalFunction = FD;
                  std::string originalDefaultArg = param->getDefaultArg()->getStmtClassName();
                  std::string funcName = FD->getNameAsString();
                  std::string templateDecl = "template<typename T = int> ";
                  std::string newParam = "int x = T()";
                  std::string templateStruct = "template<typename U = int> struct TemplateStruct { friend void " + funcName + "(int x = U()); };";

                  templatizedFunction = templateDecl + "void " + funcName + "(" + newParam + ");";
                  templatizedFunction += "\n" + templateStruct;
                  templatizedFunction += "\nvoid testFunction() { TemplateStruct<> instance; " + funcName + "(); }";

                  auto funcRange = FD->getSourceRange();
                  Rewrite.ReplaceText(funcRange, templatizedFunction);
                  break;
              }
          }
      }
    }
}
  
void MutatorFrontendAction_24::MutatorASTConsumer_24::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasAnyParameter(hasDefaultArgument())).bind("FunctionWithDefaultArg");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}