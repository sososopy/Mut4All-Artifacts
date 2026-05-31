//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutate_Array_Decl_With_Incomplete_Type_431
 */ 
class MutatorFrontendAction_431 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(431)

private:
    class MutatorASTConsumer_431 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_431(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutate_Array_Decl_With_Incomplete_Type_431.h"

// ========================================================================================================
#define MUT431_OUTPUT 1

void MutatorFrontendAction_431::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ArrayType>("Array")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto decl = MT->getElementType().getAsString();
      llvm::outs() << decl << '\n';
      //Perform mutation on the source code text by applying string replacement
      auto array_decl = stringutils::rangetoStr(*(Result.SourceManager),
                                                MT->getSourceRange());
      llvm::outs() << array_decl << '\n';
      array_decl = array_decl.substr(decl.size());
      array_decl = "/*mut431*/" + array_decl;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), array_decl);
    }
}
  
void MutatorFrontendAction_431::MutatorASTConsumer_431::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(arrayType())).bind("Array");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}