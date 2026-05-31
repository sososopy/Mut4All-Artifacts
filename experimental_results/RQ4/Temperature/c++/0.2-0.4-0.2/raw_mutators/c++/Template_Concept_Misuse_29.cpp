//header file
#pragma once
#include "Mutator_base.h"

/**
 * template_concept_misuse_29
 */ 
class MutatorFrontendAction_29 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(29)

private:
    class MutatorASTConsumer_29 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_29(Rewriter &R) : TheRewriter(R) {}
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
        std::set<std::string> conceptNames;
    };
};

//source file
#include "../include/template_concept_misuse_29.h"

// ========================================================================================================
#define MUT29_OUTPUT 1

void MutatorFrontendAction_29::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ConceptDecl>("ConceptDecl")) {
      //Filter nodes in header files
      if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CTD->getLocation()))
        return;

      //Get the source code text of target node
      auto conceptName = CTD->getNameAsString();
      if (conceptNames.find(conceptName) == conceptNames.end()) {
        conceptNames.insert(conceptName);
        auto sourceRange = CTD->getSourceRange();
        auto sourceText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);
        
        //Perform mutation on the source code text by applying string replacement
        size_t equalPos = sourceText.find('=');
        if (equalPos != std::string::npos) {
          sourceText = sourceText.substr(0, equalPos) + ";";
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), sourceText);
        }
      }
    }
}
  
void MutatorFrontendAction_29::MutatorASTConsumer_29::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = conceptDecl().bind("ConceptDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}