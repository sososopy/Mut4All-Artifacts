//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Member_Operator_Delete_385
 */ 
class MutatorFrontendAction_385 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(385)

private:
    class MutatorASTConsumer_385 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_385(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Member_Operator_Delete_385.h"

// ========================================================================================================
#define MUT385_OUTPUT 1

void MutatorFrontendAction_385::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *OD = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("operatorDelete")) {
      //Filter nodes in header files
      if (!OD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     OD->getLocation()))
        return;

      if (OD->getNumParams() == 1) {
        auto *param = OD->getParamDecl(0);
        if (param->getType()->isRecordType()) {
          //Get the source code text of target node
          auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                     OD->getSourceRange());
          //Perform mutation on the source code text by applying string replacement
          size_t pos = declaration.find(param->getType().getAsString());
          if (pos != std::string::npos) {
              declaration.replace(pos, param->getType().getAsString().length(), "void*");
          }
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(OD->getSourceRange()), declaration);
        }
      }
    }
}
  
void MutatorFrontendAction_385::MutatorASTConsumer_385::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(isDeleted(), hasName("operator delete")).bind("operatorDelete");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}