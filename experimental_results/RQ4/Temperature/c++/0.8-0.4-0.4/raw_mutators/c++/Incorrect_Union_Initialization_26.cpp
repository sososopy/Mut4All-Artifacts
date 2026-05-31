//header file
#pragma once
#include "Mutator_base.h"

/**
 * incorrect_union_initialization_26
 */ 
class MutatorFrontendAction_26 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(26)

private:
    class MutatorASTConsumer_26 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_26(Rewriter &R) : TheRewriter(R) {}
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
        const clang::VarDecl *TargetUnionDecl;
    };
};

//source file
#include "../include/incorrect_union_initialization_26.h"

// ========================================================================================================
#define MUT26_OUTPUT 1

void MutatorFrontendAction_26::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("unionVar")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;

      const clang::CXXConstructExpr *InitExpr = nullptr;
      if (VD->hasInit()) {
        InitExpr = llvm::dyn_cast<clang::CXXConstructExpr>(VD->getInit());
      }

      if (InitExpr && InitExpr->getNumArgs() == 1) {
        //Get the source code text of target node
        auto InitRange = InitExpr->getSourceRange();
        auto InitText = stringutils::rangetoStr(*(Result.SourceManager), InitRange);

        //Perform mutation on the source code text by applying string replacement
        std::string MutatedText = "{20, .b = 3.14}"; // Example mutation
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(InitRange), MutatedText);
      }
    }
}
  
void MutatorFrontendAction_26::MutatorASTConsumer_26::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(recordDecl(isUnion()))).bind("unionVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}