```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Annotation_Token_281
 */ 
class MutatorFrontendAction_281 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(281)

private:
    class MutatorASTConsumer_281 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_281(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Insert_Annotation_Token_281.h"

// ========================================================================================================
#define MUT281_OUTPUT 1

void MutatorFrontendAction_281::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::AnnotatableAttr>(("Annotations"))) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto annotation = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string newAnnotation = " [[deprecated]]";
      Rewrite.InsertTextAfterToken(MT->getEndLoc(), newAnnotation);
    }
}
  
void MutatorFrontendAction_281::MutatorASTConsumer_281::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = annotatableAttr().bind("Annotations");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}