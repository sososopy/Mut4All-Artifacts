//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Constexpr_With_Incomplete_Type_200
 */ 
class MutatorFrontendAction_200 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(200)

private:
    class MutatorASTConsumer_200 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_200(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Constexpr_With_Incomplete_Type_200.h"

// ========================================================================================================
#define MUT200_OUTPUT 1

void MutatorFrontendAction_200::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("ConstexprVar")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;

      //Get the source code text of target node
      auto varName = VD->getNameAsString();
      std::string structDecl = "struct IncompleteType;";
      std::string newDecl = "constexpr struct IncompleteType " + varName + " = { 0 };";

      //Perform mutation on the source code text by applying string replacement
      Rewrite.InsertTextBefore(VD->getBeginLoc(), structDecl + "\n");
      Rewrite.ReplaceText(VD->getSourceRange(), newDecl);
    }
}
  
void MutatorFrontendAction_200::MutatorASTConsumer_200::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(isConstexpr())).bind("ConstexprVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}