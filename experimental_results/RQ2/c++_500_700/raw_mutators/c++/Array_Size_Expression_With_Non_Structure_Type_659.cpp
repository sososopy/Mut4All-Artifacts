//header file
#pragma once
#include "Mutator_base.h"

/**
 * array_size_expression_with_non_structure_type_659
 */ 
class MutatorFrontendAction_659 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(659)

private:
    class MutatorASTConsumer_659 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_659(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/array_size_expression_with_non_structure_type_659.h"

// ========================================================================================================
#define MUT659_OUTPUT 1

void MutatorFrontendAction_659::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *UE = Result.Nodes.getNodeAs<clang::UnaryExprOrTypeTraitExpr>("sizeofExpr")) {
      //Filter nodes in header files
      if (!UE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     UE->getBeginLoc()))
        return;
      if (UE->getKind() == UETT_SizeOf && UE->isArgumentType()) {
        QualType qt = UE->getArgumentType();
        if (qt->isBuiltinType() && !qt->isStructureType()) {
          //Get the source code text of target node
          auto exprText = stringutils::rangetoStr(*(Result.SourceManager), UE->getSourceRange());
          //Perform mutation on the source code text by applying string replacement
          std::string mutatedExpr = exprText + ".undeclaredMember";
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(UE->getSourceRange(), mutatedExpr);
        }
      }
    }
}
  
void MutatorFrontendAction_659::MutatorASTConsumer_659::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = unaryExprOrTypeTraitExpr(hasKind(UETT_SizeOf), isArgumentType()).bind("sizeofExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}