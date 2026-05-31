//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Incomplete_Class_Definition_202
 */ 
class MutatorFrontendAction_202 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(202)

private:
    class MutatorASTConsumer_202 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_202(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Template_Incomplete_Class_Definition_202.h"

// ========================================================================================================
#define MUT202_OUTPUT 1

void MutatorFrontendAction_202::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      declaration = stringutils::remove_comments(declaration);
      llvm::outs() << declaration << '\n';
      auto pos = declaration.find('{');
      if (pos == string::npos)
        return;
      declaration.insert(pos + 1, "/*mut202*/");
      auto rpos = declaration.rfind('}');
      if (rpos == string::npos)
        return;
      declaration.erase(rpos);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_202::MutatorASTConsumer_202::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}