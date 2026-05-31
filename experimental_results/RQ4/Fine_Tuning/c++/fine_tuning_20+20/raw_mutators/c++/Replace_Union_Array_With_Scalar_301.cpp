//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Union_Array_With_Scalar_301
 */ 
class MutatorFrontendAction_301 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(301)

private:
    class MutatorASTConsumer_301 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_301(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Union_Array_With_Scalar_301.h"

// ========================================================================================================
#define MUT301_OUTPUT 1

void MutatorFrontendAction_301::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FieldDecl>("UnionArrayMember")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto origin = stringutils::rangetoStr(*(Result.SourceManager),
                                            MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      auto type = MT->getType();
      if (type->isArrayType()) {
        auto element_type = type->getArrayElementTypeNoTypeQual();
        if (element_type->isScalarType()) {
          llvm::outs() << "scalar\n";
          origin = element_type.getAsString() + " " + MT->getNameAsString();
        } else {
          llvm::outs() << "pointer\n";
          origin = element_type.getAsString() + " *" + MT->getNameAsString();
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getSourceRange(), origin);
    }
}
  
void MutatorFrontendAction_301::MutatorASTConsumer_301::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = fieldDecl(hasParent(unionDecl()),
                                           hasType(arrayType()))
                                     .bind("UnionArrayMember");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}