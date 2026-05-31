//header file
#pragma once
#include "Mutator_base.h"

/**
 * Union_Empty_Initializer_With_FAM_484
 */ 
class MutatorFrontendAction_484 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(484)

private:
    class MutatorASTConsumer_484 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_484(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Union_Empty_Initializer_With_FAM_484.h"

// ========================================================================================================
#define MUT484_OUTPUT 1

void MutatorFrontendAction_484::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("UnionWithFAM")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto unionName = MT->getNameAsString();
      auto ins = unionName + " mut_484 = {};\n";
      ins = "/*mut484*/" + ins;
      llvm::outs() << ins;
      //Perform mutation on the source code text by applying string replacement
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getEndLoc(), 0, ins);
    }
}
  
void MutatorFrontendAction_484::MutatorASTConsumer_484::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(isUnion(), 
    has(cxxRecordDecl(isStruct(), has(fieldDecl(hasType(arrayType(has(cxxNoProtoType())))))))).bind("UnionWithFAM");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}