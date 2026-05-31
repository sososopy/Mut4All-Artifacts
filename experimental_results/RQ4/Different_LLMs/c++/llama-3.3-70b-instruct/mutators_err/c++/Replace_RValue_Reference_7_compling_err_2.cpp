```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_RValue_Reference_7
 */ 
class MutatorFrontendAction_7 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef File) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_7>(TheRewriter);
    }

private:
    class MutatorASTConsumer_7 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_7(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_RValue_Reference_7.h"

// ========================================================================================================
#define MUT7_OUTPUT 1

void MutatorFrontendAction_7::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      if (MT->getType().getTypePtr()->isRValueReferenceType()) {
        //Perform mutation on the source code text by applying string replacement
        std::string mutatedDeclaration = declaration;
        size_t pos = mutatedDeclaration.find("&&");
        if (pos != std::string::npos) {
          mutatedDeclaration.replace(pos, 2, "&");
        } else {
          pos = mutatedDeclaration.find("&");
          if (pos != std::string::npos) {
            mutatedDeclaration.erase(pos, 1);
          }
        }
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
      }
    }
    if (auto *MT = Result.Nodes.getNodeAs<clang::ParmVarDecl>("ParmVarDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      if (MT->getType().getTypePtr()->isRValueReferenceType()) {
        //Perform mutation on the source code text by applying string replacement
        std::string mutatedDeclaration = declaration;
        size_t pos = mutatedDeclaration.find("&&");
        if (pos != std::string::npos) {
          mutatedDeclaration.replace(pos, 2, "&");
        } else {
          pos = mutatedDeclaration.find("&");
          if (pos != std::string::npos) {
            mutatedDeclaration.erase(pos, 1);
          }
        }
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
      }
    }
}
  
void MutatorFrontendAction_7::MutatorASTConsumer_7::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher1 = varDecl(hasType(isRValueReferenceType())).bind("VarDecl");
    DeclarationMatcher matcher2 = parmVarDecl(hasType(isRValueReferenceType())).bind("ParmVarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.matchAST(Context);
}