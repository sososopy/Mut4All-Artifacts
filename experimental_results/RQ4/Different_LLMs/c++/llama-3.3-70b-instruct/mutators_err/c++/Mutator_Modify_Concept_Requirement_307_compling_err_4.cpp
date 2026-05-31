//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

/**
 * Modify_Concept_Requirement_307
 */ 
class MutatorFrontendAction_307 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(307)

private:
    class MutatorASTConsumer_307 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_307(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Mutator_Modify_Concept_Requirement_307.h"
#include "clang/ASTMatchers/ASTMatchersInternal.h"

// ========================================================================================================
#define MUT307_OUTPUT 1

void MutatorFrontendAction_307::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ConceptDecl>(("Concept"))) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = llvm::StringRef::withNullAsEmpty(stringutils::rangeToString(*(Result.SourceManager),
                                                   MT->getSourceRange()));
      //Perform mutation on the source code text by applying string replacement
      // Remove a comma between requirement elements
      size_t commaPos = declaration.find(";");
      if (commaPos != std::string::npos) {
        size_t prevCommaPos = declaration.rfind(",", 0, commaPos);
        if (prevCommaPos != std::string::npos) {
          declaration = declaration.substr(0, prevCommaPos) + declaration.substr(prevCommaPos + 1);
        }
      }
      // Add a comma after a requirement element
      size_t semicolonPos = declaration.find(";");
      if (semicolonPos != std::string::npos) {
        size_t lastSemicolonPos = declaration.rfind(";", 0, semicolonPos);
        if (lastSemicolonPos != std::string::npos) {
          declaration = declaration.substr(0, lastSemicolonPos + 1) + "," + declaration.substr(lastSemicolonPos + 1);
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_307::MutatorASTConsumer_307::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxConceptDecl(isExpansionInMainFile()).bind("Concept");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}