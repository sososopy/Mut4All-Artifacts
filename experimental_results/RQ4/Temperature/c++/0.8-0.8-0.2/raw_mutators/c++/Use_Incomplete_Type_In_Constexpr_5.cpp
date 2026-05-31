//header file
#pragma once
#include "Mutator_base.h"

/**
 * use_incomplete_type_in_constexpr_5
 */ 
class MutatorFrontendAction_5 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(5)

private:
    class MutatorASTConsumer_5 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_5(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/use_incomplete_type_in_constexpr_5.h"

// ========================================================================================================
#define MUT5_OUTPUT 1

void MutatorFrontendAction_5::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("ConstexprVar")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      // Ensure the variable is constexpr
      if (!VD->isConstexpr())
        return;

      // Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 VD->getSourceRange());

      // Constructing the incomplete type usage
      std::string newType = "struct Y;"; // Forward declaration
      std::string newDeclaration = "constexpr struct Y y = {0};";

      if (!Result.Context->getTranslationUnitDecl()->containsDeclNamed("Y")) {
        // Insert the incomplete type declaration
        Rewrite.InsertTextBefore(VD->getBeginLoc(), newType + "\n");
        // Perform mutation on the source code text by applying string replacement
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), newDeclaration);
      }
    }
}
  
void MutatorFrontendAction_5::MutatorASTConsumer_5::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(isConstexpr()).bind("ConstexprVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}