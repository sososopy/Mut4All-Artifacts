//header file
#pragma once
#include "Mutator_base.h"

/**
 * MemberVariableReferenceInConstructor_335
 */ 
class MutatorFrontendAction_335 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef File) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_335>(TheRewriter);
    }

private:
    class MutatorASTConsumer_335 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_335(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_MemberVariableReferenceInConstructor_335.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/AST/AST.h"

// ========================================================================================================
#define MUT335_OUTPUT 1

void MutatorFrontendAction_335::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("Constructor")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      if (auto initList = MT->getInitList()) {
        for (auto init : *initList) {
          if (auto memInit = dyn_cast<CXXCtorInitializer>(&init)) {
            auto member = memInit->getMember();
            if (member) {
              //Perform mutation on the source code text by applying string replacement
              // Replace the member variable with another undeclared variable
              std::string mutatedText = "/*mut335*/" + member->getNameAsString() + "_mut";
              Rewrite.ReplaceText(memInit->getLocStart(), memInit->getLocEnd(), mutatedText);
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_335::MutatorASTConsumer_335::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxConstructorDecl(has(anyOf(hasParameter(0, anyValue()), hasParameter(1, anyValue())))).bind("Constructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}