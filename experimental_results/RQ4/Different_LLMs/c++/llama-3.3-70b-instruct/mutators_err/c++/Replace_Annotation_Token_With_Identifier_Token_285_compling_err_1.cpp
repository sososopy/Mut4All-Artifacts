```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Annotation_Token_With_Identifier_Token_285
 */ 
class MutatorFrontendAction_285 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(285)

private:
    class MutatorASTConsumer_285 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_285(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Replace_Annotation_Token_With_Identifier_Token_285.h"

// ========================================================================================================
#define MUT285_OUTPUT 1

void MutatorFrontendAction_285::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *AT = Result.Nodes.getNodeAs<clang::AnnotateAttr>(("Annotation"))) {
      //Filter nodes in header files
      if (!AT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     AT->getLocation()))
        return;
      //Get the source code text of target node
      auto annotation = stringutils::rangetoStr(*(Result.SourceManager),
                                                   AT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Replace annotation token with identifier token
      std::string identifierToken = "identifier";
      std::string mutatedAnnotation = annotation;
      mutatedAnnotation.replace(mutatedAnnotation.find("annotate"), 8, identifierToken);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(AT->getSourceRange()), mutatedAnnotation);
    }
}
  
void MutatorFrontendAction_285::MutatorASTConsumer_285::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    attr::AnnotateAttr annotationMatcher = attr::AnnotateAttr().bind("Annotation");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(annotationMatcher, &callback);
    matchFinder.matchAST(Context);
}