//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Decltype_Initializer_With_Direct_List_Initialization_27
 */ 
class MutatorFrontendAction_27 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(27)

private:
    class MutatorASTConsumer_27 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_27(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT27_OUTPUT 1

void MutatorFrontendAction_27::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      // Check if the type is a DeclTypeType
      auto type = VD->getType();
      if (!type->isDecltypeType())
        return;
      // Check if there is a braced-init-list initializer
      auto init = VD->getInit();
      if (!init)
        return;
      if (!isa<InitListExpr>(init))
        return;
      // Get the underlying expression from decltype
      auto decltypeType = type->getAs<DecltypeType>();
      auto underlyingExpr = decltypeType->getUnderlyingExpr();
      if (!underlyingExpr)
        return;
      // Get source ranges
      auto decltypeRange = CharSourceRange::getTokenRange(VD->getTypeSpecStartLoc(), VD->getTypeSpecEndLoc());
      auto initRange = CharSourceRange::getTokenRange(init->getSourceRange());
      // Get the source text for the underlying expression
      auto underlyingText = stringutils::rangetoStr(*(Result.SourceManager), underlyingExpr->getSourceRange());
      // Get the source text for the initializer
      auto initText = stringutils::rangetoStr(*(Result.SourceManager), init->getSourceRange());
      // Perform mutation: replace decltype(expr) with expr directly
      string mutatedDecl = underlyingText + " " + VD->getNameAsString() + initText;
      // Replace the original AST node with the mutated one
      Rewrite.ReplaceText(VD->getSourceRange(), mutatedDecl);
    }
}
  
void MutatorFrontendAction_27::MutatorASTConsumer_27::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(decltypeType()), hasInitializer(initListExpr())).bind("varDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}