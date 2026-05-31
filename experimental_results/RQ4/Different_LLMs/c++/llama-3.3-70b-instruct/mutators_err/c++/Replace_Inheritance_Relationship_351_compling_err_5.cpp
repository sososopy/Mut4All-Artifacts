//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Inheritance_Relationship_351
 */ 
class MutatorFrontendAction_351 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef File) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_351>(TheRewriter);
    }

private:
    class MutatorASTConsumer_351 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_351(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Replace_Inheritance_Relationship_351.h"

// ========================================================================================================
#define MUT351_OUTPUT 1

void MutatorFrontendAction_351::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CXXRD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("CXXRecordDecl")) {
      //Filter nodes in header files
      if (!CXXRD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CXXRD->getLocation()))
        return;
      //Get the source code text of target node
      auto bases = CXXRD->bases();
      if (!bases.empty()) {
        // Perform mutation on the source code text by applying string replacement
        // Change the inheritance mode (public, private, protected) if it exists
        for (auto &base : bases) {
          if (base.getAccessSpecifierAsWritten() == AccessSpecifier::AS_public) {
            Rewrite.ReplaceText(base.getBeginLoc(), 6, "private");
          } else if (base.getAccessSpecifierAsWritten() == AccessSpecifier::AS_private) {
            Rewrite.ReplaceText(base.getBeginLoc(), 7, "protected");
          } else if (base.getAccessSpecifierAsWritten() == AccessSpecifier::AS_protected) {
            Rewrite.ReplaceText(base.getBeginLoc(), 9, "public");
          }
        }
        // If a class inherits from multiple base classes, swap the order of the base classes
        if (bases.size() > 1) {
          auto firstBase = *bases.begin();
          auto secondBase = *(bases.begin() + 1);
          Rewrite.ReplaceText(firstBase.getBeginLoc(), 
                              firstBase.getTypeSpecEndLoc().getOffset() - firstBase.getBeginLoc().getOffset(), 
                              secondBase.getType().getAsString() + " " + firstBase.getType().getAsString());
        }
        // Introduce a new base class into the inheritance hierarchy
        Rewrite.InsertTextAfterToken(CXXRD->getEndLoc(), ";\n/*mut351*/struct NewBase {};");
        Rewrite.InsertTextAfterToken(CXXRD->getEndLoc(), ";\n/*mut351*/class " + CXXRD->getNameAsString() + " : public NewBase {};");
        // Remove an existing base class from the inheritance hierarchy
        Rewrite.RemoveText(bases.begin()->getBeginLoc(), 
                           bases.begin()->getTypeSpecEndLoc().getOffset() - bases.begin()->getBeginLoc().getOffset());
      }
    }
}

void MutatorFrontendAction_351::MutatorASTConsumer_351::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(hasAnyBase(cxxBaseSpecifier()));
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}