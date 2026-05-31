//header file
#pragma once
#include "Mutator_base.h"

/**
 * NullPointerIntroduction_276
 */ 
class MutatorFrontendAction_NullPointerIntroduction_276 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, llvm::StringRef InFile) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_NullPointerIntroduction_276>(TheRewriter);
    }

private:
    class MutatorASTConsumer_NullPointerIntroduction_276 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_NullPointerIntroduction_276(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_NullPointerIntroduction_276.h"

// ========================================================================================================
#define MUTNULLPOINTERINTRODUCTION_276_OUTPUT 1

void MutatorFrontendAction_NullPointerIntroduction_276::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VarDecl = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecls")) {
      //Filter nodes in header files
      if (!VarDecl || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VarDecl->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   VarDecl->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (VarDecl->getType()->isPointerType()) {
        declaration.replace(declaration.find("=") + 1, declaration.find(";"), "nullptr");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(VarDecl->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_NullPointerIntroduction_276::MutatorASTConsumer_NullPointerIntroduction_276::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(pointerType())).bind("VarDecls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}