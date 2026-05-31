//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Typename_Keyword_171
 */ 
class MutatorFrontendAction_Remove_Typename_Keyword_171 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef File) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_Remove_Typename_Keyword_171>(TheRewriter);
    }

private:
    class MutatorASTConsumer_Remove_Typename_Keyword_171 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_Remove_Typename_Keyword_171(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Mutator_Remove_Typename_Keyword_171.h"

// ========================================================================================================
#define MUT171_OUTPUT 1

void MutatorFrontendAction_Remove_Typename_Keyword_171::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("TemplateTypeParmDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      std::string str = Result.Context->getSourceManager().getBuffer(MT->getSourceRange().getBegin().getBufID())->getBuffer().substr(
          MT->getSourceRange().getBegin().getOffset(),
          MT->getSourceRange().getEnd().getOffset() - MT->getSourceRange().getBegin().getOffset());
      //Perform mutation on the source code text by applying string replacement
      size_t typenamePos = str.find("typename");
      if (typenamePos != std::string::npos) {
        str.erase(typenamePos, 8);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getSourceRange(), str);
    }
}
  
void MutatorFrontendAction_Remove_Typename_Keyword_171::MutatorASTConsumer_Remove_Typename_Keyword_171::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateTypeParmDecl().bind("TemplateTypeParmDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}