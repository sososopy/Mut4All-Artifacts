//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_annotations_in_template_classes_537
 */ 
class MutatorFrontendAction_537 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(537)

private:
    class MutatorASTConsumer_537 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_537(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_annotations_in_template_classes_537.h"

// ========================================================================================================
#define MUT537_OUTPUT 1

void MutatorFrontendAction_537::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithAnnotation")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      
      //Get the source code text of target node
      auto annotationRange = FD->getAttr<AnnotateAttr>()->getRange();
      auto annotationText = stringutils::rangetoStr(*(Result.SourceManager), annotationRange);
      
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedAnnotation = "/*mut537*/[[clang::annotate(\"info\", (9, (void*)0))]]";
      std::string newAnnotationText = annotationText;
      size_t pos = newAnnotationText.find("clang::annotate");
      if (pos != std::string::npos) {
          newAnnotationText.replace(pos, std::string("clang::annotate").length(), "clang::annotate(\"info\", (9, (void*)0))");
      }
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(annotationRange), newAnnotationText);
    }
}
  
void MutatorFrontendAction_537::MutatorASTConsumer_537::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasAttr(attr::Annotate)).bind("FunctionWithAnnotation");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}