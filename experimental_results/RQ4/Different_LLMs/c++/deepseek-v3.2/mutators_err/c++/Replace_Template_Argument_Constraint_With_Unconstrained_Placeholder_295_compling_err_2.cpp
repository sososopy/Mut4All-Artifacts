//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Argument_Constraint_With_Unconstrained_Placeholder_295
 */ 
class MutatorFrontendAction_295 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(295)

private:
    class MutatorASTConsumer_295 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_295(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_295.h"

// ========================================================================================================
#define MUT295_OUTPUT 1

void MutatorFrontendAction_295::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TP = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("TemplateParam")) {
      //Filter nodes in header files
      if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TP->getLocation()))
        return;
      //Get the source code text of target node
      auto paramText = stringutils::rangetoStr(*(Result.SourceManager), TP->getSourceRange());
      //Check if the parameter has a concept constraint with auto...
      if (TP->hasTypeConstraint() && TP->isParameterPack() && TP->getTypeConstraint()->getConceptName()) {
        auto conceptName = TP->getTypeConstraint()->getConceptName()->getName();
        //Find the concept identifier before auto...
        size_t conceptPos = paramText.find(conceptName);
        if (conceptPos != std::string::npos) {
            //Perform mutation on the source code text by applying string replacement
            //Remove the concept identifier and any trailing whitespace before auto...
            std::string mutatedText = paramText;
            mutatedText.erase(conceptPos, conceptName.length());
            //Remove any extra whitespace between concept and auto
            while (mutatedText[conceptPos] == ' ' || mutatedText[conceptPos] == '\t') {
                mutatedText.erase(conceptPos, 1);
            }
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(TP->getSourceRange()), mutatedText);
        }
      }
    }
}
  
void MutatorFrontendAction_295::MutatorASTConsumer_295::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateTypeParmDecl(hasTypeConstraint(), isParameterPack()).bind("TemplateParam");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}