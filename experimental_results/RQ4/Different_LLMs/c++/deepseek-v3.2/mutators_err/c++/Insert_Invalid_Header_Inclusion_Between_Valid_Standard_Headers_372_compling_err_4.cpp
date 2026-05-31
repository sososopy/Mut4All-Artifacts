//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Invalid_Header_Inclusion_Between_Valid_Standard_Headers_372
 */ 
class MutatorFrontendAction_372 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(372)

private:
    class MutatorASTConsumer_372 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_372(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const InclusionDirective*> validIncludes;
    };
};

//source file
#include "../include/Mutator_Insert_Invalid_Header_Inclusion_Between_Valid_Standard_Headers_372.h"

// ========================================================================================================
#define MUT372_OUTPUT 1

void MutatorFrontendAction_372::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<InclusionDirective>("Include")) {
      //Filter nodes in header files
      if (!MT || !Result.SourceManager->isWrittenInMainFile(
                     MT->getSourceRange().getBegin()))
        return;
      //Get the source code text of target node
      //Record the node information to be used in the mutation process
      if (MT->getFileName().str().find("<") != std::string::npos) {
        validIncludes.push_back(MT);
      }
    }
    else if (auto *MT = Result.Nodes.getNodeAs<clang::TranslationUnitDecl>("TU")) {
      if (!MT || !Result.SourceManager->isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Perform mutation on the source code text by applying string replacement
      if (validIncludes.size() > 1) {
        size_t idx = getrandom::getRandomIndex(validIncludes.size() - 1);
        if (idx > 0) {
          const InclusionDirective* targetInclude = validIncludes[idx];
          std::string invalidHeaderName = getrandom::getRandomString(10);
          std::string insertion = "\n#include <" + invalidHeaderName + ">\n";
          //Replace the original AST node with the mutated one
          Rewrite.InsertTextAfterToken(targetInclude->getSourceRange().getEnd(), insertion);
        }
      }
    }
}
  
void MutatorFrontendAction_372::MutatorASTConsumer_372::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher includeMatcher = inclusionDirective(isExpansionInMainFile()).bind("Include");
    DeclarationMatcher tuMatcher = translationUnitDecl().bind("TU");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(includeMatcher, &callback);
    matchFinder.addMatcher(tuMatcher, &callback);
    matchFinder.matchAST(Context);
}