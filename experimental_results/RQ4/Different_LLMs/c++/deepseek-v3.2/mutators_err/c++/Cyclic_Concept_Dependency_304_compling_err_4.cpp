//header file
#pragma once
#include "Mutator_base.h"

/**
 * Cyclic_Concept_Dependency_304
 */ 
class MutatorFrontendAction_304 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(304)

private:
    class MutatorASTConsumer_304 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_304(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ConceptDecl*> conceptDecls;
    };
};

//source file
#include "../include/Mutator_Cyclic_Concept_Dependency_304.h"

// ========================================================================================================
#define MUT304_OUTPUT 1

void MutatorFrontendAction_304::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CD = Result.Nodes.getNodeAs<clang::ConceptDecl>("ConceptDecl")) {
      //Filter nodes in header files
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
         return;
      //Record the node information to be used in the mutation process
      conceptDecls.push_back(CD);
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      if (conceptDecls.empty())
        return;
      size_t index = getrandom::getRandomIndex(conceptDecls.size() - 1);
      const clang::ConceptDecl* targetConcept = conceptDecls[index];
      std::string conceptName = targetConcept->getNameAsString();
      std::string newConceptName = conceptName + "_cyclic";
      //Perform mutation on the source code text by applying string replacement
      std::string newConceptDef = "template<typename T>\nconcept " + newConceptName + " = requires(T t) { requires " + conceptName + "<T>; };";
      std::string originalConceptText = stringutils::rangetoStr(*(Result.SourceManager), targetConcept->getSourceRange());
      std::string mutatedConceptText = originalConceptText;
      size_t pos = mutatedConceptText.find("requires(T t)");
      if (pos != std::string::npos) {
        size_t endPos = mutatedConceptText.find("};", pos);
        if (endPos != std::string::npos) {
          mutatedConceptText.insert(endPos - 1, "; requires " + newConceptName + "<T>");
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.InsertTextBefore(targetConcept->getBeginLoc(), newConceptDef + "\n");
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(targetConcept->getSourceRange()), mutatedConceptText);
    }
}
  
void MutatorFrontendAction_304::MutatorASTConsumer_304::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher conceptMatcher = conceptDecl().bind("ConceptDecl");
    DeclarationMatcher functionMatcher = functionDecl().bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(conceptMatcher, &callback);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.matchAST(Context);
}