//header file
#pragma once
#include "Mutator_base.h"

/**
 * InvalidTypeCastInsertion_298
 */ 
class MutatorFrontendAction_298 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(298)

private:
    class MutatorASTConsumer_298 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_298(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Mutator_InvalidTypeCastInsertion_298.h"

// ========================================================================================================
#define MUT298_OUTPUT 1

void MutatorFrontendAction_298::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::CStyleCastExpr>("CStyleCast")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getLocStart()))
        return;
      //Get the source code text of target node
      auto castType = CE->getTypeAsWritten();
      std::string castTypeStr = castType.getAsString();
      //Perform mutation on the source code text by applying string replacement
      std::string incompatibleType;
      if (castTypeStr.find("int") != std::string::npos) {
        incompatibleType = "float";
      } else if (castTypeStr.find("float") != std::string::npos) {
        incompatibleType = "int";
      } else if (castTypeStr.find("char") != std::string::npos) {
        incompatibleType = "double";
      } else {
        return;
      }
      std::string mutatedCode = "(" + incompatibleType + ")" + CE->getSubExprAsWritten();
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()), mutatedCode);
    }
}
  
void MutatorFrontendAction_298::MutatorASTConsumer_298::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    ExprMatcher matcher = cStyleCastExpr().bind("CStyleCast");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}