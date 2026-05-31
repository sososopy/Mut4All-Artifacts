//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Reference_Init_With_Value_Init_157
 */ 
class MutatorFrontendAction_157 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(157)

private:
    class MutatorASTConsumer_157 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_157(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Replace_Reference_Init_With_Value_Init_157.h"

// ========================================================================================================
#define MUT157_OUTPUT 1

void MutatorFrontendAction_157::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Check if it's a reference declaration
      if (!VD->getType()->isReferenceType())
        return;
      //Check if it has an initializer that is a call expression
      const Expr* Init = VD->getInit();
      if (!Init)
        return;
      //Check if the initializer is a call expression (function or method call)
      if (!llvm::isa<CallExpr>(Init) && !llvm::isa<CXXConstructExpr>(Init))
        return;
      //Get the source code text of target node
      std::string OriginalText = stringutils::rangetoStr(*(Result.SourceManager), VD->getSourceRange());
      //Find the '&' after the type name and remove it
      size_t AmpPos = OriginalText.find('&');
      if (AmpPos == std::string::npos)
        return;
      //Perform mutation on the source code text by applying string replacement
      std::string MutatedText = OriginalText;
      MutatedText.erase(AmpPos, 1);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), MutatedText);
    }
}
  
void MutatorFrontendAction_157::MutatorASTConsumer_157::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasInitializer(anyOf(hasDescendant(callExpr()), hasDescendant(cxxConstructExpr())))).bind("varDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}