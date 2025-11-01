//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Rvalue_Reference_With_Lvalue_Reference_7
 */ 
class MutatorFrontendAction_7 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(7)

private:
    class MutatorASTConsumer_7 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_7(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_rvalue_reference_with_lvalue_reference_7.h"

// ========================================================================================================
#define MUT7_OUTPUT 1

void MutatorFrontendAction_7::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("rvalueRefVar")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;

      //Get the source code text of target node
      auto typeSourceRange = VD->getTypeSourceInfo()->getTypeLoc().getSourceRange();
      auto typeText = stringutils::rangetoStr(*(Result.SourceManager), typeSourceRange);

      //Perform mutation on the source code text by applying string replacement
      auto mutatedTypeText = std::regex_replace(typeText, std::regex("&&"), "&");

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(typeSourceRange), mutatedTypeText);
    }
}
  
void MutatorFrontendAction_7::MutatorASTConsumer_7::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(rValueReferenceType())).bind("rvalueRefVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}