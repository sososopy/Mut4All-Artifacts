//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

/**
 * Replace_Constraint_Requires_Clause_339
 */ 
class MutatorFrontendAction_339 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_339>(TheRewriter);
    }

private:
    class MutatorASTConsumer_339 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_339(Rewriter &R) : TheRewriter(R) {}
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
    Rewriter TheRewriter;
};

//source file
#include "../include/Replace_Constraint_Requires_Clause_339.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

// ========================================================================================================
#define MUT339_OUTPUT 1

void MutatorFrontendAction_339::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ConceptDecl>("Concept")) {
      //Filter nodes in header files
      if (!MT ||!Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Replace the requirement with a different requirement
      // For example, change a equality comparison to a less-than comparison
      size_t pos = declaration.find("==");
      if (pos!= string::npos) {
        declaration.replace(pos, 2, "<");
      } else {
        pos = declaration.find("convertible_to");
        if (pos!= string::npos) {
          declaration.replace(pos, 14, "same_as");
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_339::MutatorASTConsumer_339::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = clang::ast_matchers::cxxConceptDecl().bind("Concept");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}