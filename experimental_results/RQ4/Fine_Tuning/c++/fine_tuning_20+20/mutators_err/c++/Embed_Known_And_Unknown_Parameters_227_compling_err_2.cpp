//header file
#pragma once
#include "Mutator_base.h"

/**
 * Embed_Known_And_Unknown_Parameters_227
 */ 
class MutatorFrontendAction_227 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(227)

private:
    class MutatorASTConsumer_227 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_227(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Embed_Known_And_Unknown_Parameters_227.h"

// ========================================================================================================
#define MUT227_OUTPUT 1

void MutatorFrontendAction_227::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DL = Result.Nodes.getNodeAs<clang::PreprocessingDirective>("Directives")) {
      //Filter nodes in header files
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (content.find("embed") != string::npos) {
        if (content.find('(') == string::npos) {
          content += "(bla(1))";
        } else {
          content.insert(content.rfind(')'), ",bla(1)");
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_227::MutatorASTConsumer_227::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = preprocessingDirective().bind("Directives");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}