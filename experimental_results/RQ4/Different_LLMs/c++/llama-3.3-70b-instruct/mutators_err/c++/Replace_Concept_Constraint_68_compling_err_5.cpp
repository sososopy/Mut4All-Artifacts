//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"

/**
 * Replace_Concept_Constraint_68
 */ 
class MutatorFrontendAction_68 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_68>(TheRewriter);
    }

private:
    class MutatorASTConsumer_68 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_68(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Concept_Constraint_68.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/AST/AST.h"
#include "clang/AST/Decl.h"

// ========================================================================================================
#define MUT68_OUTPUT 1

void MutatorFrontendAction_68::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ConceptDecl>("ConceptDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      std::string declaration = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(MT->getSourceRange()), 
          *Result.Context->getSourceManager(), 
          Result.Context->getLangOpts());
      //Perform mutation on the source code text by applying string replacement
      // Replace the concept constraint with a different concept constraint
      std::string newConstraint = "requires(T t) { g(t); }";
      size_t pos = declaration.find("requires");
      if (pos != std::string::npos) {
        declaration = declaration.substr(0, pos) + newConstraint + declaration.substr(declaration.find("}", pos) + 1);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getSourceRange(), declaration);
    }
}
  
void MutatorFrontendAction_68::MutatorASTConsumer_68::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxConceptDecl().bind("ConceptDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}